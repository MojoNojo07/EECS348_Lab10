#include <string>
#include <regex>
#include <iostream>
#include <vector>

using namespace std;

const int charOffset = 48;

struct Number{
    string integer;
    string decimal;
    bool negative;
};

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

bool stringContains(string input, char target) {
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == target) {
            return true;
        }
    }

    return false;
}

string add(string num1, string num2) {
    num1.insert(num1.begin(), 4, '1');
    return num1;
}

bool checkValid(string input) {
    if (!isdigit(input[0]) && input[0] != '+' && input[0] != '-') {
        return false;
    }

    bool decimalFound = false;
    for (int i = 1; i < input.length(); i++) {
        if (!isdigit(input[i])) {
            if (input[i] = '.' && !decimalFound) {
                decimalFound = true;
            } else {
                return false;
            }
        }
    }

    return true;
}

Number toNumber(string input) {
    Number returnNumber;

    if (checkValid(input)) {
        if (input[0] == '-') {
            returnNumber.negative = true;
            input = input.substr(1);
        } else if (input[0 == '+']) {
            returnNumber.negative = false;
            input = input.substr(1);
        } 
        else {
            returnNumber.negative = false;
        }

        if (stringContains(input, '.')) {
            vector<string> splitDecimal = splitString(input, '.');
            returnNumber.integer = splitDecimal[0];
            returnNumber.decimal = splitDecimal[1];
        } else {
            returnNumber.integer = input;
        }

    } else {
        cout << "Invalid number format!\n";
    }
    return returnNumber;
}

int main() {
    // string number = "102 205";

    // vector<int> charNums = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    // for (int i = 0; i < charNums.size(); i++) {
    //     cout << charNums[i] << endl;
    // }

    cout << checkValid("Yes hellow") << endl;
    cout << checkValid("-211.4.5") << endl;

    char addedChars = (('2'- charOffset) + ('4' - charOffset)) + charOffset;

    cout << "Added numbers: " << addedChars << endl;

    return 0;

    // vector<string> splitNumber = splitString(number, ' ');

    // string added = add(splitNumber[0], splitNumber[1]);
    // cout << added << endl;

}