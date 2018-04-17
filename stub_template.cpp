{%- extends 'base_template.cpp' %}

{%- set purpose = 'stub' %}

{%- block functions %}
{%- for f, signature in funcs.iteritems() %}
void __{{ f }}({{ signature['arguments'] | map('render_param', types) | join(', ') }}) {
    string payload = "DONE";
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub.cpp: invoking {{ f }}()");
    {% if signature['return_type'] != 'void' %}
    {{ signature['return_type'] }} result;
    result = {{ f }}({{ signature['arguments'] | map(attribute='name') | join(', ') }});
    payload = result + " " + payload;
    {% else %}
    {{ f }}({{ signature['arguments'] | map(attribute='name') | join(', ') }});
    {% endif %}
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.stub.cpp: returned from {{ f }}() -- responding to client");
    RPCSTUBSOCKET->write(payload.c_str(), payload.length());
}
{%- endfor %}
{%- endblock %}

{%- block definitions %}
void dispatchFunction() {
    char functionNameBuffer[50];
    getFunctionNameFromStream(functionNameBuffer,sizeof(functionNameBuffer));
    if (!RPCSTUBSOCKET-> eof()) {
    {%- for f, signature in funcs.iteritems() %} 
        {%- if loop.first %}
        if (strcmp(functionNameBuffer, "{{ f }}") == 0) {
            __{{ f }}();
        {%- else %}
        } else if (strcmp(functionNameBuffer, "{{ f }}") == 0) {
            __{{ f }}();
        {%- endif %}
    {%- endfor %}
        } else {
            __badFunction(functionNameBuffer);
        }
    }
}

void getFunctionNameFromStream(char *buffer, unsigned int bufSize) {
    unsigned int i;
    char *bufp;
    bool readnull;
    ssize_t readlen;  

    readnull = false;
    bufp = buffer;
    for (i=0; i< bufSize; i++) {
        readlen = RPCSTUBSOCKET-> read(bufp, 1);  // read a byte
        // check for eof or error
        if (readlen == 0) {
            break;
        }
        // check for null and bump buffer pointer
        if (*bufp++ == ' ') {
            readnull = true;
            break;
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
{% endblock %}
