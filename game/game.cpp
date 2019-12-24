#include "game.hpp"

game::game(){
  myMap = new map(WIDTH, HEIGHT);
  
  plrs[0] = new player(3, HEIGHT - 4, 0);
  myMap->setControl(plrs[0]->getX(),plrs[0]->getY(), plrs[0]->getTeam());

  plrs[1] = new player(WIDTH - 4, HEIGHT - 4, 1);
  myMap->setControl(plrs[1]->getX(),plrs[1]->getY(), plrs[1]->getTeam());

  plrs[2] = new player(WIDTH - 4, 3, 2);  
  myMap->setControl(plrs[2]->getX(),plrs[2]->getY(), plrs[2]->getTeam());

  plrs[3] = new player(3, 3, 3);
  myMap->setControl(plrs[3]->getX(),plrs[3]->getY(), plrs[3]->getTeam());

  
}

void game::tick(){
  for (int i = 0; i < AMT_PLRS; i++){
    plrs[i]->tick();
  }
  for(int i = 0; i < AMT_PLRS; i++){
    for(int j = i; j < AMT_PLRS; j++){
      if(i != j){
        if(plrs[i]->getX() == plrs[j]->getX() && plrs[i]->getY() == plrs[j]->getY()){
          // if j and i are on the same square, kill both of them
          plrs[i]->kill();
          plrs[j]->kill();
        }
      }
    }
    int tileTeam = myMap->getController(plrs[i]->getX(), plrs[i]->getY());
    if(tileTeam != -1){ // if i steps on something that is not empty, kill it.
      plrs[i]->kill();
    }else{
      myMap->setControl(plrs[i]->getX(), plrs[i]->getY(), plrs[i]->getTeam());
    }
  }
}

// prereq: data must be 32 chars long
// stores the game into data
void game::storeGame(unsigned char * data){
  for(int i = 0; i < 32; i++){
    data[i] = 0;
  }
  
  for (int i = 0; i < AMT_PLRS; i++){
    
    // first lets use the first character to store user data
    data[i*8] = plrs[i]->getUserData();
    
    //printf("getUserData player: %d %d %d, %d %d\n", plrs[i]->getTeam(), plrs[i]->getDir(), plrs[i]->getDed() ,i, data[i*8]);
    // next lets store the 7 tiles
    for (int j = 0; j < 6; j++){
      
      unsigned char loc = plrs[i]->getPy(j);
      loc += plrs[i]->getPx(j)*16;
      data[i*8+j+1] = loc;
    }
    unsigned char loc = plrs[i]->getY();
    loc += plrs[i]->getX()*16;

    data[i*8+7] = loc;
    
  }
  
}

void game::loadGame(unsigned char * data){
  for (int i = 0; i < AMT_PLRS; i++){
    int x = data[i*8+7]/16;
    int y = data[i*8+7]-x*16;
    plrs[i]->loadUserData(data[i*8],x,y);
    myMap->setControl(x,y,plrs[i]->getTeam());
  }
  for (int i = 0; i < AMT_PLRS; i++){
    
    for (int j = 0; j < 6; j++){ // the first 4 bits contain the x coordinate, the last 4 contain the y coordinate
      int x = data[i*8+j+1]/16;
      int y = data[i*8+j+1]-x*16;
      myMap->setControl(x,y,plrs[i]->getTeam());
    }
    
  }
  
}

void game::setDirection(int player, int dir){
  plrs[player]->changeDir(dir);
}

int game::blockVal(int x, int y){
  return myMap->getController(x,y);
}

void game::resetBoard(){
  for (int i = 0; i < 16; i++){
    for (int j = 0; j < 16; j++){
      myMap->setControl(i,j,-1);
    }
    
  }
  
}

void game::printGame(){
  printf("_______________\n");
  for (int y = 0; y < MAP_HEIGHT; y++)
  {
    for (int x = 0; x < MAP_WIDTH; x++){
      bool hasplr = false;
      for (int i = 0; i < 4; i++){
        if(plrs[i]->getX() == x && plrs[i]->getY() == y){
          printf("x ");
          hasplr = true;
        }
      }
      if(!hasplr){
        if(myMap->getController(x,y) == -1){
          printf(". ");
        }else{
          printf("%d ",myMap->getController(x,y));
        }
      }
      
    }
    printf("\n");
    
  }
  printf("_______________\n");
  
}

game::~game(){
  delete myMap;
  for(int i = 0; i < 4; i++){
    delete plrs[i];
  }
}
