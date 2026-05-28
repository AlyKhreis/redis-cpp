#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "command.h"
#include "hash_table.h"
#include "resp.h"
#include "aof.h"

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::cerr << "socket creation failed" << std::endl;
        return 1;
    }
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int rv = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (rv < 0) {
        std::cerr << "port binding failed" << std::endl;
        return 1;
    }
    rv = listen(fd, SOMAXCONN);
    if (rv < 0) {
        std::cerr << "failed to listen" << std::endl;
        return 1;
    }

    struct sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);
    HashTable* ht = ht_new(53);
    aof_load(ht);

    while (true) {
        int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            std::cerr << "server failed to accept client" << std::endl;
            return 1;
        }

        while (true) {
            char buf[256] = {};
            int n = read(client_fd, buf, sizeof(buf));
            if (n <= 0) break;

            vector<string> tokens = parse_resp(buf);
            command* cmd = parse_command_from_resp(tokens);
            std::string response;

            switch (cmd->type) {
                case SET:
                    ht_insert(ht, cmd->key, cmd->value);
                    aof_write("SET " + cmd->key + " " + cmd->value);
                    response = resp_simple_string("OK");
                    break;
                case GET: {
                    string val = ht_search(ht, cmd->key);
                    response = val.empty() ? resp_null() : resp_bulk_string(val);
                    break;
                }
                case DEL:
                    ht_delete(cmd->key, ht);
                    aof_write("DEL " + cmd->key);
                    response = resp_simple_string("OK");
                    break;
                case EXISTS:
                    response = resp_integer(ht_search(ht, cmd->key).empty() ? 0 : 1);
                    break;
                case KEYS: {
                    vector<string> keys = ht_keys(ht);
                    response = resp_array(keys);
                    break;
                }
                case FLUSHALL:
                    ht_flush(ht);
                    aof_write("FLUSHALL");
                    response = resp_simple_string("OK");
                    break;
                case APPEND:
                    ht_append(ht, cmd->key, cmd->value);
                    aof_write("APPEND " + cmd->key + " " + cmd->value);
                    response = resp_simple_string("OK");
                    break;
                case INCR: {
                    ht_increment(ht, cmd->key);
                    string val = ht_search(ht, cmd->key);
                    aof_write("INCR " + cmd->key);
                    response = resp_integer(stoi(val));
                    break;
                }
                case EXPIRE: {
                    int seconds = stoi(cmd->value);
                    bool result = ht_expire(ht, cmd->key, seconds);
                    aof_write("EXPIRE " + cmd->key + " " + cmd->value);
                    response = resp_integer(result ? 1 : 0);
                    break;
                }
                case TTL: {
                    int ttl = ht_ttl(ht, cmd->key);
                    response = resp_integer(ttl);
                    break;
                }
                case UNKNOWN:
                    response = resp_error("unknown command");
                    break;
            }
            write(client_fd, response.c_str(), response.size());
            delete cmd;
        }

        close(client_fd);
    }
}