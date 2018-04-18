{%- extends 'base_template.cpp' %}

{%- set agent = 'stub' %}


{%- block declarations %}

{%- for t, _ in return_types.iteritems() | reject('builtin') %}
string serialize_{{ t | escape_declaration }}({{ {'name': 'x', 'type': t} | render_param(types) }});
{%- endfor %}

{%- for t, _ in types.iteritems() | reject('builtin') %}
{{ t | render_return_type(types) }}deserialize_{{ t | escape_declaration }}(string s);
{%- endfor %}

{%- endblock %}


{%- block functions %}
{%- for f, signature in funcs.iteritems() %}
void __{{ f }}({{ signature['arguments'] | map('render_param', types) | join(', ') }}) {
    string payload = "DONE";
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub.cpp: invoking {{ f }}()");
    {% if signature['return_type'] != 'void' %}
    {{ signature['return_type'] }} result;
    result = {{ f }}({{ signature['arguments'] | map(attribute='name') | join(', ') }});
    payload = serialize_{{ signature['return_type'] | escape_declaration }}(result) + " " + payload;
    {% else %}
    {{ f }}({{ signature['arguments'] | map(attribute='name') | join(', ') }});
    {% endif %}
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub.cpp: returned from {{ f }}() -- responding to client");
    RPCSTUBSOCKET->write(payload.c_str(), payload.length());
}
{%- endfor %}
{%- endblock %}


{%- block definitions %}
void __badFunction(char *functionName) {
    char doneBuffer[5] = "BAD";  // to write magic value DONE + null
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub.cpp: received call for nonexistent function %s()",functionName);
    RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}

void dispatchFunction() {
    char functionNameBuffer[50];
    getFunctionNameFromStream(functionNameBuffer, sizeof(functionNameBuffer));

    {%- for f, signature in funcs.iteritems() %} 
    {%- if loop.first %}
    if (strcmp(functionNameBuffer, "{{ f }}") == 0) {
    {%- else %}
    } else if (strcmp(functionNameBuffer, "{{ f }}") == 0) {
    {%- endif %}
        {%- for arg in signature['arguments'] %}
        {{ arg | render_param(types) }} = deserialize_{{ signature['return_type'] | escape_declaration }}(get_param({{ loop.index0 }}));
        {%- endfor %}
        __{{ f }}({{ signature['arguments'] | map(attribute='name') | join(', ') }});
    {%- endfor %}
    } else {
        __badFunction(functionNameBuffer);
    }
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
        if (*bufp++ == '\0') {
            readnull = true;
            break;
        }
    }
    if (readlen == 0) {
        c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub: read zero length message, checking EOF");
        if (RPCSTUBSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub: EOF signaled on input");
        } else {
            throw C150Exception("{{filename }}.stub: unexpected zero length read without eof");
        }
    } else if (!readnull) {
        throw C150Exception("{{ filename }}.stub: method name not null terminated or too long");
    }
}
{% endblock %}
