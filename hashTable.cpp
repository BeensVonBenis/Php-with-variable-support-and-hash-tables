#include "hashTable.h"
using namespace std;

int HashTable::hashFunction(const string& s) {
    int hash = 0;
    for (char c : s) {
        hash = (hash + c) % SIZE;
    }
    return hash;
}

HashTable::HashTable() : table(SIZE) {}

void HashTable::insert(const string& s, const string& key) {
    int idx = hashFunction(s);
    for (auto& p : table[idx]) {
        if (p.first == s) {
            p.second = key;
            return;
        }
    }
    table[idx].emplace_back(s, key);
}

string HashTable::search(const string& s) {
    int idx = hashFunction(s);
    for (auto& p : table[idx]) {
        if (p.first == s) {
            return p.second;
        }
    }
    return "";
}
