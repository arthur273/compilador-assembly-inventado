#ifndef STAGES_H_INCLUDED
#define STAGES_H_INCLUDED

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>

#include "auxiliares.h"


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

#endif
