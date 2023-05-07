/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/30 18:21:22 by lsinke        #+#    #+#                 */
/*   Updated: 2023/04/30 18:21:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <PmergeMe.hpp>
#include <PairIterator.hpp>
#include <iostream>
#include <vector>

void test_iterator() {
	std::vector<int> vec;

	vec.reserve(256);
	for (int i = 0; i < 256; i++)
		vec.push_back(i);

	for (int i = 0; i < 5; i++) {
		std::cout << "Testing PairIterator of size " << (1 << i) << '\n';
		PairIterator<std::vector<int>::iterator> it(vec.begin(), 1 << i);
		PairIterator<std::vector<int>::iterator> end(vec.end(), 1 << i);
		for (; it < end; it++)
			std::cout << *it << '\t';
		std::cout << '\n';
	}
}

int main(int argc, char **argv) {
	// test_iterator();
	try {
		PmergeMe m = PmergeMe(argc, argv);
		m.sort();
	} catch (const std::invalid_argument &err) {
		std::cerr << "Invalid argument: " << err.what() << std::endl;
	}
}