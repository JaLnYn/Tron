#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>


//#include "game/game.hpp"
#include <stdlib.h>
#define BUF_SIZE 64
#define AMT_PLRS 4
#define MY_PORT 31330



void print_client_address(const char *prefix, int fd, const struct sockaddr_in *ptr)
{
  char dot_notation[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &ptr->sin_addr, dot_notation, INET_ADDRSTRLEN);
  printf("%s: fd=%d, %s port %d\n", prefix, fd, dot_notation, ntohs(ptr->sin_port));
}

void writeToAll(int * cfds, char *buf, int size){
  char newBuff[100];
  strcpy(newBuff, buf);
  fprintf(stderr,"%s sent to all\n",newBuff);
  for(int i = 0; i < AMT_PLRS; i++){
    if(*(cfds + i) != -1){
      sendto(*(cfds + i), buf, size, 0, NULL, 0);
    }
    
  }

}



int main(void)
{
  // first set up game
  //game * mygame = new game();
  
  int sfd;
  struct sockaddr_in a;
  int playerSocket[AMT_PLRS];
  for (int i = 0; i < AMT_PLRS; i++){
    playerSocket[i] = -1;
  }
  
  char * playerNames = (char*)malloc(sizeof(char)*8*4);// each player gets a 8 size name
  int buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&a, 0, sizeof(struct sockaddr_in));
  a.sin_family = AF_INET;
  a.sin_port = htons(MY_PORT);
  a.sin_addr.s_addr = htonl(INADDR_ANY);
  if (-1 == bind(sfd, (struct sockaddr *)&a, sizeof(struct sockaddr_in))) {
    perror("bind");
    return 1;
  }
  if (-1 == listen(sfd, 2)) {
    perror("listen");
    return 1;
  }
  fcntl(sfd, F_SETFL, O_NONBLOCK);

  int ep;
  struct epoll_event e;

  ep = epoll_create1(0);
  e.events = EPOLLIN;
  e.data.fd = sfd;
  epoll_ctl(ep, EPOLL_CTL_ADD, sfd, &e);

  

  int readyForGame = false;

  // Henceforth e will be reused for future epoll_* calls.

  while (!readyForGame) {
    epoll_wait(ep, &e, 1, -1);
    if (e.data.fd == sfd) {
      // waiting for connections
      struct sockaddr_in ca;
      socklen_t sinlen = sizeof(struct sockaddr_in);
      int cfd = accept(sfd, (struct sockaddr *)&ca, &sinlen);
      if (cfd != -1) {
        print_client_address("Got client", cfd, &ca);
        e.events = EPOLLIN;
        e.data.fd = cfd;

        epoll_ctl(ep, EPOLL_CTL_ADD, cfd, &e);

        int newPlayerPlaced = 0;
        for (int j = 0; j < AMT_PLRS; j++){
          if(playerSocket[j] == -1){
            // j is empty
            playerSocket[j] = cfd;
        
            // read the name
            char buf[8];
            memset(buf,0,8);
            int n = read(cfd, buf, 8);
            
            fprintf(stderr,"%s joins the match\n",buf);
            for (int i = 0; i < 8; i++){
              *(playerNames + j*8 + i) = buf[i];
            }
            newPlayerPlaced = 1;
            j = AMT_PLRS; // exit loop
          }
        }
        if(newPlayerPlaced == 0){
          // somewhere here I need to tell client that this room is full
        }
        writeToAll(playerSocket, playerNames, 8 * 4 * sizeof(char));
        
      } // else cfd == -1, recall sfd is in non-blocking mode, shrug :)
    } else {
      // e.data.fd is a socket connected to a client. and something happend to it
      int cfd = e.data.fd;
      int n;
      char buf[BUF_SIZE];
      n = read(cfd, buf, BUF_SIZE);
      if (n == 0) {
        
        printf("Client fd=%d leaves.\n", cfd);
        epoll_ctl(ep, EPOLL_CTL_DEL, cfd, NULL);
        close(cfd);
        
        for (int i = 0; i < AMT_PLRS; i++){
          if(playerSocket[i] == cfd){
            playerSocket[i] = -1;
            memset((playerNames + i * 8), 0, 8);
          }
        }
        
        writeToAll(playerSocket, playerNames, 8 * 4 * sizeof(char));
      } else if (n > 0) {
        // check if they are asking to start. if they are, then we start
        int isStart = strncmp(buf,"start",5);
        if(isStart == 0){
          readyForGame = 1;
        }
      } 
    }
  }

  // then wait for users to connect
  fprintf(stderr,"Game Start\n\n");
  // then game loop

  return 0;
  
}