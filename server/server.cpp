
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

  game*g = new game();

  int sockfd; // socket
  int port; // my port to listen on
  struct sockaddr_in serveraddr; // server's address
  struct sockaddr_in clientaddrs[4];
  socklen_t clientlen; // client's address size
  struct sockaddr_in clientaddr; // client's address
  socklen_t clientLens[4];
  int currentAddrMax = 0;
  struct hostent * hostp; //host info
  char * hostaddrp; // host adddr string
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
    //TODO store senders 
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
  //TODO finished waiting for all senders. send them start signal
  //MAY BE USEFULL:n = sendto(sockfd, toClientBuf, strlen(toClientBuf), 0, (struct sockaddr *) &clientaddr, clientlen);

  bzero(toClientBuf,TO_CLI_BUF_SIZE);
  g->storeGame(toClientBuf);
  for (int i = 0; i < 32; i++){
    printf("%d ", toClientBuf[i]);
  }

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


  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = 100;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&read_timeout,sizeof(read_timeout)) < 0) {
    perror("Error with options");
  }

  clientlen = sizeof(clientaddr);
  
  using clock = std::chrono::high_resolution_clock;
  
  
  bool quit_game = false;
  unsigned int current_tick = 0;
  double previousTickTime = 10000;
  printf("start loop\n");
  double microseconds = 0;
  double scanTime = 35000000;
  while(!quit_game) {
    auto start = clock::now();

    double timeTillNextTick = (start + timestep - clock::now()).count();
    
    
    while(0 < timeTillNextTick - (double)previousTickTime*2-scanTime*2){ 
      auto tickStart = clock::now();
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
        //printf("server recieved %d/%d bytes: %s\n", (int) strlen(fromClientBuf), n, fromClientBuf);
        // parse input // TODO


        // send current toClientBuffer
        n = sendto(sockfd, toClientBuf, TO_CLI_BUF_SIZE, 0, (struct sockaddr *) &clientaddr, clientlen);
        if(n < 0) {
          perror("ERROR in sendto");
          exit(1);
        }
      }else if(n < 0 && errno!=EAGAIN){
        perror("ERROR in recv from");
        exit(1);
      }
      scanTime = (clock::now() - tickStart).count();
      if(scanTime < 0){
        scanTime = 0;
      }
      timeTillNextTick = (start + timestep - clock::now()).count();
    }
    
    //tick here
    auto tickStart = clock::now();
    //printf("tick: %d\n", current_tick);
    // parse output //TODO



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
    //printf("tick. previous tick time: %lf, slept for %lf\n", previousTickTime, microseconds);
    usleep(microseconds);
    current_tick++;
  }

  
  
  return 0;
  
}