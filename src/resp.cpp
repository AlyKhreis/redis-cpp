#include "resp.h"
#include <algorithm>

string resp_bulk_string(string key) {
    string resp_bulk ="$" + std::to_string(key.size()) + "\r\n"+ key+ "\r\n";
    return resp_bulk;
}
string resp_simple_string(string s) {
    return "+" + s + "\r\n";
}
string resp_error(string msg) {
    return "-ERR "+ msg+ "\r\n";
}
string resp_integer(int n) {
    return  ":"+std::to_string(n)+"\r\n";
}
string resp_null() {
    return "$-1\r\n";
}
vector <string> parse_resp(string input) {
    vector <string> tokens;
    std::stringstream ss(input);
    string line;
    while (getline(ss, line, '\n')) {
        // remove \r if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        // skip empty lines, lines starting with * or $
        if (line.empty() || line[0] == '*' || line[0] == '$') {
            continue;
        }
        // otherwise it's a real token
        tokens.push_back(line);
    }
    return tokens;
}

command *parse_command_from_resp(vector <string> tokens) {
    command *co = new command;
    if (!tokens.empty()) {
        transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::toupper);
    }
    if (tokens[0] == "SET") co->type = SET;
    else if (tokens[0] == "DEL") co->type = DEL;
    else if (tokens[0] == "GET") co->type = GET;
    else if (tokens[0] == "EXISTS") co->type = EXISTS;
    else if (tokens[0] == "KEYS" ) co-> type = KEYS;
    else if (tokens[0] == "FLUSHALL" ) co-> type = FLUSHALL;
    else if (tokens[0] == "APPEND") co->type = APPEND;
    else if (tokens[0] == "INCR") co->type = INCR;
    else co->type = UNKNOWN;
    if (tokens.size() > 1) co->key = tokens[1];
    if (tokens.size() > 2) co->value = tokens[2];

    return co;
}


string resp_array(vector<string> items) {
    string result = "*" + std::to_string(items.size()) + "\r\n";
    for (auto& item : items) {
        result += resp_bulk_string(item);
    }
    return result;
}