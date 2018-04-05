// --------------------------------------------------------------
//
//                        simplefunction.proxy.cpp
//
//        Author: Noah Mendelsohn         
//   
//       This is a hand-crafted demonstration proxy.
//
//       For your project, your "rpcgen" program will generate
//       proxies like this one automatically from the idl
//       file. Note that this proxy also #includes the 
//       simplefunctions.idl file. Of course, your rpcgen
//       program will change that, as well as the number
//       of functions generated. More importantly, it will
//       generate code to handle function arguments and
//       return values.
//
//
//
//       Copyright: 2012 Noah Mendelsohn
//     
// --------------------------------------------------------------

// IMPORTANT! WE INCLUDE THE IDL FILE AS IT DEFINES THE INTERFACES
// TO THE FUNCTIONS WE'RE IMPLEMENTING. THIS MAKES SURE THE
// CODE HERE ACTUALLY MATCHES THE REMOTED INTERFACE

#include "arithmetic.idl"

#include "rpcproxyhelper.h"

#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <arpa/inet.h>
#include <climits>


using namespace C150NETWORK;  // for all the comp150 utilities 

const size_t sentinel_length = strlen("DONE") + 1; 

int add(int x, int y) {
  size_t bufferSize  = INT_MIN.str().size() + sentinel_length;
  char returnValBuf[sentinel_length - 5];
  char readBuffer[bufferSize];  // to read magic value DONE + null
  string arg1 = x.str() + " ";
  string arg2 = y.str() + " ";

  //
  // Send the Remote Call
  //
  c150debug->printf(C150RPCDEBUG,"arithmetic.proxy.cpp: add() invoked");
  RPCPROXYSOCKET->write("add ", sizeof("add ")); // write function name including null
  RPCPROXYSOCKET->write("x ", sizeof("x "));
  RPCPROXYSOCKET->write(arg1.c_str(), sizeof(arg1.c_str()));
  RPCPROXYSOCKET->write("y ", sizeof("y "));
  RPCPROXYSOCKET->write(arg2.c_str(), sizeof(arg2.c_str()));
  // Read the response
  //
  //c150debug->printf(C150RPCDEBUG,"arithmetic.proxy.cpp: add() invocation sent, waiting for response");
  RPCPROXYSOCKET->read(readBuffer, sizeof(readBuffer)); // only legal response is DONE
  memcpy(returnValBuf, readBuffer, sizeof(readBuffer) - sentinel_length);
  cout<<(atoi(returnValBuf))<<endl;
  //
  // Check the response
  //
  // if (strncmp(readBuffer,"DONE", sizeof(readBuffer))!=0) {
  //  throw C150Exception("simplefunction.proxy: func1() received invalid response from the server");
  //}
 // c150debug->printf(C150RPCDEBUG,"simplefunction.proxy.cpp: func1() successful return from remote call");

}

