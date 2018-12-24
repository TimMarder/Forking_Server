#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;
  char input[BUFFER_SIZE];
  char response[BUFFER_SIZE];

  from_server = client_handshake( &to_server );

  while( 1 ) {

      printf("What should we send?\n");
      fgets( input, sizeof( input ), stdin );
      input[ strlen( input ) - 1] = 0;
      write( from_server, input, sizeof( input ) );
      printf("Message successfully sent.\n");
      read( from_server, response, sizeof( response ) );
      printf("Message from server: %s\n", response);

  }

}
