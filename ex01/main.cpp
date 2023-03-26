/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/26 01:26:14 by lsinke        #+#    #+#                 */
/*   Updated: 2023/03/26 01:26:14 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <RPN.hpp>
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./RPN <input>\n";
		return 1;
	}

	RPN rpn;
	rpn.solve(argv[1]);
	//"8 9 * 9 - 9 - 9 - 4 - 1 +"
}
