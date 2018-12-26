#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv) {
  char *hostname = argv[1];
  int port_no = atoi(argv[2]);
  int message = atoi(argv[3]);

  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  
  // set server information
  struct hostent *server;
  struct sockaddr_in serv_addr;
  server = gethostbyname(hostname);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memmove(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port_no);

  connect(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  write(sfd, &message, sizeof(int));

  close(sfd);
}

