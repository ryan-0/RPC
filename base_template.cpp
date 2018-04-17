#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include "rpc{{ agent }}helper.h"
#include "{{ filename }}.idl"

using namespace C150NETWORK;

void get_next_param(char *buffer, unsigned int bufSize) {
    unsigned int i, count = 0;
    char opener, closer, *bufp;
    ssize_t readlen;  

    // readnull = false;
    bufp = buffer;

    for (i = 0; i < bufSize; i++) {
        readlen = RPCSTUBSOCKET->read(bufp, 1);
        if (!count) {
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
                default:
                    count = 0;
            }
        } else if (*bufp == opener) {
            count++;
        } else if (*bufp == closer) {
        }
    } 

    if (readlen == 0) {
        c150debug->printf(C150RPCDEBUG,"simplefunction.stub: read zero length message, checking EOF");
        if (RPCSTUBSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"simplefunction.stub: EOF signaled on input");
        } else {
            throw C150Exception("simplefunction.stub: unexpected zero length read without eof");
        }
    } else if (!readnull) {
        throw C150Exception("simplefunction.stub: method name not null terminated or too long");
    }
}

string serialize_float(float n) {
    return to_string(n);
}

string serialize_int(int n) {
    return to_string(n);
}

string serialize_string(string s) {
    return s;
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

{% block declarations %}
{% endblock %}

{% block functions %}
{% endblock %}

{% block definitions %}
{% endblock %}
