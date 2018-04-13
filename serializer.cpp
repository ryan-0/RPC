/*
 * file: serialize.cpp
 * authors: Russell Parker, Ryan Osgood
 */

#include <string>

using namespace std;

/*
 * serialize: overload template function for float base case
 */
string serialize(float n) {
    return to_string(n);
}

/*
 * deserialize_float: parse a string representation of a float
 */
float deserialize_float(string s) {
    return stof(s, NULL);
}

/*
 * serialize: overload template function for string base case
 */
string serialize(string s) {
    // TODO: escape characters, wrap in quotes
    return s;
}

/*
 * serialize: overload template function for int base case
 */
string serialize(int n) {
    return to_string(n);
}

/*
 * deserialize_int: parse a string representation of an int
 */
float deserialize_int(string s) {
    return stoi(s, NULL);
}

/*
 * serialize: 
 */
string serialize(float arr[], size_t size) {
    string s = "[";

    // add commas between vals, but not for last one
    for (int i = 0; i < size - 1; i++) {
        s.append(serialize(arr[i]));
        s.append(" ");
    }
    s.append(serialize(arr[size - 1]));
    s.append("]");
    return s;
}

/*
 * serialize: 
 */
string serialize(int arr[], size_t size) {
    string s = "[";

    // add commas between vals, but not for last one
    for (int i = 0; i < size - 1; i++) {
        s.append(serialize(arr[i]));
        s.append(" ");
    }
    s.append(serialize(arr[size - 1]));
    s.append("]");
    return s;
}

/*
 * serialize: 
 */
string serialize(string arr[], size_t size) {
    string s = "[";

    // add commas between vals, but not for last one
    for (int i = 0; i < size - 1; i++) {
        s.append(serialize(arr[i]));
        s.append(" ");
    }
    s.append(serialize(arr[size - 1]));
    s.append("]");
    return s;
}
