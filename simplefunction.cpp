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

