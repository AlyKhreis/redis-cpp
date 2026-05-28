#include "aof.h"



void aof_write(string command) {
    std::ofstream file("aof.txt", std::ios::app);
    file << command << std::endl;
    file.close();
}

void aof_load(HashTable *ht) {
    std::ifstream file("aof.txt");
    string line;
    while (getline(file, line)) {
        command* cmd = parse_command(line);
        switch (cmd->type) {
            case SET:
                ht_insert(ht, cmd->key, cmd->value);
                break;
            case DEL:
                ht_delete(cmd->key, ht);
                break;
            case APPEND:
                ht_append(ht, cmd->key, cmd->value);
                break;
            case INCR:
                ht_increment(ht, cmd->key);
                break;
            case FLUSHALL:
                ht_flush(ht);
                break;
            case EXPIRE: {
                int seconds = stoi(cmd->value);
                ht_expire(ht, cmd->key, seconds);
                break;
            }
            default:
                break;
        }
        delete cmd;
    }
}
