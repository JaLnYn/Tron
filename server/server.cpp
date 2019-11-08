
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>


//#include "game/game.hpp"
#define TO_CLI_BUF 32
#define FROM_CLI_BUF 8
#define AMT_PLRS 2

#define NAMESIZE 7

int main(int argc, char ** argv)
{
  int sockfd; // socket
  int port; // my port to listen on
  int clilen; // client's address size
  struct sockaddr_in serveraddr; // server's address
  struct sockaddr_in clientaddr; // client's address
  struct hostent * hostp; //host info
  char * hostaddrp; // host adddr string
  char toClient[TO_CLI_BUF];

  // check command line args

  // create socket

  //internet stuff

  //main loop

  // get message and return
  
  
  return 0;
  
}