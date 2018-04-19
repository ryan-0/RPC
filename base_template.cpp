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
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.{{ agent }}: reached end of string");
    c150debug->printf(C150RPCDEBUG,"THE STRING BUFFER IS %s", buffer);
    throw C150Exception("{{ filename }}.{{ agent }}: unmatched brackets detected");
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

{%- for t, _ in serializers.iteritems() | reject('builtin') %}
string serialize_{{ t | escape_declaration }}({{ {'name': 'x', 'type': t} | render_param(types) }});
{%- endfor %}

{%- for t, _ in deserializers.iteritems() | reject('builtin') %}
{{ t | render_return_type(types) }}deserialize_{{ t | escape_declaration }}(string s);
{%- endfor %}

{% block functions %}
{% endblock %}

{% block definitions %}
{% endblock %}

{% for name, definition in serializers.iteritems() | reject('builtin' )%}
string serialize_{{ name | escape_declaration }}({{ {'name': 'x', 'type': name} | render_param(types) }}) {
    string s = "";
    {%- if definition['type_of_type'] == 'array' %}
    s += "[";
    for (int i = 0; i < {{ definition['element_count'] }}; i++) {
        s += serialize_{{ definition['member_type'] | escape_declaration }}(x[i]);
        if (i < {{ definition['element_count'] | int - 1}}) {
            s += ",";
        }
    }
    s += "[";
    {%- else %}
    s += "{";
    {%- for mem in definition['members'] %}
    s += serialize_{{ mem['type'] | escape_declaration }}(x.{{ mem['name'] }});
    {%- if not loop.last %}
    s += ",";
    {%- endif %}
    {%- endfor %}
    s += "}";
    {%- endif %}
    return s;
}
{% endfor %}

{%- for t, definition in deserializers.iteritems() | reject('builtin') %}
{{ t | render_return_type(types) }}deserialize_{{ t | escape_declaration }}(string s) {
    {%- if definition['type_of_type'] == 'array' %}
        {# TODO: handle case of  deserializing arbitrary array #}
    {%- else %}
    {{ t }} x;
    {%- for mem in definition['members'] %}
    x.{{ mem['name'] }} = deserialize_{{ mem['type'] | escape_declaration }}(s);
    {%- endfor %}
    return x;
    {%- endif %}
}
{%- endfor %}
