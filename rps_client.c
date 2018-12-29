// TODO: detect disconnections

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv) {
  char *host_ip = argv[1];
  int port_no = atoi(argv[2]);

  int sfd = socket(AF_INET6, SOCK_STREAM, 0);

  // networking setup
  struct sockaddr_in6 serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr)); // zero out serv_addr
  serv_addr.sin6_family = AF_INET6;
  serv_addr.sin6_port = htons(port_no);
  inet_pton(AF_INET6, host_ip, &(serv_addr.sin6_addr));
  connect(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  // game loop
  int player_number;
  while(1) {
    // read instruction from server
    int instruction;
    read(sfd, &instruction, sizeof(int));

    // 0 - wait
    // 1 - enter move
    // 2 - get info
    // 3 - end game

    // process instruction
    if(instruction == 0) {
      printf("waiting for an opponent\n");
      // player_number used in instruction == "get info" case
      // player 0 or player 1
      read(sfd, &player_number, sizeof(int));
    } else if(instruction == 1) {
      printf("enter a move (rock, paper, or scissors): ");
      fflush(0); // fixes interaction between "printf" and "read(0, ...)"

      // get input from the human player
      char input_buffer[32];
      read(0, input_buffer, 32);
      
      // 0 - rock
      // 1 - paper
      // 2 - scissors
      int move;
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

      // send move
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
      int player0_score, player1_score;
      read(sfd, &player0_score, sizeof(int));
      read(sfd, &player1_score, sizeof(int));

      // show scores
      if(player_number == 0) {
        printf("your total: %d, opponent total: %d\n",
          player0_score, player1_score);
      } else if(player_number == 1) {
        printf("your total: %d, opponent total: %d\n",
          player1_score, player0_score);
      } else { // TODO
        printf("your total: %d, opponent total: %d\n",
          player0_score, player1_score);
      }
    } else if(instruction == 3) {
      printf("end game\n");
      break;
    }
  }

  close(sfd);
}

