#include "hash_table.h"

#include <iostream>
#include <ostream>
#include <vector>
#include <ctime>
HtItem *ht_new_item(string key, string value) {
    HtItem *i = new HtItem;
    i->key = key;
    i->value = value;
    i->expiry = 0;
    return i;
}
HashTable *ht_new(int base_size) {
    HashTable *ht = new HashTable;
    ht->size = base_size;
    ht->count = 0;
    ht->items = new HtItem*[base_size]{};
    return ht;
}
void ht_delete_item(HtItem *i) {
    delete i;
}

void ht_delete(HashTable* ht) {
    for (int i =0; i<ht->size; i++) {
        if (ht->items[i] != nullptr && ht->items[i] != &DELETED_HT_ITEM) {
            ht_delete_item(ht->items[i]);
        }
    }
    delete[] ht->items;
    delete ht;
}
int ht_hash(string key, int prime, int num_buckets) {
    int hash = 0;
    for (int i =0; i<key.length(); i++) {
        int val = (int) key[i];
        hash = (prime * hash+ val )%num_buckets;
    }
    return hash;
}
int ht_get_hash(string key, int num_buckets, int attempt) {
    int hash1 = ht_hash(key, 151, num_buckets);
    int hash2 = ht_hash(key, 163, num_buckets);
    int new_hash = (hash1 + attempt* hash2)% num_buckets;
    return new_hash;
}
void ht_resize(HashTable *ht, int new_size ) {
    int np = next_prime(new_size);
    HashTable *nht= ht_new(np);
    for (int i = 0; i<ht->size; i++) {
        if (ht->items[i]!=nullptr && ht->items[i] != &DELETED_HT_ITEM) {
            ht_insert(nht, ht->items[i]->key, ht->items[i]->value);
        }
    }
    HtItem **old_items = ht->items;
    ht->size = nht->size;
    ht->count = nht->count;
    ht->items =nht->items;
    delete[] old_items;
    delete nht;
}

void ht_insert(HashTable *ht, string key, string value) {
    if (ht->count * 100 / ht->size > 70) {      // if hashtable is at 70% capacity, resize up
        ht_resize(ht, ht->size * 2);
    }
    int attempt {0};
    int hash = ht_get_hash(key, ht->size, attempt);
    while (ht->items[hash]!= nullptr) {
        if (ht->items[hash] != &DELETED_HT_ITEM && ht->items[hash]->key == key) {
            delete ht->items[hash];
            ht->items[hash] = ht_new_item(key, value);
            return;
        }
        attempt ++;
        hash = ht_get_hash(key, ht->size, attempt);
    }
    ht->items[hash]= ht_new_item(key, value);
    ht->count++;
}
bool is_expired(HtItem *i) {
    long long now = (long long) time(nullptr);
    if (i->expiry ==0) return false;
    if (i->expiry < now) return true;
    else return false;
}
string ht_search(HashTable *ht, string key) {
    int attempt = 0;
    int hash = ht_get_hash(key, ht->size, attempt);
    for (; attempt < ht->size; attempt++) {
        hash = ht_get_hash(key, ht->size, attempt);
        if (ht->items[hash] == nullptr) return "";
        else if (ht->items[hash]->key == key) {
            if (is_expired(ht->items[hash]))return "";
            else return ht->items[hash]->value;
        }
    }
    return "";
}

void ht_delete(string key, HashTable *ht) {
    if (ht->count * 100 / ht->size < 10) {   //if table is less than 10% full, resize down
        ht_resize(ht, ht->size / 2);
    }
    int attempt = 0;
    int hash = ht_get_hash(key, ht->size, attempt);
    while (ht->items[hash] !=nullptr) {
        if (ht->items[hash]->key == key) {
            ht->count--;
            delete ht->items[hash];
            ht->items[hash] = &DELETED_HT_ITEM;
            return;
        }else {
            attempt ++;
            hash = ht_get_hash(key, ht->size, attempt);
        }
    }

}

std::vector<string> ht_keys(HashTable *ht){
    std::vector<string> collectedkeys {};
    for (int i = 0; i < ht->size; i++) {
        if (ht->items[i]!= nullptr && ht->items[i] != &DELETED_HT_ITEM) {
            collectedkeys.push_back(ht->items[i]->key);
        }
    }
    return collectedkeys;
}

void ht_flush(HashTable *ht) {
    for (int i =0; i<ht->size; i++) {
       if (ht->items[i]!= nullptr && ht->items[i] != &DELETED_HT_ITEM)  {
           ht_delete_item(ht->items[i]);
           ht->items[i] = nullptr;
        }
    }
    ht->count=0;
}

void ht_append (HashTable *ht, string key, string value) {
    string s2 = ht_search(ht, key);

    if (!s2.empty()) {
        ht_insert(ht, key, s2 + value);
    } else {
        ht_insert(ht, key, value);
    }
}

void ht_increment(HashTable *ht, string key) {
    try {
        int x = std::stoi(ht_search(ht, key));
        x++;
        string s = std::to_string(x);
        ht_insert(ht, key, s);
    } catch (std::invalid_argument e) {
        std::cout<<"not an interger"<<std::endl;
    }

}
HtItem* ht_get_item(HashTable* ht, string key) {
    int attempt = 0;
    for (; attempt < ht->size; attempt++) {
        int hash = ht_get_hash(key, ht->size, attempt);
        if (ht->items[hash] == nullptr) return nullptr;
        if (ht->items[hash] != &DELETED_HT_ITEM &&
            ht->items[hash]->key == key) {
            return ht->items[hash];
            }
    }
    return nullptr;
}
bool ht_expire (HashTable *ht, string key, int seconds) {
    HtItem *i =ht_get_item(ht, key);
    long long now = (long long) time(nullptr);
    if (i != nullptr && i != &DELETED_HT_ITEM) {
        i->expiry = now + seconds;
        return true;
    }else return false;
}
int ht_ttl(HashTable *ht, string key) {
    HtItem *i =ht_get_item(ht, key);
    if (i != nullptr && i != &DELETED_HT_ITEM) {
        if (i->expiry == 0 ) return -1;
        else return (int) (i->expiry - (long long) time(nullptr)) ;
    }else return -2;

}
