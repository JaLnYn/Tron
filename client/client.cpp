#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#define MY_PORT 8080


void printNewThing(char * buf){
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 7; j++){
      printf("%c", *(i*8 + buf + 1 + j));
    }
    printf("   ");
    if((*buf + i * 8) == '0'){
      printf("ready\n");
    }else{
      printf("not ready\n");
    }
  }
}

int main(int argc, char **argv)
{

  char name[7];
  scanf("%s",name);

  if (argc < 2) {
    fprintf(stderr, "Need server IPv4 address and port.\n");
    return 1;
  }

  int cfd;
  struct sockaddr_in a;

  memset(&a, 0, sizeof(struct sockaddr_in));
  a.sin_family = AF_INET;
  a.sin_port = htons(atoi(argv[2]));
  if (0 == inet_pton(AF_INET, argv[1], &a.sin_addr)) {
    fprintf(stderr, "That's not an IPv4 address.\n");
    return 1;
  }

  cfd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == connect(cfd, (struct sockaddr *)&a, sizeof(struct sockaddr_in))) {
    perror("connect");
    return 1;
  }

  // starts here

  int n;
  char buf[64];
  // n = read(cfd, buf, 100);
  // write(1, buf, n);
  
  strcpy(buf, name);
  write(cfd, buf, strlen(buf));
  
  while(1){
    int cmd = -1;
    scanf("%d",&cmd);
    if(cmd == 1){
      
      n = read(cfd, buf, 64);
      if(strncmp(buf, "kick",4) == 0){
        printf("ya got kicked");
        exit(0);
      }
  
      printNewThing(buf);
    }
  }

  

  close(cfd);
  return 0;
}