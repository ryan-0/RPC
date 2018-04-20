#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include "rpcstubhelper.h"
#include "simplefunction.idl"

using namespace C150NETWORK;

string get_param(int i, char *buffer, unsigned int bufSize, string input) {
    unsigned int count;
    string::iterator it;
    char opener, closer, *bufp;
    int j;

    j = 0;
    count = 0;
    bufp = buffer;

    for (it = input.begin(); it != input.end(); it++) {
        *bufp = *it;
        if (count) {
            if (*bufp == closer && count > 1) {
                count--;
            } else if (*bufp == closer) {
                // overwrite last character to NULL-terminate the string
                *bufp = '\0';
                c150debug->printf(C150RPCDEBUG,"Finished param %d: %s", j, buffer);
                count = 0;
                bufp = buffer;
                if (j == i) {
                    return string(buffer);
                }
                j++;
                continue;
            } else if (*bufp == opener) {
                    count++;
                }
            bufp++;
        } else {
            count = 1;
            switch (*bufp) {
                case '{':
                    opener = '{';
                    closer = '}';
                    break;
                case '[':
                    opener = '[';
                    closer = ']';
                    break;
                case '(':
                    opener = '(';
                    closer = ')';
                    break;
                case '\037':
                    opener = '\037';
                    closer = '\037';
                    break;
                default:
                    count = 0;
            }
        }
    }
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub: reached end of string");
    c150debug->printf(C150RPCDEBUG,"THE STRING BUFFER IS %s", buffer);
    throw C150Exception("simplefunction.stub: unmatched brackets detected");
    return "you failed";
}

string serialize_float(float n) {
    return "(" + to_string(n) + ")";
}

string serialize_int(int n) {
    return "(" + to_string(n) + ")";
}

string serialize_string(string s) {
    return '\037' + s + '\037';
}

float deserialize_float(string s) {
    return stof(s, NULL);
}

float deserialize_int(string s) {
    return stoi(s, NULL);
}

string deserialize_string(string s) {
    return s;
}

void deserialize___int_2__2_(string s, int arr[2][2]);


void deserialize___int_2_(string s, int arr[2]);



void __func3() {
    string payload = "DONE";
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: invoking func3()");
    
    func3();
    
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: returned from func3() -- responding to client");
    RPCSTUBSOCKET->write(payload.c_str(), payload.length());
}
void __func2(int x, int y) {
    string payload = "DONE";
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: invoking func2()");
    
    string result;
    result = func2(x, y);
    payload = serialize_string(result) + " " + payload;
    
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: returned from func2() -- responding to client");
    RPCSTUBSOCKET->write(payload.c_str(), payload.length());
}
void __func1(string x, string y) {
    string payload = "DONE";
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: invoking func1()");
    
    string result;
    result = func1(x, y);
    payload = serialize_string(result) + " " + payload;
    
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: returned from func1() -- responding to client");
    RPCSTUBSOCKET->write(payload.c_str(), payload.length());
}
void __takesTwoArrays(int x[2], int y[2][2]) {
    string payload = "DONE";
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: invoking takesTwoArrays()");
    
    int result;
    result = takesTwoArrays(x, y);
    payload = serialize_int(result) + " " + payload;
    
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: returned from takesTwoArrays() -- responding to client");
    RPCSTUBSOCKET->write(payload.c_str(), payload.length());
}


void __badFunction(char *functionName) {
    char doneBuffer[5] = "BAD";  // to write magic value DONE + null
    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: received call for nonexistent function %s()",functionName);
    RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}

void getFunctionNameFromStream(char *buffer, unsigned int bufSize) {
    unsigned int i;
    char *bufp;
    bool readnull;
    ssize_t readlen;  

    readnull = false;
    bufp = buffer;
    for (i = 0; i < bufSize; i++) {
        readlen = RPCSTUBSOCKET-> read(bufp, 1);  // read a byte
        // check for EOF or error
        if (readlen == 0) {
            break;
        }

        // check for null and bump buffer pointer
        if (*bufp == '\0' || *bufp == ' ') {
            readnull = true;
            *bufp = '\0';
            break;
        }
        bufp++;

    }
    if (readlen == 0) {
        c150debug->printf(C150RPCDEBUG,"simplefunction.stub: read zero length message, checking EOF");
        if (RPCSTUBSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"simplefunction.stub: EOF signaled on input");
            buffer[0] = '\0';
        } else {
            throw C150Exception("simplefunction.stub: unexpected zero length read without eof");
        }
    } else if (!readnull) {
        throw C150Exception("simplefunction.stub: method name not null terminated or too long");
    }
}

void dispatchFunction() {
    char functionNameBuffer[50];
    char argsBuffer[1060];

    getFunctionNameFromStream(functionNameBuffer, sizeof(functionNameBuffer));
    if (functionNameBuffer[0] == '\0') {
        return;
    }
    RPCSTUBSOCKET->read(argsBuffer,sizeof(argsBuffer));

    c150debug->printf(C150RPCDEBUG,"simplefunction.stub.cpp: read in |%s|", argsBuffer);
    string input(argsBuffer);
    memset(argsBuffer, 0, sizeof(argsBuffer));
    c150debug->printf(C150RPCDEBUG,"arithmetic.stub.cpp: THE FUNCTION NAME IS  %s()",functionNameBuffer);
    //need to fix this extra space bug (its fixed, but space should be removed in the first place)
    if (strcmp(functionNameBuffer, "func3") == 0) {
        try {
            __func3();
        } catch (...) { 
            c150debug->printf(C150RPCDEBUG,"arithmetic.stub.cpp: ITS ACTUALLY HERE LOLS");
            __badFunction(functionNameBuffer);
        }
    } else if (strcmp(functionNameBuffer, "func2") == 0) {
        try {
            int x = deserialize_int(get_param(0, argsBuffer, sizeof(argsBuffer), input));
            int y = deserialize_int(get_param(1, argsBuffer, sizeof(argsBuffer), input));
            __func2(x, y);
        } catch (...) { 
            c150debug->printf(C150RPCDEBUG,"arithmetic.stub.cpp: ITS ACTUALLY HERE LOLS");
            __badFunction(functionNameBuffer);
        }
    } else if (strcmp(functionNameBuffer, "func1") == 0) {
        try {
            string x = deserialize_string(get_param(0, argsBuffer, sizeof(argsBuffer), input));
            string y = deserialize_string(get_param(1, argsBuffer, sizeof(argsBuffer), input));
            __func1(x, y);
        } catch (...) { 
            c150debug->printf(C150RPCDEBUG,"arithmetic.stub.cpp: ITS ACTUALLY HERE LOLS");
            __badFunction(functionNameBuffer);
        }
    } else if (strcmp(functionNameBuffer, "takesTwoArrays") == 0) {
        try {
            int x[2];
            deserialize___int_2_(get_param(0, argsBuffer, sizeof(argsBuffer), input), x);
            int y[2][2];
            deserialize___int_2__2_(get_param(1, argsBuffer, sizeof(argsBuffer), input), y);
            __takesTwoArrays(x, y);
        } catch (...) { 
            c150debug->printf(C150RPCDEBUG,"arithmetic.stub.cpp: ITS ACTUALLY HERE LOLS");
            __badFunction(functionNameBuffer);
        }
    } else {
        __badFunction(functionNameBuffer);
    }
}



void deserialize___int_2__2_(string s, int arr[2][2]) {
    char buffer[4096];
    for (int i = 0; i < 2; i++) {
        deserialize___int_2_(get_param(i, buffer, sizeof(buffer), s), arr[i]);
    }
}
void deserialize___int_2_(string s, int arr[2]) {
    char buffer[4096];
    for (int i = 0; i < 2; i++) {
        arr[i] = deserialize_int(get_param(i, buffer, sizeof(buffer), s));
    }
}