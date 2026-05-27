#ifndef HASHTABLE_HASH_TABLE_H
#define HASHTABLE_HASH_TABLE_H
#include "prime.h"
#include <string>
#include <vector>
using std::string;


struct HtItem {
    string key;
    string value;
    long long expiry;
};
static HtItem DELETED_HT_ITEM = {"", ""};// used for deleted items to leave a mark, if two items collided and you move it up a spot, when searching for it will find it
struct HashTable {
    int size;
    int count;
    HtItem **items;// a pointer to an array of pointers
};
HtItem *ht_new_item(string key, string value);
HashTable *ht_new(int base_size);
void ht_delete_item(HtItem *i);
void ht_delete(HashTable* ht);
int ht_hash(string key, int prime, int num_buckets);
int ht_get_hash(string key, int num_buckets, int attempt);
void ht_insert(HashTable* ht, string key, string value);
string ht_search(HashTable *ht, string key);
void ht_delete(string key, HashTable *ht);
std::vector<string> ht_keys(HashTable *ht);
void ht_flush(HashTable *ht);
void ht_append(HashTable *ht, string key, string value);
void ht_increment(HashTable *ht, string key);
bool is_expired(HtItem *i);
bool ht_expire (HashTable *ht, string key, int seconds);
int ht_ttl(HashTable *ht, string key);
#endif //HASHTABLE_HASH_TABLE_H

