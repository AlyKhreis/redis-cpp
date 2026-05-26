#include "command.h"
#include <sstream> // for stringstream ss
#include <algorithm>   // for transform

command *parse_command(string s1) {
    command *co = new command;
    std::stringstream ss(s1);
    string s2;
    ss >> s2;
    transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
    if (s2 == "SET") co->type = SET;
    else if (s2 == "DEL") co->type = DEL;
    else if (s2 == "GET") co->type = GET;
    else if (s2 == "EXISTS") co->type = EXISTS;
    else if (s2 == "KEYS" ) co-> type = KEYS;
    else if (s2 == "FLUSHALL" ) co-> type = FLUSHALL;
    else if (s2 == "APPEND") co->type = APPEND;
    else if (s2 == "INCR") co->type = INCR;
    else co->type = UNKNOWN;
    ss >> co->key;
    ss >> co->value;
    return co;
}