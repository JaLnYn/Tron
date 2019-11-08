// this client is used to test the 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define TO_SERVER_BUF 8
#define FROM_SERVER_BUF 32


int main(int argc, char **argv){
  int sockfd, portno, n;
  int serverlen;
  struct sockaddr_in serveraddr;
  struct  hostent *server;
  
  // check command line args

  // create socket

  // get host

  // build server's internet address

  // loop:
  // get message from user
  
  // send message to the server

  // print server's reply


}