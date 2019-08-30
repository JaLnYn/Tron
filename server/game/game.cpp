#include "game.hpp"

game::game(){
  myMap = new map(WIDTH, HEIGHT);

  plrs[1] = new player(4, 4, 1);
  plrs[2] = new player(WIDTH - 4, 4, 2);
  plrs[3] = new player(WIDTH - 4, HEIGHT - 4, 3);
  plrs[0] = new player(4, HEIGHT - 4, 0);
  
}

void game::tick(){
  for (int i = 0; i < 4; i++){
    plrs[i]->tick();
  }
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
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
  if(n >= 100 || n < 0){
    return;
  }
  *c = n/10 + '0';
  *(c + 1) = n%10 + '0';
}

void game::storeGame(char * data){
  int currentIndex = 0;
  for (int i = 0; i < AMT_PLRS; i++){
    // for each player store it's x and y. also store the previous steps
    storeNumber(plrs[i]->getX(), data+currentIndex);
    currentIndex += 2;
    storeNumber(plrs[i]->getY(), data+currentIndex);
    currentIndex += 2;
    for (int j = 0; j < 3; j++){
      storeNumber(plrs[i]->getPx(2-j), data+currentIndex);
      currentIndex += 2;
      storeNumber(plrs[i]->getPy(2-j), data+currentIndex);
      currentIndex += 2;
    }

  }
}

void game::setDirection(int player, int dir){
  plrs[player]->changeDir(dir);
}

