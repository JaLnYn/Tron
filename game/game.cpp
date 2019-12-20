#include "game.hpp"

game::game(){
  myMap = new map(WIDTH, HEIGHT);

  
  
  if(AMT_PLRS > 0){
    plrs[0] = new player(3, HEIGHT - 4, 0);
    player*plr = plrs[0];
    myMap->setControl(plr->getX(),plr->getY(), plr->getTeam());
  }
  if(AMT_PLRS > 1){
    plrs[1] = new player(3, 3, 1);
    player*plr = plrs[1];
    myMap->setControl(plr->getX(),plr->getY(), plr->getTeam());
  }
  if(AMT_PLRS > 2){
    plrs[2] = new player(WIDTH - 4, 3, 2);  
    player*plr = plrs[2];
    myMap->setControl(plr->getX(),plr->getY(), plr->getTeam());
  }
  if(AMT_PLRS > 3){
    plrs[3] = new player(WIDTH - 4, HEIGHT - 4, 3);
    player*plr = plrs[3];
    myMap->setControl(plr->getX(),plr->getY(), plr->getTeam());
  }
  
}

void game::tick(){
  for (int i = 0; i < AMT_PLRS; i++){
    plrs[i]->tick();
  }
  for(int i = 0; i < AMT_PLRS; i++){
    for(int j = 0; j < AMT_PLRS; j++){
      if(plrs[i]->getX() == plrs[j]->getX() && plrs[i]->getY() == plrs[j]->getY()){
        // if j and i are on the same square, kill both of them
        plrs[i]->kill();
        plrs[j]->kill();
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
    printf("getUserData player: %d %d %d, %d %d\n", plrs[i]->getTeam(), plrs[i]->getDir(), plrs[i]->getDed() ,i, data[i*8]);
    // next lets store the 7 tiles
    for (int j = 0; j < 6; j++){
      unsigned char loc = plrs[i]->getPy(j);
      loc += plrs[i]->getPx(j)*16;
      data[i*8+j+1] = loc;
    }
    unsigned char loc = plrs[i]->getY();
    loc += plrs[i]->getX()*16;

    data[i*8+7] = loc;
    printf("getUserData %d %d\n", i, data[i*8]);
  }
}

void game::loadGame(unsigned char * data){
  
  for (int i = 0; i < AMT_PLRS; i++){
    int x = data[i*8+7]/16;
    int y = data[i*8+7]-x;
    plrs[i]->loadUserData(data[i*8],x,y);
  }
  for (int i = 0; i < AMT_PLRS; i++){
    for (int j = 0; j < 7; j++){
      int x = data[i*8+j+1]/16;
      int y = data[i*8+j+1]-x;
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

game::~game(){
  
}
