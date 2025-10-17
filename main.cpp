#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "tinyfiledialogs.h"
using namespace std;

class HashTable {
    static const int SIZE=8;
    vector<vector<pair<string, string>>> table;
    int hashFunction(string s) {
        int hash=0;
        for (char c:s) {
            hash=(hash+c)%SIZE;
        }
        return hash;
    }
public:
    HashTable() : table(SIZE) {

    }

    void insert(const string& s, string key) {
        int idx = hashFunction(s);
        for (auto& p : table[idx]) {
            if (p.first == s) {
                p.second = key;
                return;
            }
        }
        table[idx].emplace_back(s, key);
    }

    string search(const string& s) {
        int idx = hashFunction(s);
        for (auto&p : table[idx]) {
            if (p.first == s) {
                return p.second;
            }
        }
        return "";
    }
};

int main() {
    const char* filters[] = { "*.txt", "*.php"};

    const char* filePath = tinyfd_openFileDialog(
        "Select a file",
        "",
        3, filters,
        "Text and C++ Files",
        0
    );

    if (!filePath)
    {
        return 0;
    }
    vector <string> codeFrags;
    ifstream phpStream(filePath);

    stringstream buffer;
    buffer << phpStream.rdbuf();
    string fileContent = buffer.str();

    string fragment;
    int start=0;
    for (int i =0; i < fileContent.size(); i++) {
        if (fileContent[i]==';') {
            string code=fileContent.substr(start,i-start);
            erase_if(code, [](unsigned char c) {
                return c==';' || c=='\n' || c=='\r' || c==' ';
            });

            codeFrags.push_back(code);
            start = i;
        }
    }

    HashTable tableNames;
    vector<HashTable> tables;

    tableNames.insert("testus", "skibidi");

    for (string fragment:codeFrags) {
        int operation;
        string variableName;
        bool foundDollar=false;
        bool foundArrow=false;
        bool foundExuals=false;
        bool foundBracket=false;
        bool foundEcho=false;

        if (fragment.find("echo") != string::npos) {
            foundEcho=true;
        }
        int fragIndex=0;
        for (char c:fragment) {
            if (c=='$') {
                foundDollar=true;
            }
            if (c=='[') {
                foundBracket=true;
            }
            if (c=='=') {
                if (fragment[fragIndex+1]=='>') {
                    foundArrow=true;
                }else {
                    foundExuals=true;
                }
            }
            fragIndex++;
        }

        //wyppisywanie zmiennej

        if (foundEcho && foundDollar && !foundArrow && !foundExuals && !foundBracket) {
            int variableNameStart=0;
            for (char c:fragment) {
                variableNameStart++;
                if (c=='$') {
                    break;
                }
            }
            string variableName=fragment.substr(variableNameStart);
            string variableValue=tableNames.search(variableName);
            cout << variableValue<<endl;
        }

        //tworzenie zmiennej

        if (!foundEcho && foundDollar && !foundArrow && foundExuals && !foundBracket) {
            int variableNameEnd=0;
            for (char c:fragment) {
                variableNameEnd++;
                if (c=='=') {
                    break;
                }
            }
            string variableName=fragment.substr(1, variableNameEnd-2);
            string variableValue;

            if (fragment[variableNameEnd]==34) {
                variableValue=fragment.substr(variableNameEnd+1, fragment.size()-2-variableNameEnd);
            }else {
                variableValue=fragment.substr(variableNameEnd);
            }

            tableNames.insert(variableName, variableValue);
        }

        //tworzenie tablic

        if (!foundEcho && foundDollar && foundArrow && foundExuals && foundBracket) {
            HashTable thisTable;
            int variableNameEnd=0;
            for (char c:fragment) {
                variableNameEnd++;
                if (c=='=') {
                    break;
                }
            }
            string variableName=fragment.substr(1, variableNameEnd-2);
            int keyValBreakIndex=variableNameEnd;
            for (int i=variableNameEnd+1; i<fragment.size(); i++) {
                if (fragment[i]==',' || fragment[i]==']') {
                    string fragmentWorkedOn= fragment.substr(keyValBreakIndex+1, i-keyValBreakIndex-1);
                    int arrowLocation=0;
                    for (char c:fragmentWorkedOn) {
                        if (c=='>') {
                            break;
                        }
                        arrowLocation++;
                    }
                    string key= fragmentWorkedOn.substr(0, arrowLocation-1);
                    string val=fragmentWorkedOn.substr(arrowLocation);
                    if (key[0]=='"') {
                        key=key.substr(1, key.size()-2);
                    }
                    if (val[0]=='>') {
                        val=val.substr(2, val.size()-3);
                    }
                    thisTable.insert(key, val);
                    keyValBreakIndex=i;
                }

            }
            tables.push_back(thisTable);
            stringstream ss;
            ss << tables.size();
            tableNames.insert(variableName, ss.str() );
        }

        //wyppisywanie zmiennej z tablicy

        if (foundEcho && foundDollar && !foundArrow && !foundExuals && foundBracket) {
            int variableNameStart=0;
            for (char c:fragment) {
                variableNameStart++;
                if (c=='$') {
                    break;
                }
            }
            int variableNameEnd=variableNameStart;
            for (int i=variableNameStart; i<fragment.size(); i++) {

                if (fragment[i]=='[') {
                    variableNameEnd=i;
                    break;
                }
            }
            string variableName=fragment.substr(variableNameStart, variableNameEnd-variableNameStart);
            string keyName=fragment.substr(variableNameEnd+2, fragment.size()-variableNameEnd-4);
            int arrayIndex=stoi( tableNames.search(variableName));
            cout << tables.at(arrayIndex-1).search(keyName) << endl;
        }

        //ustawianie zmiennej w tablicy

        if (!foundEcho && foundDollar && !foundArrow && foundExuals && foundBracket) {
            int variableNameStart=0;
            for (char c:fragment) {
                variableNameStart++;
                if (c=='$') {
                    break;
                }
            }
            int variableNameEnd=variableNameStart;
            for (int i=variableNameStart; i<fragment.size(); i++) {

                if (fragment[i]=='[') {
                    variableNameEnd=i;
                    break;
                }
            }
            string variableName=fragment.substr(variableNameStart, variableNameEnd-variableNameStart);
            int keyNameEnd=variableNameEnd+1;
            for (int i=keyNameEnd; i<fragment.size(); i++)
            {
                if (fragment[i]==']')
                {
                    keyNameEnd=i;
                    break;
                }
            }
            string keyName=fragment.substr(variableNameEnd+2, fragment.size()-keyNameEnd-2);
            if ((int)keyName[keyName.length()-1]==34)
            {
                keyName=keyName.substr(0, keyName.length()-4);
            }
            if ((int)keyName[keyName.length()-1]==61)
            {
                keyName=keyName.substr(0, keyName.length()-3);
            }
            string value=fragment.substr(keyNameEnd+3, fragment.size()-keyNameEnd-4);

            int arrayIndex=stoi( tableNames.search(variableName));

            tables.at(arrayIndex-1).insert(keyName, value);
        }
    }
    int index = stoi( tableNames.search("arr"));
    return 0;
}