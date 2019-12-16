#include "game.hpp"

game::game(){
  myMap = new map(WIDTH, HEIGHT);

  
  
  if(AMT_PLRS > 0){
    plrs[0] = new player(3, HEIGHT - 4, 0);
  }
  if(AMT_PLRS > 1){
    plrs[1] = new player(3, 3, 1);
  }
  if(AMT_PLRS > 2){
    plrs[2] = new player(WIDTH - 4, 3, 2);  
  }
  if(AMT_PLRS > 3){
    plrs[3] = new player(WIDTH - 4, HEIGHT - 4, 3);
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

void game::storeNumber(int n, char * c){
  
}

// prereq: data must be 32 chars long
void game::storeGame(char * data){
  for(int i = 0; i < 32; i++){
    data[i] = 0;
  }
  for (int i = 0; i < AMT_PLRS; i++){
    // first lets use the first character to store user data
    data[i*8] = plrs[i]->getUserData();

    // next lets store the 7 tiles
    for (int j = 0; j < 6; j++){
      char loc = plrs[i]->getPy(j);
      loc += plrs[i]->getPx(j)*2^4;
      data[i*8+j+1] = loc;
    }
    char loc = plrs[i]->getY();
    loc += plrs[i]->getX()*2^4;

    data[i*8+7] = loc;
    
  }
  
}

void game::loadGame(char * data){
  for (int i = 0; i < AMT_PLRS; i++){
    int x = data[i*8+7]/2^4;
    int y = data[i*8+7]%2^4;
    plrs[i]->loadUserData(data[i*8],x,y);
  }
  for (int i = 0; i < AMT_PLRS; i++){
    for (int j = 0; j < 7; j++){
      int x = data[i*8+j+1]/2^4;
      int y = data[i*8+j+1]%2^4;
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

