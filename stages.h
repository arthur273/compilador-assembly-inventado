#ifndef STAGES_H_INCLUDED
#define STAGES_H_INCLUDED

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>

#include "auxiliares.h"

map <string,int> opcodes = {
{"ADD", 1},
{"SUB", 2},
{"MULT", 3},
{"DIV", 4},
{"JMP", 5},
{"JMPN", 6},
{"JMPP", 7},
{"JMPZ", 8},
{"COPY", 9},
{"LOAD", 10},
{"STORE", 11},
{"INPUT", 12},
{"OUTPUT", 13},
{"STOP", 14}};


void preprocess (vector<vector<string>> &programa) {
    vector<int> remove_index;
    map <string,int> equs;
    int value;
    string label;

    for (int i=0; i < programa.size(); i++){
        if (programa[i].size() > 1) {
            if (programa[i][1] == "EQU") {
                label = programa[i][0].substr(0, programa[i][0].size()-1); // tira os dois pontos da label
                value = atoi(programa[i][2].c_str());
                equs.insert(pair<string, int>(label, value));
                remove_index.push_back(i);
                continue;
            }
        }

        if (programa[i][0] == "IF") {
            value = getValue(programa[i][1], equs); // get Value retorna programa[i][1] se nao encontrado em getValue
            remove_index.push_back(i);
            if (value == 0) {
                remove_index.push_back(i+1);
            }
        } else {
            if (programa[i].size() > 1) {
                if (programa[i][1] == "CONST") {
                    if (programa[i].size() > 2) {
                        if (inMap(programa[i][2], equs)) {
                            value = getValue(programa[i][2], equs);
                            programa[i][2] = to_string(value);
                        }
                    }
                }
            }
        }
    }

    sort(remove_index.begin(), remove_index.end(), greater<int>());

    for (int i=0; i < remove_index.size(); i++) {
        programa.erase(programa.begin() + remove_index[i]);
    }
}

struct data {
    int value;
    bool flag;
    string reference;
};


struct tsinstance {
    std::string name;
    int value;
    bool flag;
    int reference;
};
// funcao single pass precisa retornar: lista de inteiros(codigo), tabela de uso(map), tabela de definicoes(map) e realocacao(string ou vetor)
std::tuple<vector<int>, map<std::string, int>, map<std::string, int>, vector<int>> singlepass(vector<vector<string>> &programa);{

    vector<int> code;
    map <string,int> tuso;
    map <string,int> tdef;
    vector<int> real;

    bool hasLabel;
    vector<string> ops_sep;
    string ops_joined;
    string label;
    int next_adress;

    for (int i=0; i < programa.size(); contador_linha++, i++){
           for(int j=0; programa[i].size(); j++){}
           if (isLabel(programa[i][0])) {
                label = getLabel(programa[i][0]);

                if (isalpha(label[0]) == false) {
                    throw invalid_argument("LEXICAL ERROR: label defined with non-alphabetical character at line " + to_string(contador_linha) + ".");
                }

                if (isValidLabel(label) == false) {
                    cout << label;
                    cout << "\n";
                    throw invalid_argument("LEXICAL ERROR: label defined with non-alphanumerical character at line " + to_string(contador_linha) + ".");
                }
                if (inTS(label, ts)){
                   if (TS[label].flag){ // Ja esta definido
                        throw invalid_argument("SEMANTICAL ERROR: redefinicao de label " + to_string(contador_linha) + ".");
                   }else{ // Esta na TS mas ainda ha pendencias e ainda nao foi definido
                       TS[label].flag = true;
                       TS[label].value = contador_posicao;
                       while (next_adress != -1 ){
                            next_adress = TS[label].reference
                       }
                   }
               }else {
                    TS.insert(pair<string, int>(label, contador_posicao));
                }
            }

    }
}



#endif
