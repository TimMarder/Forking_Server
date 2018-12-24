#include "pipe_networking.h"


int main() {

  //signal( SIGINT, sighandler );

  int to_client;
  int from_client;
  char input[BUFFER_SIZE];


  while( 1 ) {

      printf("Waiting for Client...\n");
      mkfifo( "wkp", 0666 );
      from_client = open( "wkp", O_RDONLY );
      printf("Connection successful.\n");
      printf("Begin Forking...\n");
      if ( fork() ) {
          close( from_client );
          remove( "wkp" );
      }
      else {

          to_client = server_handshake( from_client );

          while( read( from_client, input, sizeof( input ) ) ) {

              printf("Message received from CLIENT: %s\n", input);
              for (char *i = input; *i; i++) {

                  (*i)++;

              }
              write( to_client, input, sizeof( input ) );
              printf("Sent back to CLIENT: %s\n", input);

          }

      }

  }

}


static void sighandler( int n ) {

    if (n == SIGINT) {

        remove("wkp");
        printf("Exiting...\n");
        exit( 0 );

    }

}
