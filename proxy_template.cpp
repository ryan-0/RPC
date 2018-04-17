{%- extends 'base_template.cpp' %}

{%- set agent = 'proxy' %}

{% block declarations %}
{% for t, _ in types.iteritems() | reject('builtin') %}
string serialize_{{ t | escape_declaration }}({{ {'name': 'x', 'type': t} | render_param(types) }});
{%- endfor %}
{%- for t, _ in return_types.iteritems() | reject('builtin') %}
{{ t }} deserialize_{{ t | escape_declaration }}(string s);
{%- endfor %}
{% endblock %}

{% block functions %}
{% for f, signature in funcs.iteritems() %}
{{ signature['return_type'] }} {{ f }}({{ signature['arguments'] | map('render_param', types) | join(', ') }}) {
    char readBuffer[1024];
    string payload = "{{ f }} ";
    {%- for arg in signature['arguments'] %}
    payload += serialize_{{ arg['type'] | escape_declaration }}({{ arg['name'] }});
    {%- if not loop.last %}
    payload += " ";
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

    return deserialize_{{ signature['return_type'] | escape_declaration }}(readBuffer);
    {%- endif %}
}
{% endfor %}
{% endblock %}

{% block definitions %}
{% for name, definition in types.iteritems() | reject('builtin')  %}
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
{%- for t, definition in return_types.iteritems() | reject('builtin') %}
{{ t }} deserialize_{{ t | escape_declaration }}(string s) {
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
{% endblock %}
