#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <ncurses.h>
#include "../game/game.hpp"


#define FROM_SER_BUF_SIZE 32
#define TO_SER_BUF_SIZE 8
//load the game onto the window
void loadWin(game* g, WINDOW * w){
  for (int x = 0; x < MAP_WIDTH; x++){
    for(int y = 0; y < MAP_HEIGHT; y++){
      mvwprintw(w, y+1,x*2+1,"%c",g->blockVal(x,y)+'0');
    }
  }
  move(0,0);
  wrefresh(w);
}

int main(int argc, char **argv){
  
  int sockfd, portno, n;
  socklen_t serverlen;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char *hostname;


  char toServerBuf[TO_SER_BUF_SIZE];
  char fromServerBuf[FROM_SER_BUF_SIZE];

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

  if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) { 
      printf("\n Error : Connect Failed \n"); 
      exit(0); 
  } 

  bzero(toServerBuf, TO_SER_BUF_SIZE);
  int key = 0;
  printf("Please enter your key: ");

  scanf("%d",&key);
  if(key > 9999 || key < 0){
    printf("INVALID KEY\n");
    exit(1);
  }
  toServerBuf[0] = key/1000 + '0';
  toServerBuf[1] = (key%1000)/100 + '0';
  toServerBuf[2] = (key%100)/10 +'0';
  toServerBuf[3] = key%10 + '0';

  serverlen = sizeof(serveraddr);
  
  n = sendto(sockfd, toServerBuf, strlen(toServerBuf), 0, ( struct sockaddr *) &serveraddr, serverlen);
  
  if (n < 0){
    perror("ERROR: sendto\n");
    exit(0);
  }

  // first lets set up n curses

  game*g = new game();
  //g->loadGame("");

  int startx=10,starty=10;
  initscr();
  cbreak();
  noecho();

  WINDOW * board = newwin(MAP_HEIGHT+2,2*MAP_WIDTH+1, starty, startx);
  refresh();  
  box(board, 0, 0);
  
  

	int c = getch();
  int dir;
  while(c != 'c'){
    // change direc
    if(c == 'a'){
      dir = 1;
    }else if(c == 's'){
      dir = 2;
    }else if(c == 'd'){
      dir = 3;
    }else if(c == 'w'){
      dir = 0;
    }
    g->setDirection(0,dir);
    g->tick();
    loadWin(g,board);

    c = getch();
  }
  
	endwin();

  return 0;
}