
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <errno.h>

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timestep(125ms);

//#include "game/game.hpp"
#define TO_CLI_BUF_SIZE 32
#define FROM_CLI_BUF_SIZE 8
#define AMT_PLRS 2

#define NAMESIZE 7

//print error messaage

int main(int argc, char ** argv){
  int sockfd; // socket
  int port; // my port to listen on
  socklen_t clientlen; // client's address size
  struct sockaddr_in serveraddr; // server's address
  struct sockaddr_in clientaddr; // client's address
  struct hostent * hostp; //host info
  char * hostaddrp; // host adddr string
  char toClientBuf[TO_CLI_BUF_SIZE];
  char fromClientBuf[FROM_CLI_BUF_SIZE];

  // check command line args

  if(argc != 2){
    perror("usage: file <port>");
    exit(1);
  }
  port = atoi(argv[1]);

  // create socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd<0){
    perror("ERROR: opening socket.");
    exit(1);
  }

  //int option = 1;
  //setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&option, sizeof(int));

  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = 100;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&read_timeout,sizeof(read_timeout)) < 0) {
    perror("Error with options");
  }

  //internet stuff
  bzero((char*) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port);

  if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
    perror("ERROR on bind");
    exit(1);
  }

  //main loop
  clientlen = sizeof(clientaddr);
  
  using clock = std::chrono::high_resolution_clock;
  
  
  bool quit_game = false;
  unsigned int current_tick = 0;
  double previousTickTime = 10000;
  printf("start loop\n");
  double microseconds = 0;
  while(!quit_game) {
    auto start = clock::now();

    double timeTillNextTick = (start + timestep - clock::now()).count();
    
    // do work here
    // 
    while(0 < timeTillNextTick - (double)previousTickTime*2-65000000){ 
      bzero(fromClientBuf, FROM_CLI_BUF_SIZE);
      int n = recvfrom(sockfd, fromClientBuf,FROM_CLI_BUF_SIZE, 0, (struct sockaddr*) &clientaddr, &clientlen);
      
      if(n >= 0){
        hostp = gethostbyaddr((const char *) &clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if(hostp == NULL) {
          perror("ERROR on gethostbyaddr");
          exit(1);
        }
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if(hostaddrp == NULL){ 
          perror("ERROR on inet_ntoa");
          exit(1);
        }
        printf("server recieved %d/%d bytes: %s\n", (int) strlen(fromClientBuf), n, fromClientBuf);
        bzero(toClientBuf, TO_CLI_BUF_SIZE);
        strcpy(toClientBuf, fromClientBuf);
        strcat(toClientBuf,"!");
        n = sendto(sockfd, toClientBuf, strlen(toClientBuf), 0, (struct sockaddr *) &clientaddr, clientlen);
        if(n < 0) {
          perror("ERROR in sendto");
          exit(1);
        }
      }else if(n < 0 && errno!=EAGAIN){
        perror("ERROR in recv from");
        exit(1);
      }
      
      timeTillNextTick = (start + timestep - clock::now()).count();
    }
    
    //tick here
    auto tickStart = clock::now();
    printf("tick: %d\n", current_tick);
    usleep(1000);
    previousTickTime = (clock::now() - tickStart).count();
    if(previousTickTime < 0){
      previousTickTime = 0;
    }
    auto x = start + timestep - clock::now();
    microseconds = x.count()/1000.0;
    
    if(microseconds < 0){
      printf("running late %lf!!!!\n", microseconds);
      microseconds = 0;
    }
    printf("tick. previous tick time: %lf, slept for %lf\n", previousTickTime, microseconds);
    usleep(microseconds);
    current_tick++;
  }

  
  
  return 0;
  
}