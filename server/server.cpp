// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <vector> 
#include <chrono>
#include <ctime>
#include <fcntl.h>


#define PORT 8080 
#define MAXLINE 64
#define NAMESIZE 7
#define AMTOFPLR 4

struct user{
  int id;
  int isReady;
  int key;
  sockaddr_in addr;
  char * name;
};

// Param:
//  vec: vec is the vector from which we will check the users
//  key: is the key that should match a user
//  addr: is the address the request is from, we should update the user's address if it is incorrect
int checkKey(std::vector<user> * vec, int key ,sockaddr_in addr){
  for(int i = 0; i < vec->size; i++){
    if((*vec)[i].key == key){
      (*vec)[i].addr = addr;
      return i;
    }
  }
  return -1;
}

void setBuffer(std::vector<user>*users, char * buffer){
  
  memset(buffer, 0, sizeof(char)*MAXLINE);
  for (int i = 0; i < AMTOFPLR; i++){
    // here we put 1 (not ready) or 0 (ready) depending on if they are ready
    *(buffer + i * (NAMESIZE + 1)) = (*users)[i].isReady + '0';
    for (int j = 0; j < NAMESIZE; j++){
      *(buffer + i * (NAMESIZE + 1) + 1 + j) = *((*users)[i].name + j);
    }
  }
    
  
}

int main() { 
  int sockfd; 
  char buffer[MAXLINE]; 
  char cmd[2];

  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr; 
  memset(&servaddr, 0, sizeof(servaddr)); 
  memset(&cliaddr, 0, sizeof(cliaddr)); 
  
  // Creating socket file descriptor 
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(1); 
  } 
    
  // server info
  servaddr.sin_family = AF_INET; // IPv4 
  servaddr.sin_addr.s_addr = INADDR_ANY; 
  servaddr.sin_port = htons(PORT); 
    
  // Bind the socket with the server address 
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  } 
  
  socklen_t len;
  int n; 


  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = 100;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

  // actual stuff starts here

  const int FRAMES_PER_SECOND = 60;
  const std::chrono::milliseconds SKIP_TICKS( 1000 / FRAMES_PER_SECOND);
  
  int gameReady = 0;
  std::vector<user> users;

  while( !gameReady) {
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
    
    if(n > 0){
      cmd[0] = *(buffer);
      cmd[1] = *(buffer+1);
      if(strcmp(cmd, "jn") == 0){
        user u;
        u.id = users.size;
        u.key = rand()%10000; // just a 4 digit key
        u.addr = cliaddr;
        u.isReady = 1;
        u.name = (char*)malloc(NAMESIZE * sizeof(char));
        for(int i = 0; i < NAMESIZE; i++){
          *(u.name + i) = *(buffer + i + 2);
        }
        users.push_back(u);

        // we should tell them they have connected and their key or the thing is full

        // connect + key

        sendto(sockfd, (const char *)buffer, sizeof(char)*MAXLINE, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
      }else if(strcmp(cmd, "rq") == 0){
        // request 
        // for all users send to them a 4 * 8 = 32 char list of players
        // first compile the list 

        // check key first
        char*gotKey = buffer + 2;
        int numKey = atoi(gotKey);
        if(checkKey(&users, numKey, cliaddr)){
          setBuffer(&users,buffer);
          sendto(sockfd, (const char *)buffer, sizeof(char)*MAXLINE, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        }
      }else if(strcmp(cmd, "rd") == 0){
        // request 
        // for all users send to them a 4 * 8 = 32 char list of players
        // first compile the list 

        // check key first
        char*gotKey = buffer + 2;
        int numKey = atoi(gotKey);
        if(checkKey(&users, numKey, cliaddr)){
          setBuffer(&users,buffer);
          sendto(sockfd, (const char *)buffer, sizeof(char)*MAXLINE, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        }
      }
    }else{
      // client left
      printf("client left\n");
    }
    
  }


  auto next_game_tick = std::chrono::system_clock::now();

  long sleep_time = 0;

  bool game_is_running = true;
  // game loop starts here
  while(game_is_running){
    next_game_tick += SKIP_TICKS;
    sleep_time =  std::chrono::duration_cast<std::chrono::milliseconds>(next_game_tick - std::chrono::system_clock::now()).count();
    if( sleep_time >= 0 ) {
      usleep( sleep_time );
    }else {
      // we are running behind!
    }
  }

  // n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
  // buffer[n] = '\0'; 
  // printf("Client : %s\n", buffer); 
  // sendto(sockfd, (const char *)hello, sizeof(char)*64, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
  
  // printf("Hello message sent.\n");  
    
  return 0; 
}
