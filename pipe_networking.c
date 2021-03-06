#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int from_client) {

    char pid[HANDSHAKE_BUFFER_SIZE];
    char msg[HANDSHAKE_BUFFER_SIZE];

    printf("Starting handshake...\n");

    remove( "wkp" );
    printf("Removed WKP.\n");

    read( from_client, pid, sizeof( pid ) );
    printf("Subserver %d connecting to %s.\n", getpid(), pid);

    int to_client = open( pid, O_WRONLY );

    write( to_client, ACK, sizeof( ACK ) );
    printf("Written to client.\n");

    read( from_client, msg, sizeof( msg ) );
    printf("%d received confirmation from %s.\n", getpid(), msg);

    printf("Finished handshake.\n");

    return to_client;

}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

    char pid[HANDSHAKE_BUFFER_SIZE];
    char msg[HANDSHAKE_BUFFER_SIZE];

    printf("Starting handshake...\n");

    sprintf( pid, "/tmp/%d", getpid() );

    printf("Client %d connecting to WKP.\n", getpid() );
    mkfifo( pid, 0666 );
    *to_server = open( "wkp", O_WRONLY );

    write( *to_server, pid, sizeof( pid ) );
    printf("%d Sent private pipe.\n", getpid() );

    int from_server = open( pid, O_RDONLY );
    remove( pid );

    read( from_server, msg, sizeof( msg ) );
    printf("Received message from client: %s.\n", msg);

    write( *to_server, "Success", HANDSHAKE_BUFFER_SIZE);
    printf("Sending confirmation.\n");

    printf("Finished handshake.\n");

    return from_server;

}
