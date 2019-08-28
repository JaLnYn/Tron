
#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
// this is the class of objects used to represent objects on the map
class object{
protected:
  int id;
  int x, y; // the x and y coordinate for this object
  int capacity; // the maximum amount of units that can be on this class
  int ctrl_by; // the faction that controlls this object. -1 if not controlled
  int amt_units;

public:
  object(int x, int y);
  
  // getters and setters
  int getCapacity();
  int getControlled();
  int getAmtUnits();
  ~object();
};
#endif


