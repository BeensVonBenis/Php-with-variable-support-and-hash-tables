//
// Created by kowal on 17.10.2025.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class HashTable {
    static const int SIZE = 8;
    vector<vector<pair<string, string>>> table;

    int hashFunction(const string& s);

public:
    HashTable();

    void insert(const string& s, const string& key);
    string search(const string& s);
};

#endif