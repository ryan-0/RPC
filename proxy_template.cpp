{%- extends 'base_template.cpp' %}

{%- set agent = 'proxy' %}
{%- set serializers = return_types %}
{%- set deserializers = types %}

{% block functions %}
{% for f, signature in funcs.iteritems() %}
{{ signature['return_type'] }} {{ f }}({{ signature['arguments'] | map('render_param', types) | join(', ') }}) {
    char readBuffer[1024];
    string payload = "{{ f }} ";
    {%- for arg in signature['arguments'] %}
    payload += serialize_{{ arg['type'] | escape_declaration }}({{ arg['name'] }});
    {%- if not loop.last %}
    //this is where the extra space bug is coming from.x

    {%- endif %}
    {%- endfor %}

    c150debug->printf(C150RPCDEBUG,"{{ filename }}.proxy.cpp: {{ f }} invoked");
    RPCPROXYSOCKET->write(payload.c_str(), payload.length() + 1);
    c150debug->printf(C150RPCDEBUG,"{{ filename }}.proxy.cpp: {{ f }}() invocation sent, waiting for response");
    RPCPROXYSOCKET->read(readBuffer, sizeof(readBuffer));

    // TODO: check for real error case
    if (0) {
        throw C150Exception("{{ filename }}.proxy.cpp: {{ f }}() received invalid response from the server");
    }
    {%- if signature['return_type'] != 'void' %}
    string response(readBuffer);
    char buffer[4096];
    return deserialize_{{ signature['return_type'] | escape_declaration }}(get_param(0, buffer, sizeof(buffer), response));
    {%- endif %}
}
{% endfor %}
{% endblock %}
