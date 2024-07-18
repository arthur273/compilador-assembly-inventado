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
{"MUL", 3},
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

vector<string> diretivas = {"CONST", "SPACE"};


void preprocess (vector<vector<string>> &programa) {
    vector<int> remove_index;
    map <string,int> equs;
    int value;
    string label;
    bool removida;

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

        if (isLabel(programa[i][0]) && programa[i][1] == "IF") {
            removida = false;
            value = getValue(programa[i][2], equs);
            if (value == 0) {
                removida = true;
                remove_index.push_back(i + 1); // remove a próxima
            }
            programa[i].erase(programa[i].begin() + 1, programa[i].end()); // remover "IF" e argumentos, mantém a label

        if (isLabel(programa[i][0]) && programa[i].size() == 1) {
            int j = removida ? i + 2 : i + 1; // ajusta o índice de início dependendo se uma linha foi removida

            while (j < programa.size() && (programa[j].empty() || !isLabel(programa[j][0])) && programa[i].size() == 1) { // itera ate encontrar uma linha nao vazia sem label
                if (!programa[j].empty() && !isLabel(programa[j][0])) {
                    programa[i].insert(programa[i].end(), programa[j].begin(), programa[j].end());
                    remove_index.push_back(j); // marca a linha atual para remoção
                }
                j++;
            }
        }

        }if (programa[i][0] == "IF") {
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


struct TSinstance {
    int value;
    bool flag;
    int reff;
};

//checkar program size para operandos utilizando hasFlag

// funcao single pass precisa retornar: lista de inteiros(codigo), tabela de uso(map), tabela de definicoes(map) e realocacao(string ou vetor)
std::tuple<vector<int>, map<std::string, int>, map<std::string, int>, vector<int>> singlePass(vector<vector<string>> &programa){

    vector<int> obj;
    map <string,int> tuso;
    map <string,int> tdef;
    vector<int> real;

    map<string, TSinstance> TS;
    bool hasLabel;
    vector<string> ops_sep;
    string operands_joined;
    string label;
    string operation;
    int contador_posicao = 0;
    int next_adress;
    string operand;

    for (int contador_linha = 1, i = 0; i < programa.size(); contador_linha++, i++){

       if (isLabel(programa[i][0])) {
            label = programa[i][0].substr(0, programa[i][0].size()-1); // tira os dois pontos da label
            operation = programa[i][1];
            if (programa[i].size() > 2){
                operands_joined = programa [i][2];
                ops_sep = separateOperands(operands_joined);}
            if (isalpha(label[0]) == false) {
                throw invalid_argument("LEXICAL ERROR: label defined with non-alphabetical character at line " + to_string(contador_linha) + ".");
            }

            if (isValidLabel(label) == false) {
                throw invalid_argument("LEXICAL ERROR: label defined with non-alphanumerical character at line " + to_string(contador_linha) + ".");
            }
            if (TS.count(label)){
               if (TS[label].flag){ // Ja esta definido
                    throw invalid_argument("SEMANTICAL ERROR: redefinicao de label " + to_string(contador_linha) + ".");
               }else{ // Esta na TS mas ainda ha pendencias e ainda nao foi definido
                   TS[label].flag = true;
                   TS[label].value = static_cast<int>(obj.size());
                   next_adress = 0;
                   while (TS[label].reff != -1){
                        next_adress = TS[label].reff;
                        TS[label].reff = obj[next_adress];
                        obj[next_adress] = TS[label].value;
                   }
                }
            }
           else { // se nao esta na tabela de simbolos, inserir
                TS[label].flag = true;
                TS[label].value = static_cast<int>(obj.size());
                }

            if (count(diretivas.begin(), diretivas.end(), operation)) {
                if (operation == "SPACE"){
                    obj.push_back(0);
                    if (programa[i].size() > 2) {throw invalid_argument("SYNTAX ERROR: wrong number of arguments at line " + to_string(contador_linha) + ".");}
            }
                if (operation == "CONST"){
                    obj.push_back(stoi(ops_sep[0]));
                    if (ops_sep.size() != 1 || programa[i].size() > 3){
                            throw invalid_argument("SYNTAX ERROR: wrong number of arguments at line " + to_string(contador_linha) + ".");}
                }
            contador_posicao += 1;
            continue;
            }
        }
        else{
            operation = programa[i][0];
            operands_joined = programa[i][1]; // O que acontece se nao ha operdores ?
            ops_sep = separateOperands(operands_joined);
        }
        if (opcodes.count(operation)) {
            if ((operation == "COPY") && (ops_sep.size() != 2)) {
                throw invalid_argument("SYNTAX ERROR: wrong number of arguments at line " + to_string(contador_linha) + ".");
            }

            if ((operation == "STOP") && (ops_sep.size() != 0)) {
                throw invalid_argument("SYNTAX ERROR: wrong number of arguments at line " + to_string(contador_linha) + ".");
            }

            if ((operation != "COPY") && (operation != "STOP") && (ops_sep.size()) != 1) {
                throw invalid_argument("SYNTAX ERROR: wrong number of arguments at line " + to_string(contador_linha) + ".");
            }
            obj.push_back(getValue(operation, opcodes));
            for (int k=0; k < ops_sep.size(); k++){
                operand = ops_sep[k];
                if (TS.count(operand)){
                        if (TS[operand].flag){
                            obj.push_back(TS[operand].value);}
                        else{
                            obj.push_back(TS[operand].reff);
                            TS[operand].reff = static_cast<int>(obj.size() - 1);} // obj.size() - 1 seria a posicao dele no objeto
                }
                else{
                    obj.push_back(-1);
                    TSinstance symbol_instance = {0, false, static_cast<int>(obj.size() - 1)};
                    TS[operand] = symbol_instance;
                }
            }
            contador_posicao += 1 + ops_sep.size();
        }
        else{
            throw invalid_argument("ERRO SINTATICO: Operacao invalida na linha" + to_string(contador_linha) + ".");
            }
        }
        return std::make_tuple(obj, tuso, tdef, real);
}

#endif
