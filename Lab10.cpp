#include <string>
#include <regex>
#include <iostream>
#include <vector>
#include <fstream>

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


string trimNumber(string input) {
    vector<string> splitInput = splitString(input, '.');
    string integer = splitInput[0];
    string decimal = splitInput[1];
    
    bool leadingNumberFound = false;
    for(int i = 0; i < integer.length() && !leadingNumberFound; i++) {
        if(isdigit(integer[i])) {
            if(integer[i] != '0') {
                leadingNumberFound = true;
            } else {
                integer.erase(i);
            }
        }
    }

    leadingNumberFound = false;
    for(int i = decimal.length(); i > 0 && !leadingNumberFound; i--) {
        if(isdigit(decimal[i])) {
            if(decimal[i] != '0') {
                leadingNumberFound = true;
            } else {
                decimal.erase(i);
            }
        }
    }

    return integer + '.' + decimal;
    
}

string addStrings(string num1, string num2) {
    bool num1Negative = false;
    bool num2Negative = false;
    string sum;
    if (checkValid(num1)) {
        if (num1[0] == '-') {
            num1Negative = true;
            num1.erase(0, 1);
        } else if (num1[0] == '+') {
            num1Negative = false;
            num1.erase(0, 1);
        } else {
            num1Negative = false;
        }
    } else {
        return num1 + " is an invalid input!";
    }

    if (checkValid(num2)) {
        if (num2[0] == '-') {
            num2Negative = true;
            num2.erase(0, 1);
        } else if (num2[0] == '+') {
            num2Negative = false;
            num2.erase(0, 1);
        } else {
            num2Negative = false;
        }
    } else {
        return num2 + " is an invalid input!";
    }

    if (num1Negative && !num2Negative) {
        string temp = num1;
        num1 = num2;
        num2 = temp;
        num2Negative = true;
        num1Negative = false;
    }

    // equalizes the lengths of each number by adding 0s
    vector<string> splitNum1 = splitString(num1, '.');
    vector<string> splitNum2 = splitString(num2, '.');

    // adds 0s to the beginning of the shortest number to equalize integer length
    if (splitNum1[0].length() < splitNum2[0].length()) {
        num1.insert(num1.begin(), splitNum2[0].length() - splitNum1[0].length(), '0');
    } else {
        num2.insert(num2.begin(), splitNum1[0].length() - splitNum2[0].length(), '0');
    }

    // adds 0s to the end of the shortest number to equalize decimal length
    if (splitNum1[1].length() < splitNum2[1].length()) {
        for (int i = 0; i < splitNum2[1].length() - splitNum1[1].length(); i++) {
            num1 += '0';
        }
    } else {
        for (int i = 0; i < splitNum1[1].length() - splitNum2[1].length(); i++) {
            num2 += '0';
        }
    }

    bool goesNegative = false;

    bool carry = false;
    for (int i = num1.length() - 1; i >= 0; i--) {
        int asciiSum;
        if (num1[i] == '.') {
            sum.insert(sum.begin(), '.');
            continue;
        }
        if (num2Negative) {
            asciiSum = num1[i] - num2[i] + charOffset - carry;
            carry = false;

            if (asciiSum - charOffset < 0) {
                if (i == 0 && num2Negative) {
                    goesNegative = true;
                }
                carry = true;
                int offset = abs((asciiSum - charOffset))%10;
                if (offset == 0) {
                    asciiSum = charOffset;
                } else {
                    asciiSum = 10 - offset + charOffset;
                }
            } 
        } else {
            asciiSum = num1[i] + num2[i] - charOffset + carry;
            carry = false;
            if (asciiSum - charOffset > 9) {
                carry = true;
                asciiSum = (asciiSum - charOffset)%10 + charOffset;
            }
        }

        sum.insert(sum.begin(), asciiSum);
    }

    if (goesNegative) {
        string oneDigitMore = "1";
        for (int i = 0; i < splitNum1[0].length() + 1; i++) {
            oneDigitMore += '0';
        }
        oneDigitMore += ".0";
        sum = addStrings(oneDigitMore, '-' + sum);
        sum.insert(sum.begin(), '-');
    } else if (carry && !goesNegative) {
        sum.insert(sum.begin(), '1');
    }

    if (num1Negative && num2Negative) {
        sum += '-';
    }


    return sum;
}

int main() {
    string line;
    string fileName;
    cout << "Input the name of your input file (including the file extension): ";
    cin >> fileName;

    ifstream inputFile(fileName);

    if (inputFile.is_open()) {
        while(getline(inputFile, line)) {
            cout << "=========" << endl;
            vector<string> splitNumbers = splitString(line, ' ');
            if (splitNumbers.size() != 2) {
                cout << "Lines must have exactly 2 numbers!\n";
                continue;
            }
            if(!stringContains(splitNumbers[0], '.')) {
                splitNumbers[0] += ".0";
            }
            if(!stringContains(splitNumbers[1], '.')) {
                splitNumbers[1] += ".0";
            }
            cout << "Number 1: " << splitNumbers[0] << endl;
            cout << "Number 2: " << splitNumbers[1] << endl;
            cout << "Sum of numbers 1 and 2: " << addStrings(splitNumbers[0], splitNumbers[1]) << endl;
        }
    } else {
        cout << "Could not find an input file called " << fileName << "!\n";
    }
    
    return 0;
}