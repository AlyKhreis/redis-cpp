#ifndef REDIS_CPP_COMMAND_H
#define REDIS_CPP_COMMAND_H
#include <string>
using std::string;

enum commandType {
    SET,
    GET,
    DEL,
    EXISTS,
    KEYS,
    FLUSHALL,
    APPEND,
    INCR,
    EXPIRE,
    TTL,
    UNKNOWN
} ;

struct command {
    commandType type;
    string key;
    string value;
};
command *parse_command(string s1);




#endif //REDIS_CPP_COMMAND_H
