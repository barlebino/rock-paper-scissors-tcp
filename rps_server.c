// TODO: detect disconnections

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

enum ttt_message {
  WAIT = 0,
  ENTER_MOVE = 1,
  INFO = 2,
  END_GAME = 3
};

enum ttt_move {
  ROCK = 0,
  PAPER = 1,
  SCISSORS = 2
};

int main(int argc, char **argv) {
  int port_no = atoi(argv[1]);
  int sfd = socket(AF_INET6, SOCK_STREAM, 0);

  /*
  // server info
  struct sockaddr_in serv_addr;
  int port_no = atoi(argv[1]);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_no);

  // bind socket to server info
  bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  */

  // networking setup
  struct sockaddr_in6 serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr)); // zero out serv_addr
  serv_addr.sin6_family = AF_INET6;
  serv_addr.sin6_port = htons(port_no);
  serv_addr.sin6_addr = in6addr_any;
  bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  // get two players
  int p0fd, p1fd;
  int temp_message, temp_player_id;

  // messages
  // 0 - wait
  // 1 - enter move
  // 2 - get info
  // 3 - end game

  printf("looking for first player\n");
  listen(sfd, 5);
  p0fd = accept(sfd, NULL, NULL);
  temp_message = 0;
  temp_player_id = 0;
  write(p0fd, &temp_message, sizeof(int));
  write(p0fd, &temp_player_id, sizeof(int));

  printf("looking for second player\n");
  listen(sfd, 5);
  p1fd = accept(sfd, NULL, NULL);
  temp_message = 0;
  temp_player_id = 1;
  write(p1fd, &temp_message, sizeof(enum ttt_message));
  write(p1fd, &temp_player_id, sizeof(int));

  // prepare 3 rounds of rock, paper, scissors
  int p0score, p1score;
  p0score = 0;
  p1score = 0;
  for(int i = 0; i < 3; i++) {
    // ask clients to enter move
    temp_message = 1;
    write(p0fd, &temp_message, sizeof(int));
    write(p1fd, &temp_message, sizeof(int));

    // 0 - rock
    // 1 - paper
    // 2 - scissors

    // get moves
    int p0move, p1move;
    read(p0fd, &p0move, sizeof(int)); // TODO: size consistency
    read(p1fd, &p1move, sizeof(int));

    // process moves
    int match_result = -1;
    // match result
    if(p0move == 0) {
      if(p1move == 0) {
        match_result = 2;
      } else if(p1move == 1) {
        match_result = 1;
      } else if(p1move == 2) {
        match_result = 0;
      }
    } else if(p0move == 1) {
      if(p1move == 0) {
        match_result = 0;
      } else if(p1move == 1) {
        match_result = 2;
      } else if(p1move == 2) {
        match_result = 1;
      }
    } else if(p0move == 2) {
      if(p1move == 0) {
        match_result = 1;
      } else if(p1move == 1) {
        match_result = 0;
      } else if(p1move == 2) {
        match_result = 2;
      }
    }
    // total scores
    if(match_result == 0) {
      p0score = p0score + 1;
    } else if(match_result == 1) {
      p1score = p1score + 1;
    }

    // tell clients they'll get info
    temp_message = 2;
    write(p0fd, &temp_message, sizeof(int));
    write(p1fd, &temp_message, sizeof(int));
    // send match results
    write(p0fd, &match_result, sizeof(int));
    write(p1fd, &match_result, sizeof(int));
    // send total scores
    write(p0fd, &p0score, sizeof(int));
    write(p1fd, &p0score, sizeof(int));
    write(p0fd, &p1score, sizeof(int));
    write(p1fd, &p1score, sizeof(int));
  }
  
  temp_message = 3;
  write(p0fd, &temp_message, sizeof(int));
  write(p1fd, &temp_message, sizeof(int));

  close(sfd);

  return;

  /*
  // get a number from each player
  int p1num, p2num;
  
  read(p1fd, &p1num, sizeof(int));
  read(p2fd, &p2num, sizeof(int));

  // close socket
  close(sfd);

  // print
  printf("p1num: %d\n", p1num);
  printf("p2num: %d\n", p2num);
  */
}

