#include <string>
#include <regex>
#include <iostream>
#include <vector>

using namespace std;

// function stolen from my previous lab submission
vector<string> splitString(string input, char delimiter) {
    vector<string> split = {""};
    for (char character : input) {
        if (character == delimiter) {
            split.push_back("");
        } else {
            split.at(split.size() - 1) += character;
        }
    }

    return split;
} 

int main() {
    string number = "102 205";

    vector<string> splitNumber = splitString(number, ' ');

    for (int i = 0; i < splitNumber.size(); i++) {
        cout << splitNumber[i] << endl;
    }

}