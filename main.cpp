#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>


#include "tokenizer.h"
#include "auxiliares.h"
#include "stages.h"

using namespace std;

void printMatrix(const std::vector<std::vector<std::string>>& matrix) {
    // Determine the maximum width needed for each column
    std::vector<size_t> maxColumnWidths(matrix[0].size(), 0);

    for (const auto& row : matrix) {
        for (size_t col = 0; col < row.size(); ++col) {
            if (row[col].length() > maxColumnWidths[col]) {
                maxColumnWidths[col] = row[col].length();
            }
        }
    }

    // Print the matrix
    for (const auto& row : matrix) {
        for (size_t col = 0; col < row.size(); ++col) {
            // Print each element, adjusting width for alignment
            std::cout << std::setw(maxColumnWidths[col]) << std::left << row[col] << " ";
        }
        std::cout << std::endl;
    }
}


int main(int argc, char *argv[]){
    std::string op = argv[1];
    std::string arquivo = argv[2];
    std::string file_name = arquivo.substr(0, arquivo.find_last_of('.'));
    std::string extension;
    string objeto;

    vector<vector<string>> programa = tokenParser(arquivo);
    cout << "tokenized \n";
    printMatrix(programa);
    if(op == "-p"){ // .PRE
        preprocess(programa);
        cout << "preprocessed \n";
        printMatrix(programa);
        //createFile(programa, file_name, ".PRE");
        return 0;
    }

}
