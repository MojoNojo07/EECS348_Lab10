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

string numberToString(Number input) {
    return input.integer + '.' + input.decimal;
}

Number toNumber(string input) {
    Number returnNumber;

    if (checkValid(input)) {
        if (input[0] == '-') {
            returnNumber.negative = true;
            input = input.substr(1);
        } else if (input[0] == '+') {
            returnNumber.negative = false;
            input = input.substr(1);
        } else {
            returnNumber.negative = false;
        }

        if (stringContains(input, '.')) {
            vector<string> splitDecimal = splitString(input, '.');
            returnNumber.integer = splitDecimal[0];
            returnNumber.decimal = splitDecimal[1];
        } else {
            returnNumber.integer = input;
            returnNumber.decimal = "0";
        }

    } else {
        cout << "Invalid number format!\n";
    }
    return returnNumber;
}

string addStrings(string num1, string num2, bool neg) {
    string sum;

    if (num1.length() < num2.length()) {
        num1.insert(num1.begin(), num2.length() - num1.length(), '0');
    } else {
        num2.insert(num2.begin(), num1.length() - num2.length(), '0');
    }

    bool goesNegative = false;

    bool carry = false;
    for (int i = num1.length() - 1; i >= 0; i--) {
        int asciiSum;
        if (num1[i] == '.') {
            sum.insert(sum.begin(), '.');
            continue;
        }
        if (neg) {
            asciiSum = num1[i] - num2[i] + charOffset - carry;
            carry = false;

            if (asciiSum - charOffset < 0) {
                if (i == 0 && neg) {
                    goesNegative = true;
                }
                cout << "carrying, jumped from " << asciiSum - charOffset;
                carry = true;
                int offset = abs((asciiSum - charOffset))%10;
                if (offset == 0) {
                    asciiSum = charOffset;
                } else {
                    asciiSum = 10 - offset + charOffset;
                }
                cout << " down to " << asciiSum - charOffset << endl;
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
        for (int i = 0; i < num1.length(); i++) {
            oneDigitMore += '0';
        }
        cout << "negativizing by performing " << oneDigitMore << " - " << sum << endl;
        sum = addStrings(oneDigitMore, sum, true);
        sum.insert(sum.begin(), '-');
    } else if (carry && !goesNegative) {
        sum.insert(sum.begin(), '1');
    }

    return sum;
}

Number add(Number num1, Number num2) {
    Number addedNumbers;

    string added;
    if (num1.negative && num2.negative) {
        added = addStrings(num1.integer + '.' + num1.decimal, num2.integer + '.' + num2.decimal, false);
    } else if (num1.negative) {
        added = addStrings(num2.integer + '.' + num2.decimal, num1.integer + '.' + num1.decimal, true);
    } else if (num2.negative) {
        added = addStrings(num1.integer + '.' + num1.decimal, num2.integer + '.' + num2.decimal, true);
    } else {
        added = addStrings(num1.integer + '.' + num1.decimal, num2.integer + '.' + num2.decimal, false);
    }

    addedNumbers.integer = added;

    return addedNumbers;
}

int main() {
    string number = "-905.7 -903.8";

    vector<string> splitNumber = splitString(number, ' ');

    Number num1 = toNumber(splitNumber[0]);
    cout << "num1: " << numberToString(num1) << endl;
    Number num2 = toNumber(splitNumber[1]);
    cout << "num2: " << numberToString(num2) << endl;

    cout << "added: " << numberToString(add(num1, num2)) << endl;
    
    return 0;
}