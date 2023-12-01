//Question Lab Task 2 - Final
//1. Write a C/C++ program using the following instructions (filename: ID.cpp):
//a. Read a file named “program.cpp”.
//b. After reading file you have to identify and count the unique name of the user defined functions

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <regex>
#include <map>
using namespace std;

bool isFunctionDeclaration(const string& line, const string& functionNameToExclude) {
    return line.find('(') != string::npos &&
           line.find(')') != string::npos &&
           line.find('{') != string::npos &&
           line.find(functionNameToExclude) == string::npos;
}

int main() {
    ifstream file("program.cpp");
    if (!file.is_open()) {
        cout << "Unable to open the file." << endl;
        return 1;
    }

    string line;
    int functionCount = 0;
    int operatorCount = 0;
    bool insideFunction = false;
    const string functionNameToExclude = "main";

    regex operatorPattern("([+\\-*/=])");
    set<string> printedOperators;
    map<string, string> operatorNames = {
        {"+", "addition"},
        {"-", "subtraction"},
        {"*", "multiplication"},
        {"/", "division"},
        {"=", "assignment"}
    };

    vector<string> functions;
    vector<string> operators;

    while (getline(file, line)) {
        if (isFunctionDeclaration(line, functionNameToExclude)) {
            stringstream ss(line);
            string returnType, functionName;

            ss >> returnType;
            ss >> functionName;

            returnType.erase(remove_if(returnType.begin(), returnType.end(), ::isspace), returnType.end());
            functionName.erase(remove_if(functionName.begin(), functionName.end(), ::isspace), functionName.end());
            functionName.erase(remove_if(functionName.begin(), functionName.end(), ::ispunct), functionName.end());

            functions.push_back("Function-" + to_string(++functionCount) + ": " + functionName);
            functions.push_back("Type- " + returnType);

            insideFunction = true;
        }
        else if (insideFunction && line.find('}') != string::npos) {
            insideFunction = false;
        }
        else {
            smatch match;
            while (regex_search(line, match, operatorPattern)) {
                string matchedOperator = match[0];
                if (printedOperators.find(matchedOperator) == printedOperators.end()) {
                    if (operatorNames.find(matchedOperator) != operatorNames.end()) {
                        operators.push_back("Operator-" + to_string(++operatorCount) + ": " + operatorNames[matchedOperator]);
                        printedOperators.insert(matchedOperator);
                    }
                }
                line = match.suffix();
            }
        }
    }
    for (const string& function : functions) {
        cout << function << endl;
    }
    cout << "Total number of user-defined functions: " << functionCount << endl;

    for (const string& op : operators) {
        cout << op << endl;
    }
    cout << "Total number of operators: " << operatorCount << endl;

    file.close();
    return 0;
}
