// --------------------------------------------------------------
//
//                        simplefunction.cpp
//
//        Author: Noah Mendelsohn         
//   
//
//        Trivial implementations of the routines declared
//        in simplefunction.idl. These are for testing: they
//        just print messages.
//
//       Copyright: 2012 Noah Mendelsohn
//     
// --------------------------------------------------------------

// IMPORTANT! WE INCLUDE THE IDL FILE AS IT DEFINES THE INTERFACES
// TO THE FUNCTIONS WE'RE IMPLEMENTING. THIS MAKES SURE THE
// CODE HERE ACTUALLY MATCHES THE REMOTED INTERFACE

#include <string>
using namespace std;

#include "simplefunction.idl"

#include <cstdio>
#include "c150debug.h"

using namespace C150NETWORK;  // for all the comp150 utilities 


string func1(string x, string y) {
  printf("func1() invoked\n");
  string retval = x + y;
  return retval;
  c150debug->printf(C150RPCDEBUG,"simplefunction.cpp: func1() invoked");
}

string func2(int x, int z) {
  string retval = to_string(z + x);
  printf("func2() invoked\n");				  
  c150debug->printf(C150RPCDEBUG,"simplefunction.cpp: func2() invoked");
  return retval;
}

void func3() {
  printf("func3() invoked\n");
  c150debug->printf(C150RPCDEBUG,"simplefunction.cpp: func3() invoked");
}
int takesTwoArrays(rectangle s[2], int y){
  int retval = s[0].x + s[1].y + y;
  c150debug->printf(C150RPCDEBUG,"simplefunction.cpp: takesTWoArrays() invoked");
  return retval;
}

int takesTwoShapes(shape shapes, rectangle square){
  int retval = shapes.o.x + square.x; //should return 2 + 10
  retval += shapes.c.pi[0]; //+3
  retval += shapes.c.pi[4]; //+5
  c150debug->printf(C150RPCDEBUG,"simplefunction.cpp: takesTWoArrays() invoked");
  return retval;
}

