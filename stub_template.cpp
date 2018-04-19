{%- extends 'base_template.cpp' %}

{%- set agent = 'stub' %}
{%- set serializers = return_types %}
{%- set deserializers = types %}


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

void getFunctionNameFromStream(char *buffer, unsigned int bufSize) {
    unsigned int i;
    char *bufp;
    bool readnull;
    ssize_t readlen;  

    readnull = false;
    bufp = buffer;
    for (i = 0; i < bufSize; i++) {
        readlen = RPCSTUBSOCKET-> read(bufp, 1);  // read a byte


        //added this to fix buffer bug
        if(*bufp == ' '){
            readnull = true;
            break;
        }
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

void dispatchFunction() {
    char functionNameBuffer[50];
    char argsBuffer[1060];

    getFunctionNameFromStream(functionNameBuffer, sizeof(functionNameBuffer));
    RPCSTUBSOCKET->read(argsBuffer,sizeof(argsBuffer));
    string input(argsBuffer);
    memset(argsBuffer, 0, sizeof(argsBuffer));
    c150debug->printf(C150RPCDEBUG,"arithmetic.stub.cpp: THE FUNCTION NAME IS  %s()",functionNameBuffer);
    {%- for f, signature in funcs.iteritems() %} 
    {%- if loop.first %}
    //need to fix this extra space bug (its fixed, but space should be removed in the first place)
    if (strcmp(functionNameBuffer, "{{ f }} ") == 0) {
    {%- else %}
    } else if (strcmp(functionNameBuffer, "{{ f}} ") == 0) {
    {%- endif %}
        {%- for arg in signature['arguments'] %}
        {{ arg | render_param(types) }} = deserialize_{{ signature['return_type'] | escape_declaration }}(get_param({{ loop.index0 }}, argsBuffer, sizeof(argsBuffer), input));
        {%- endfor %}
        __{{ f }}({{ signature['arguments'] | map(attribute='name') | join(', ') }});
    {%- endfor %}
    } else {
        __badFunction(functionNameBuffer);
    }
}
{% endblock %}
