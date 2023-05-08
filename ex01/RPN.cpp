/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RPN.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/26 01:26:24 by lsinke        #+#    #+#                 */
/*   Updated: 2023/03/26 01:26:24 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <RPN.hpp>
#include <stack>
#include <sstream>
#include <iostream>

// Constructors
RPN::RPN() {}

RPN::RPN(const RPN &other) {
	(void) other;
}

// Destructor
RPN::~RPN() {}

// Operators
RPN &RPN::operator=(const RPN &other) {
	(void) other;
	return *this;
}

static bool useOperator(std::stack<int> &stack, const char op) {
	if (stack.size() < 2) {
		std::cerr << "Error: Stack underflow on operator '" << op << "', ";
		if (stack.empty())
			std::cerr << "Stack is empty\n";
		else
			std::cerr << "Stack: " << stack.top() << "\n";
		return false;
	}
	int a, b;
	a = stack.top(), stack.pop();
	b = stack.top(), stack.pop();
	switch (op) {
		case '+':
			stack.push(b + a);
			return true;
		case '-':
			stack.push(b - a);
			return true;
		case '*':
			stack.push(b * a);
			return true;
		case '/':
			stack.push(b / a);
			return true;
		default:
			std::cerr << "Unexpected character: " << op << "\n";
			return false;
	}
}

static bool is_operator(const char &c) {
	switch (c) {
		case '+':
		case '-':
		case '*':
		case '/':
			return true;
		default:
			std::cerr << "Unexpected character: " << c << '\n';
			return false;
	}
}

void RPN::solve(const std::string &input) {
	std::stack<int> stack;
	std::istringstream stream(input);

	while (!stream.eof()) {
		char c;
		if (!(stream >> c))
			break;
		if (c == ' ') // weird space but ok
			continue;
		if (isdigit(c))
			stack.push(c - '0');
		else if (!is_operator(c) || !useOperator(stack, c))
			return;
		if (stream.peek() != ' ' && !stream.eof())
			std::cerr << "Unexpected character: " << (char) stream.peek() << "\n";
	}
	if (stack.size() == 1)
		std::cout << stack.top() << '\n';
	else {
		std::cerr << "Error: Stack size not 1: stack: ";
		while (!stack.empty()) {
			std::cerr << stack.top() << ' ';
			stack.pop();
		}
		std::cerr << '\n';
	}
}
