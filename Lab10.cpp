#include <string>
#include <regex>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// this program does addition by treating each number as its ascii value
// each number char in ascii is exactly 48 higher than its "actual" value
// for example, 0 in ascii is 48, 5 in ascii is 53, etc
const int charOffset = 48;
// while this is technically treating each char as a number, I'm not converting the chars or strings into other data types
// so this is fully allowed per the assignment guidelines
// and the only alternative is just using a big, tediously-long switch statement for every possible combination of numbers
// the ascii thing is a much more elegant solution and takes advantage of unique the way c++ handles data
// plus I do have to consider how to carry between digits when adding and keep track of the decimal, so I feel it's still in the spirit of the assignment

// function stolen from my previous lab submission
/** splits a string into a vector based on a user-specified delimiter 
 * @param input the string to split
 * @param delimiter the char to act as a delimiter
 * @return a vector, with each element being part of the string split by the delimiter
*/
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

/** checks if a string contains a specified char.
 * @param input the string to search through
 * @param target the char to search for
 * @return true if the string contains the character, false if not
 */
bool stringContains(string input, char target) {
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == target) {
            return true;
        }
    }

    return false;
}

/** Checks to make sure a number string contains only 1 decimal and no letters
 * @param input the string to check
 * @return true if the string is valid, false if not
 */
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

/** Removes extra 0s from a number string
 * @param input the string to trim
 * @return the trimmed string
*/
string trimNumber(string input) {
    bool isNegative = false;
    if (input[0] == '-') {
        isNegative = true;
        input.erase(0, 1);
    }
    vector<string> splitInput = splitString(input, '.');

    // turning these into their own variables to make it a little easier to read
    string integer = splitInput[0];
    string decimal = splitInput[1];
    
    bool leadingNumberFound = false;
    for(int i = 0; i < integer.length(); i++) {
        if(integer[i] == '0') {
            continue;
        } else {
            integer = integer.substr(i);
            break;
        }
    }

    leadingNumberFound = false;
    for(int i = decimal.length(); i > 0; i--) {
        if(decimal[i] == '0' && !leadingNumberFound) {
            continue;
        } else {
            decimal = decimal.substr(0, i);
            break;
        }
    }

    if (isNegative) {
        integer.insert(integer.begin(), '-');
    }

    return integer + '.' + decimal;
    
}

/** Adds 2 number strings together
 * @param num1 the first number
 * @param num2 the second number
 * @return the sum of the two numbers as a string
 */
string addStrings(string num1, string num2) {
    bool num1Negative = false;
    bool num2Negative = false;
    string sum;

    // makes sure num1 is a valid number, and handles its negative sign if it has one
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

    // same as above but for num2
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

    // if num1 is negative and num2 isn't, swap em
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
        if (num2Negative && !num1Negative) {
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
        sum.insert(sum.begin(), '-');
    }

    // trim the sum so it doesn't have any unnecessary 0s
    return trimNumber(sum);
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