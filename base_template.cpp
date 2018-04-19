#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include "rpc{{ agent }}helper.h"
#include "{{ filename }}.idl"

using namespace C150NETWORK;

string get_param(int i, char *buffer, unsigned int bufSize, string input) {
    unsigned int count;
    string::iterator it;
    char opener, closer, *bufp;

    for (int j = 0; j <= i; j++) {
        count = 0;
        bufp = buffer;
        for (it = input.begin(); it != input.end(); it++) {
            // TODO: check if at end of buffer
            *bufp = *it;
            if (count) {
                bufp++;
                if (*bufp == opener) {
                    count++;
                } else if (*bufp == closer && count > 1) {
                    count--;
                } else if (*bufp == closer) {
                    // overwrite last character to NULL-terminate the string
                    *bufp = '\0';
                    break;
                }
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
                    default:
                        count = 0;
                }
            }
        }
        c150debug->printf(C150RPCDEBUG,"{{ filename }}.{{ agent }}: reached end of string");
        c150debug->printf(C150RPCDEBUG,"THE STRING BUFFER IS %s",string(buffer));
        throw C150Exception("{{ filename }}.{{ agent }}: unmatched brackets detected");
        
    } 
    return string(buffer);
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
