#ifndef REDIS_CPP_AOF_H
#define REDIS_CPP_AOF_H
#include "hash_table.h"
#include "command.h"
#include <string>
#include <iostream>
#include <fstream>
using std::string;

void aof_write(string command);
void aof_load(HashTable *ht);




#endif
