#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main(int argc, char **argv) {
  int sfd;
  sfd = socket(AF_INET, SOCK_STREAM, 0);

  // server info
  struct sockaddr_in serv_addr;
  int port_no = atoi(argv[1]);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_no);

  // bind socket to server info
  bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  // get two players
  int p1fd, p2fd;

  listen(sfd, 5);
  p1fd = accept(sfd, NULL, NULL);

  // TODO: wait for second player

  listen(sfd, 5);
  p2fd = accept(sfd, NULL, NULL);

  // get a number from each player
  int p1num, p2num;
  
  read(p1fd, &p1num, sizeof(int));
  read(p2fd, &p2num, sizeof(int));

  // close socket
  close(sfd);

  // print
  printf("p1num: %d\n", p1num);
  printf("p2num: %d\n", p2num);
}

