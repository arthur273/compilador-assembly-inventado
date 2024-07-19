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
    int linha;
};

struct TDEFinstance {
    int value;
    int linha;
};

struct TUSOinstance {
    string nome;
    int mem_pos;
    int linha;
};
//checkar program size para operandos utilizando hasFlag val,2+;val,5+

// funcao single pass precisa retornar: lista de inteiros(codigo), tabela de uso(map), tabela de definicoes(map) e realocacao(string ou vetor)
tuple<vector<int>, multimap<string,int> , map<string, int>, vector<int>, int> singlePass(vector<vector<string>> &programa){
    vector<int> obj;
    map<string, int> TUSO;
    multimap<string,int> TUSO_map;
    map <string, int> TDEF_final;
    vector<int> real;
    int ligacao = 0;  // 0 => nao passa tuso, tdeg real 2 =>  passa tuso, tdeg real, 1 => erro

    //map <string, TUSOinstance> TUSO;
    map <string, TDEFinstance> TDEF;
    map<string, TSinstance> TS;

    for (int contador_linha = 1, i = 0; i < programa.size(); contador_linha++, i++){
        bool hasLabel; // Tudo esta declarado aqui para diminuir a quantidade de codigo e nao precisar zerar tudo a cada iteracao
        vector<string> ops_sep;
        string operands_joined;
        string label;
        string operation;
        int contador_posicao = 0;
        int next_adress;
        string operand;

       if (isLabel(programa[i][0])) {
            label = programa[i][0].substr(0, programa[i][0].size()-1); // tira os dois pontos da label
            operation = programa[i][1];
            if ((contador_linha == 1) && (operation == "BEGIN")){
                ligacao += 1;
                continue;
                }
            if (programa[i].size() > 2){
                operands_joined = programa [i][2];
                ops_sep = separateOperands(operands_joined);}
            if (isalpha(label[0]) == false) {
                cout << ("LEXICAL ERROR: label defined with non-alphabetical character at line " + to_string(contador_linha) + "." + "\n");
            }

            if (isValidLabel(label) == false) {
                cout << ("LEXICAL ERROR: label defined with non-alphanumerical character at line " + to_string(contador_linha) + "." + "\n");
            }
            if (TS.count(label)){
               if (TS[label].flag){ // Ja esta definido
                    throw invalid_argument("SEMANTICAL ERROR: redefinicao de label " + to_string(contador_linha) + "." + "\n");
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
           else {// se nao esta na tabela de simbolos, inserir
                TS[label].flag = true;
                TS[label].value = static_cast<int>(obj.size());
                }

            if (count(diretivas.begin(), diretivas.end(), operation)) { // TRATAMENTO DE DIRETIVAS COM LABEL
                if (operation == "SPACE"){
                    obj.push_back(0);
                    real.push_back(0);
                    if (programa[i].size() > 2) {cout << ("ERRO SINTATICO: NUMERO DE ARGUMENTOS ERRADO NA LINHA " + to_string(contador_linha) + "." + "\n");}
            }
                if (operation == "CONST"){
                    obj.push_back(stoi(ops_sep[0]));
                    real.push_back(0);
                    if (ops_sep.size() != 1 || programa[i].size() > 3){
                            cout << ("ERRO SINTATICO: NUMERO DE ARGUMENTOS ERRADO NA LINHA " + to_string(contador_linha) + "." + "\n");}
                }
            contador_posicao += 1;
            continue;
            }
            if (operation == "EXTERN"){
                TUSO[label] = 0;
                if (programa[i].size() > 2) {cout << ("ERRO SINTATICO: ARGUMENTO PARA EXTERN, LINHA " + to_string(contador_linha) + "." + "\n");}
                continue;
            }
        }
        else{
            operation = programa[i][0];
            if (programa[i].size() > 1){
                operands_joined = programa[i][1]; // O que acontece se nao ha operdores ?
                ops_sep = separateOperands(operands_joined);}
        }
        if (operation == "END"){ // se end antes do programa terminar, programa termina
            ligacao += 1;
            if (programa[i].size() > 1) {cout << ("ERRO SINTATICO: END NAO ACEITA ARGUMENTOS, LINHA " + to_string(contador_linha) + "." + "\n");}
            break;
        }
        if (operation == "PUBLIC"){
            if (programa[i].size() > 2){
                 cout << ("ERRO SINTATICO: NUMERO DE ARGUMENTOS ERRADO NA LINHA " + to_string(contador_linha) + "." + "\n");
            }
            TDEF[ops_sep[0]].value = 0;
            TDEF[ops_sep[0]].linha = contador_linha;
        }

        else if (opcodes.count(operation)) {
            if ((operation == "COPY") && (ops_sep.size() != 2)) {
                cout <<("ERRO SINTATICO: NUMERO DE ARGUMENTOS ERRADO NA LINHA " + to_string(contador_linha) + "." + "\n");
            }

            if ((operation == "STOP") && (ops_sep.size() != 0)) {
                cout << ("ERRO SINTATICO: NUMERO DE ARGUMENTOS ERRADO NA LINHA " + to_string(contador_linha) + "." + "\n");
            }

            if ((operation != "COPY") && (operation != "STOP") && (ops_sep.size()) != 1) {
                cout << ("ERRO SINTATICO: NUMERO DE ARGUMENTOS ERRADO NA LINHA " + to_string(contador_linha) + "." + "\n");
            }
            obj.push_back(getValue(operation, opcodes));
            real.push_back(0);
            for (int k=0; k < ops_sep.size(); k++){
                operand = ops_sep[k];
                if (TUSO.count(operand)){
                    obj.push_back(0);
                    TUSO_map.insert(std::make_pair(operand, static_cast<int>(obj.size())));
                    real.push_back(1);
                    continue;
                }
                else if (TS.count(operand)){
                        if (TS[operand].flag){
                            obj.push_back(TS[operand].value);}
                        else{
                            obj.push_back(TS[operand].reff);
                            TS[operand].reff = static_cast<int>(obj.size() - 1);} // obj.size() - 1 seria a posicao dele no objeto
                }
                else{
                    obj.push_back(-1);
                    TSinstance symbol_instance = {0, false, static_cast<int>(obj.size() - 1), contador_linha};
                    TS[operand] = symbol_instance;
                }
                real.push_back(1);
            }
            contador_posicao += 1 + ops_sep.size();
        }
        else{
            cout << ("ERRO SINTATICO: OPERACAO INVALIDA NA LINHA" + to_string(contador_linha) + "." + "\n");
            }
    }


    for (auto it = TDEF.begin(); it != TDEF.end(); ++it) {
        //std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
        if (TS.count(it->first)){
            if (TS[it->first].flag){
                TDEF[it->first].value = TS[it->first].value;
                TDEF_final[it->first] = TDEF[it->first].value;
                continue;}
        cout << ("ERRO SEMANTICO: LABEL NAO DECLARADA PARA USO PUBLICO NA LINHA " + to_string(TDEF[it->first].linha) + "." + "\n");
        }
    }

    // check if any flag is fase in TS
    bool anyFalse = false;
    for (const auto& pair : TS) {
        const TSinstance& data = pair.second;
        if (!data.flag) {
            anyFalse = true;
            cout << ("ERRO SEMANTICO: LABEL NAO DECLARADA NA LINHA " + to_string(data.linha) + "." + "\n");
            break; // No need to continue if we found a false flag
        }
    }

    return make_tuple(obj, TUSO_map, TDEF_final, real, ligacao);
}

#endif
