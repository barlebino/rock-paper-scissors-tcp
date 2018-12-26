// TODO: detect disconnections

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

  // game loop
  int instruction, move, player_number;
  char input_buffer[32];
  while(1) {
    // read instruction from server
    read(sfd, &instruction, sizeof(int));

    // 0 - wait
    // 1 - enter move
    // 2 - get info
    // 3 - end game

    // process instruction
    if(instruction == 0) {
      printf("waiting for an opponent\n");
      // get player number for the instruction == INFO case
      // player 0 or player 1
      read(sfd, &player_number, sizeof(int));
    } else if(instruction == 1) {
      printf("enter a move (rock, paper, or scissors): ");
      fflush(0);
      read(0, input_buffer, 32);
      
      // 0 - rock
      // 1 - paper
      // 2 - scissors
      switch(input_buffer[0]) {
        case 'r':
          move = 0;
          break;
        case 'p':
          move = 1;
          break;
        case 's':
          move = 2;
          break;
        default: // TODO
          move = 0;
      }

      write(sfd, &move, sizeof(int));
    } else if(instruction == 2) {
      // get match result
      int match_result;
      read(sfd, &match_result, sizeof(int));
      if(match_result == player_number) {
        printf("you won this round\n");
      } else if(match_result == 2) {
        printf("you tied\n");
      } else {
        printf("you lost this round\n");
      }

      // get scores
      int p0score, p1score;
      read(sfd, &p0score, sizeof(int));
      read(sfd, &p1score, sizeof(int));

      // show scores
      if(player_number == 0) {
        printf("your total: %d, opponent total: %d\n",
          p0score, p1score);
      } else if(player_number == 1) {
        printf("your total: %d, opponent total: %d\n",
          p1score, p0score);
      } else { // TODO
        printf("your total: %d, opponent total: %d\n",
          p0score, p1score);
      }
    } else if(instruction == 3) {
      printf("end game\n");
      break;
    }
  }
  write(sfd, &message, sizeof(int));

  close(sfd);
}

