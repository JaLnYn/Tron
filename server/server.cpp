
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
#include <ifaddrs.h>
#include "../game/game.hpp"
#include <sys/epoll.h>
#include <sys/prctl.h>
#include <signal.h>


using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timestep(125ms);


#define TO_CLI_BUF_SIZE 32
#define FROM_CLI_BUF_SIZE 8

// This function is used to print addresses


void printAddr(){

  struct ifaddrs * ifAddrStruct=NULL;
  struct ifaddrs * ifa=NULL;
  void * tmpAddrPtr=NULL;
  
  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr) {
      continue;
    }
    if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
      // is a valid IP4 Address
      tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      char addressBuffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
    } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
      // is a valid IP6 Address
      tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
        char addressBuffer[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
      printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
    } 
  }
  if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}

int extractCommand(unsigned char * fromClientBuf){
  int cmd = (fromClientBuf[4]-'0')*100;
      cmd += (fromClientBuf[5]-'0')*10;
      cmd += (fromClientBuf[6]-'0')*1;
  return cmd;
}
int extractKey(unsigned char * fromClientBuf){
  int key = (fromClientBuf[0]-'0')*1000;
      key += (fromClientBuf[1]-'0')*100;
      key += (fromClientBuf[2]-'0')*10;
      key += (fromClientBuf[3]-'0');
  return key;
}



int main(int argc, char ** argv){

  //seed rand
  srand(time(NULL));

  int toChdfd[2]; // 0 is to parent 1 is to child
  int toParfd[2]; // 0 is to parent 1 is to child
  int childId;
  
 
  if ((pipe(toParfd) == -1) || (pipe(toChdfd) == -1) || (childId = fork()) == -1){
      perror("pipe/fork failed :(");
      return 1;
  }
  if (childId == 0){ // child{
    prctl(PR_SET_PDEATHSIG, SIGHUP);
    

    close (toParfd[0]);
    close (toChdfd[1]);
    
    int c = 0;
    read(toChdfd[0],&c,1);
    if(c == 1){
      c = 2;
      using clock = std::chrono::high_resolution_clock;
      double microseconds = 0;
      while(1) {
        auto start = clock::now();
        auto x = start + timestep - clock::now();
        microseconds = x.count()/1000.0;
        
        if(microseconds < 0){
          microseconds = 0;
        }
        write(toParfd[1],&c,1);
        usleep(microseconds);
      }
    }
    return 1;
  } else { // parent
        
    game*g = new game();

    int sockfd; // socket
    int port; // my port to listen on
    struct sockaddr_in serveraddr; // server's address
    struct sockaddr_in clientaddrs[4];
    socklen_t clientlen; // client's address size
    struct sockaddr_in clientaddr; // client's address
    socklen_t clientLens[4];
    int currentAddrMax = 0;
    //struct hostent * hostp; //host info
    //char * hostaddrp; // host adddr string
    unsigned char toClientBuf[TO_CLI_BUF_SIZE];
    unsigned char fromClientBuf[FROM_CLI_BUF_SIZE];


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


    //internet stuff
    bzero((char*) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
      perror("ERROR on bind");
      exit(1);
    }



    int playerKeys[4];
    int playerJoined[4];
    printf("(you can enter 1,2,3 or 4)\n");
    printf("Enter amount of players: \n");
    int amountPlayers = 0;
    scanf("%d",&amountPlayers);
    // hacky way to clear screen
    printf("\033[H\033[J");
    printAddr();
    printf("PORT: %d\n", port);
    printf("player| key| in\n");
    for(int i = 0; i < 4; i++){
      bool keyExists = true;
      while (keyExists == true ){
        playerKeys[i] = rand()%10000;
        playerJoined[i] = 0;
        keyExists = false;
        for(int j = 0; j < i; j++){
          if(playerKeys[i] == playerKeys[j]){
            keyExists = true;
          }
        }
      }

      printf("%d     |%04d|",i+1, playerKeys[i]);
      if(playerJoined[i] == 1){
        printf(" o\n");
      }else{
        printf(" x\n");
      }
      fflush(stdin);
    } 
    for(int i = 0; i < amountPlayers;){

      bzero(fromClientBuf, FROM_CLI_BUF_SIZE);
      clientLens[currentAddrMax] = sizeof(clientaddrs[currentAddrMax]);
      int n = recvfrom(sockfd, fromClientBuf,FROM_CLI_BUF_SIZE, 0, (struct sockaddr*) &clientaddrs[currentAddrMax], &(clientLens[currentAddrMax]));
      
      if(n>0){
        printf("\033[H\033[J");
        int key = extractKey(fromClientBuf);
        int cmd = extractCommand(fromClientBuf);
        if(cmd == 0){
          for (int j = 0; j < 4; j++){
            if(playerKeys[j] == key && playerJoined[j] == 0){
              playerJoined[j] = 1;
              currentAddrMax++;
              i++;
            }
          }
        }
        
        
        printAddr();
        printf("PORT: %d\n", port);
        printf("%d, %d attempted to join.\n", key, cmd);
        printf("player| key| in\n");
        for(int i = 0; i < 4; i++){
          printf("%d     |%04d|",i+1, playerKeys[i]);
          if(playerJoined[i] != 0){
            printf(" o\n");
          }else{
            printf(" x\n");
          }
        }
        // decode key

      }
    }
    
    bzero(toClientBuf,TO_CLI_BUF_SIZE);
    g->storeGame(toClientBuf);
    

    for(int j = 0; j < currentAddrMax; j++){
      int n = sendto(sockfd, toClientBuf, TO_CLI_BUF_SIZE, 0, (struct sockaddr *) &clientaddrs[j], (clientLens[j]));
      if(n < 0) {
        perror("ERROR in sendto");
        exit(1);
      }
    }

    // wait for connections
    //main loop
    //set some options 

    int ep = epoll_create1(0);
    struct epoll_event e1,e2, e[2]; // e1 for sockfd, e2 for timer
    memset(&e1, 0, sizeof(struct epoll_event));
    e1.events = EPOLLIN; 
    e1.data.fd = sockfd;
    epoll_ctl(ep, EPOLL_CTL_ADD, sockfd, &e1);
    memset(&e2, 0, sizeof(struct epoll_event));
    e2.events = EPOLLIN; 
    e2.data.fd = toParfd[0];
    epoll_ctl(ep, EPOLL_CTL_ADD, toParfd[0], &e2);
    

    clientlen = sizeof(clientaddr);
    printf("cid: %d\n", childId);
    printf("Start Loop\n");
    char c = 1;

    write(toChdfd[1],&c,1);

    while (1){ 
      int n = epoll_wait(ep, e, 2, -1);
      for(int i = 0; i < n; i++){
        if (e[i].data.fd == sockfd) {
          //
          bzero(fromClientBuf, FROM_CLI_BUF_SIZE);
          int n = recvfrom(sockfd, fromClientBuf,FROM_CLI_BUF_SIZE, 0, (struct sockaddr*) &clientaddr, &clientlen);
          if(n <= 0){
            perror("ERROR in recv from");
            exit(1);
          }
          printf("Recieved %s\n",fromClientBuf);
          int plr = -1;
          int theKey = extractKey(fromClientBuf);
          // for (int p = 0; p < AMT_PLRS; p++)
          // {
          //   if(playerKeys[i] == ){
            //TODO here
          //   }
          // }
          
        }else if(e[i].data.fd == toParfd[0]){
          read(toParfd[0], &c, 1);
          g->tick();
          bzero(toClientBuf, TO_CLI_BUF_SIZE);
          g->storeGame(toClientBuf);
          for(int j = 0; j < currentAddrMax; j++){
            int n = sendto(sockfd, toClientBuf, TO_CLI_BUF_SIZE, 0, (struct sockaddr *) &clientaddrs[j], (clientLens[j]));
            //printf("sent: %d\n",n);
            if(n < 0) {
              perror("ERROR in sendto");
              exit(1);
            }
          }
        }
      }
    }
    


  }

  

  
  return 0;
  
}