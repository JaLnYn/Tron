// this client is used to test the 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define TO_SERVER_BUF 8
#define FROM_SERVER_BUF 32

int main(int argc, char **argv){
  int sockfd, portno, n;
  socklen_t serverlen;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char *hostname;

  char toServerBuf[TO_SERVER_BUF];
  char fromServerBuf[FROM_SERVER_BUF];

  // check command line args

  if (argc != 3) {
    perror("usage: filename <hostname> <port>\n");
    exit(0);
  }
  hostname = argv[1];
  portno = atoi(argv[2]);

  // create socket

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("ERROR: opening sockets\n");
    exit(0);
  }

  // get host
  server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host as %s\n", hostname);
    exit(0);
  }

  // build server's internet address

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portno);

  bzero(toServerBuf, TO_SERVER_BUF);
  printf("Please enter msg: ");
  fgets(toServerBuf, TO_SERVER_BUF, stdin);
  toServerBuf[strlen(toServerBuf)-1] = 0;

  serverlen = sizeof(serveraddr);
  
  n = sendto(sockfd, toServerBuf, strlen(toServerBuf), 0, ( struct sockaddr *) &serveraddr, serverlen);
  
  if (n < 0){
    perror("ERROR: sendto\n");
    exit(0);
  }
  
  // print server's reply
  bzero(fromServerBuf, FROM_SERVER_BUF);
  n = recvfrom(sockfd, fromServerBuf, FROM_SERVER_BUF, 0,( struct sockaddr *) &serveraddr, &serverlen);
  if (n < 0){
    perror("ERROR: recvfrom\n");
    exit(0);
  }
  printf("Echo from server: %s %d,%d\n", fromServerBuf, (int)strlen(fromServerBuf), n);
  return 0;


}