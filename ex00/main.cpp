/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/26 01:25:56 by lsinke        #+#    #+#                 */
/*   Updated: 2023/03/26 01:25:56 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <BitcoinExchange.hpp>
#include <iostream>

void test() {
	std::cerr << "\nTesting nonexisting db and nonexisting input" << '\n';
	try {
		BitcoinExchange exchange("invalid");
	} catch (std::ios_base::failure const &ex) {
		std::cerr << "Error: " << ex.what() << '\n';
	}
	try {
		BitcoinExchange exchange("data.csv");
		exchange.applyExchangeRate("invalid");
	} catch (std::ios_base::failure const &ex) {
		std::cerr << "Error: " << ex.what() << '\n';
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./btc <input>\n";
		return 1;
	}

	try {
		BitcoinExchange exchange("data.csv");
		exchange.applyExchangeRate(argv[1]);
	} catch (std::ios_base::failure const &ex) {
		std::cerr << "Error: " << ex.what() << '\n';
	}

	// test();
}
