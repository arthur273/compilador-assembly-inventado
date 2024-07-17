#ifndef STAGES_H_INCLUDED
#define STAGES_H_INCLUDED

#include <iostream>
#include <map>
#include <string>


struct Data {
    int value;
    bool flag;
    std::string reference;
};

int main() {
    std::map<std::string, Data> myMap;

    // Example usage:
    Data d1 = {10, true, "ref1"};
    Data d2 = {20, false, "ref2"};

    myMap["L1"] = d1;
    myMap["L2"] = d2;

    // Accessing elements:
    std::cout << "Value of item1: " << myMap["L!"].value << std::endl;
    std::cout << "Flag of item2: " << (myMap["l2"].flag ? "true" : "false") << std::endl;

    return 0;
}

#endif
