#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include "tinyfiledialogs.h"
#include "hashTable.h"
using namespace std;

bool debug = false;

// class PHPizer
// {
//     static string cleanFragment(const string& fragment) {
//         string cleaned;
//         for (char c : fragment) {
//             if (c != ';' && c != '\n' && c != '\r' && c != ' ') {
//                 cleaned += c;
//             }
//         }
//         return cleaned;
//     }
//
//     static string extractValue(const string& fragment, size_t startPos) {
//         if (startPos >= fragment.size()) return "";
//
//         string value;
//         if (fragment[startPos] == '"') {
//             size_t endQuote = fragment.find('"', startPos + 1);
//             if (endQuote != string::npos) {
//                 value = fragment.substr(startPos + 1, endQuote - startPos - 1);
//             }
//         } else {
//             value = fragment.substr(startPos);
//         }
//         return value;
//     }
//
//     static int findChar(const string& str, char c) {
//         return str.find(c);
//     }
//
// };

int main() {
    std::cout << "TinyFD version: " << tinyfd_version << std::endl;

    const char* filePath = tinyfd_openFileDialog(
        "Select a file",
        "",
        0, nullptr,
        nullptr,
        0
    );

    if (!filePath)
        std::cout << "Dialog canceled or failed." << std::endl;
    else
        std::cout << "Selected: " << filePath << std::endl;
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
                if (fragIndex + 1 < fragment.size() && fragment[fragIndex + 1] == '>') {
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

            if (fragment[variableNameEnd]==string::npos) {
                variableValue=fragment.substr(variableNameEnd+1, fragment.size()-2-variableNameEnd);
            }else {
                variableValue=fragment.substr(variableNameEnd);
            }
            if (debug){
                cout << "Variable: " << variableName << "Value: " << variableValue <<  endl;
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
            if (debug){
                cout << "Variable: " << variableName <<  endl;
            }
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
            if (debug)
            {
                cout << "Variable: " << variableName << "Key: " << keyName <<  endl;
                cout << "Indeks: " << tableNames.search(variableName) << endl;
            }

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
            if ((int)keyName[keyName.length()-1]==string::npos)
            {
                keyName=keyName.substr(0, keyName.length()-4);
            }
            if ((int)keyName[keyName.length()-1]==61)
            {
                keyName=keyName.substr(0, keyName.length()-3);
            }
            string value=fragment.substr(keyNameEnd+3, fragment.size()-keyNameEnd-4);
            int arrayIndex=stoi( tableNames.search(variableName));
            if (debug){
                cout << "Variable: " << variableName << "Key: " << keyName <<  endl;
            }
            tables.at(arrayIndex-1).insert(keyName, value);
        }
    }

    return 0;
}