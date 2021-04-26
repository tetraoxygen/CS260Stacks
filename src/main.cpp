#include <iostream>
#include <stack>
#include <string>
#include <regex>
#include <stdexcept>


int precedence(char op) {
	switch (op) {
		case '(':
			return 0;
			break;
		case ')':
			return 0;
			break;
		case '*':
			return 2;
			break;
		case '/':
			return 2;
			break;
		case '+':
			return 1;
			break;
		case '-':
			return 1;
			break;
		default:
			return 0;
			break;
	}
	throw std::invalid_argument("precedence() called on a non-operator");
}

bool cleanInfix(std::string& str) {
	// if there's an invalid character, give an error and stop executing this (original regex: "[0-9 () + * \- \/  \s]*")
	if (!std::regex_match(str, std::regex("[0-9 ( ) + * \\- \\/  \\s]*"))) {
		str = "ERROR";
		return false;
	}

	// strip the whitespace
	str.erase(remove(str.begin(),str.end(),' '), str.end());
	return true;
}

bool convertToPostfix(std::string& str) {
	std::stack<char> holdingStack;
	std::string result;
	
	for (int i = 0; i < str.length(); i++) {
		// if (!holdingStack.empty()) {
		// 	std::cout << "holding stack top: " << holdingStack.top() << std::endl; //" Precedence: " << precedence(holdingStack.top()) << std::endl;
		// }
				
		if (isdigit(str[i])) {
			result.push_back(str[i]);
		} else if (str[i] == '(') {
			holdingStack.push(str[i]);
		} else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') {
			if (!holdingStack.empty()) {
				while ((precedence(holdingStack.top()) >= precedence(str[i]))) {
					result.push_back(holdingStack.top());
					holdingStack.pop();
					
					if (holdingStack.empty()) {
						break;
					}
				}
			}
			holdingStack.push(str[i]);	
		} else if (str[i] == ')') {
			while (!holdingStack.empty()) {
				if (holdingStack.top() == '(') {
					break;
				}
				
				result.push_back(holdingStack.top());
				holdingStack.pop();
			}
			// discard the left parenthesis
			holdingStack.pop();
		}
	}
	while (!holdingStack.empty()) {
		result.push_back(holdingStack.top());
		holdingStack.pop();
	}
	str = result;
	return true;
}

bool evaluatePostfix(std::string& str) {
	std::stack<int> holdingStack;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') { // Operator
			try {
				if (holdingStack.empty()) {
					str = "ERROR";
					return false;
				}
				int op1 = holdingStack.top();
				
				
				holdingStack.pop();
				
				if (holdingStack.empty()) {
					str = "ERROR";
					return false;
				}
				int op2 = holdingStack.top();
				
				
				holdingStack.pop();
				
				
				switch (str[i]) {
					case '*':
						holdingStack.push(op2 * op1);
						break;
					case '/':
						holdingStack.push(op2 / op1);
						break;
					case '+':
						holdingStack.push(op2 + op1);
						break;
					case '-':
						holdingStack.push(op2 - op1);
						break;
				}
			} catch (...) {
				str = "ERROR";
				return false;
			}
		} else { // Operand
			holdingStack.push(str[i] - '0');
		}
	}
	str = std::to_string(holdingStack.top());
	return true;
}

bool processEquation (std::string str) {
	std::cout << "Processing:\t" << str << std::endl;
	
	std::cout << "Infix:\t\t";
	bool clean = cleanInfix(str);
	std::cout << str << std::endl;
	if (!clean) {
	std::cout << std::endl;
		return false;
	}
	
	std::cout << "Postfix:\t";
	bool conversion = convertToPostfix(str);
	std::cout << str << std::endl;
	if (!conversion) {
		std::cout << std::endl;
		return false;
	}
	
	std::cout << "Result:\t\t";
	bool result = evaluatePostfix(str);
	std::cout << str << std::endl;
	if (!result) {
		std::cout << std::endl;
		return false;
	}
	
	std::cout << std::endl;
	return true;
}

int main() {
	std::string eq1 = "5 # 2";
	std::string eq2 = "5 * - 6 8";
	std::string eq3 = "5*2  +   3";
	std::string eq4 = "3 + 5 * (7-2)";
	std::string eq5 = "3 + ( (8-5) * (4+9) )";
	
	processEquation(eq1);
	processEquation(eq2);
	processEquation(eq3);
	processEquation(eq4);
	processEquation(eq5);
}
