#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include "rpc{{ purpose }}helper.h"
#include "{{ filename }}.idl"

using namespace C150NETWORK;

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
