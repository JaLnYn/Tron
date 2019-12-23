#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <ncurses.h>
#include <errno.h>
#include <fcntl.h>
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

void setKey(unsigned char * toServerBuf, int key){

  toServerBuf[0] = key/1000 + '0';
  toServerBuf[1] = (key%1000)/100 + '0';
  toServerBuf[2] = (key%100)/10 +'0';
  toServerBuf[3] = key%10 + '0';
}
void setCmd(unsigned char * toServerBuf, int cmd){
  toServerBuf[4] = (cmd)/100 + '0';
  toServerBuf[5] = (cmd%100)/10 +'0';
  toServerBuf[6] = cmd%10 + '0';
}

void setSendItem(unsigned char * toserv, int key, int cmd, unsigned char final){
  bzero(toserv, 8);
  setKey(toserv, key);
  setCmd(toserv, cmd);
  toserv[7] = final + '0';
}

int main(int argc, char **argv){
  
  int sockfd, portno, n;
  socklen_t serverlen;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char *hostname;
  game * g = new game();
  

  unsigned char toServerBuf[TO_SER_BUF_SIZE];
  unsigned char fromServerBuf[FROM_SER_BUF_SIZE];

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

  bzero((unsigned char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((unsigned char *)server->h_addr, 
  (unsigned char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portno);
  serverlen = sizeof(serveraddr);

  // connect to server 
  if(connect(sockfd, (struct sockaddr *)&serveraddr, serverlen) < 0) { 
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

  setSendItem(toServerBuf,key,0,0);

  n = sendto(sockfd, toServerBuf, TO_SER_BUF_SIZE, 0, ( struct sockaddr *) &serveraddr, serverlen);
  printf("%d, %s was sent\n", n,toServerBuf);
  if (n < 0){
    perror("ERROR: sendto");
    exit(0);
  }
  
  bzero(fromServerBuf, FROM_SER_BUF_SIZE);
  n = recvfrom(sockfd, fromServerBuf, FROM_SER_BUF_SIZE, 0,( struct sockaddr *) &serveraddr, &serverlen);
  if(n < 0) {
    perror("ERROR in recv");
    exit(1);
  }

  int startx=10,starty=10;
  initscr();
  cbreak();
  noecho();

  WINDOW * board = newwin(MAP_HEIGHT+2,2*MAP_WIDTH+1, starty, startx);
  refresh();  
  box(board, 0, 0);

  
  
  
  fcntl(sockfd, F_SETFL, O_NONBLOCK); 
  //nodelay(stdscr, true);
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); 
  initscr();
  noecho();
  int keyboardtick = 0;
  int servertick = 0;
  int ep = epoll_create1(0);
  struct epoll_event e1,e2, e[2]; // e1 for serverfd, e2 for stdin
  memset(&e1, 0, sizeof(struct epoll_event));
  e1.events = EPOLLIN; 
  e1.data.fd = sockfd;
  epoll_ctl(ep, EPOLL_CTL_ADD, sockfd, &e1); 
  memset(&e2, 0, sizeof(struct epoll_event));
  e2.events = EPOLLIN; 
  e2.data.fd = STDIN_FILENO;
  epoll_ctl(ep, EPOLL_CTL_ADD, STDIN_FILENO, &e2);
  
  mvprintw(0,0,"ticks from server: %d",servertick);
  mvprintw(2,0,"ticks from keyboard: %d",keyboardtick);
  int normalTick = 0;
  g->resetBoard();
  while (1){ 
    int n = epoll_wait(ep, e, 2, -1);
    for(int i = 0; i < n; i++){
      if (e[i].data.fd == sockfd) { // from server
        //
        bzero(fromServerBuf, FROM_SER_BUF_SIZE);
        n=recvfrom(sockfd, fromServerBuf, FROM_SER_BUF_SIZE, 0,( struct sockaddr *) &serveraddr, &serverlen);
        while(n>=0){
          if(n == 32){
            g->loadGame(fromServerBuf);
          }
          bzero(fromServerBuf, FROM_SER_BUF_SIZE);
          n=recvfrom(sockfd, fromServerBuf, FROM_SER_BUF_SIZE, 0,( struct sockaddr *) &serveraddr, &serverlen);
        }
        servertick+=1;
        loadWin(g,board);
        mvprintw(0,0,"ticks from server: %d",servertick);
        
        if(n < 0 && errno != EAGAIN) {
          perror("ERROR in recv");
          exit(1);
        }
      }else if(e[i].data.fd == STDIN_FILENO){
        char c = getch();
        //fprintf(stderr,"randomly got %d\n", c);
        if(c>=0){
          int sendCmd = 1;
          int final = 1;
          if(c == 'w'){
            final = 0;
          }else if(c == 'a'){
            final = 1;
          }else if (c == 's'){
            final = 2;
          }else if (c == 'd'){
            final = 3;
          }
          setSendItem(toServerBuf, key, sendCmd, final);
          n = sendto(sockfd, toServerBuf, TO_SER_BUF_SIZE, 0, ( struct sockaddr *) &serveraddr, serverlen);
          if (n < 0){
            perror("ERROR: sendto");
            exit(0);
          }
          
          keyboardtick++;
          mvprintw(2,0,"ticks from keyboard: %d %d",keyboardtick, c);
        }
      }
    }
    normalTick++;
    mvprintw(3,0,"ticks: %d",normalTick);
    refresh();
  }
  //loadWin(g,board);

  
	endwin();

  return 0;
}
