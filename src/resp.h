#ifndef REDIS_CPP_RESP_H
#define REDIS_CPP_RESP_H
#include <string>
#include <vector>
#include "command.h"
#include <sstream>
using std::vector;
using std::string;


string resp_bulk_string(string key);
string resp_simple_string(string s);
string resp_error(string msg);
string resp_integer(int n);
string resp_null();
vector<string> parse_resp(string input);
command *parse_command_from_resp(vector<string> tokens);
string resp_array(vector<string> items);

#endif
