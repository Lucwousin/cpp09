/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   PmergeMe.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/01 01:26:24 by lsinke        #+#    #+#                 */
/*   Updated: 2023/05/01 01:26:24 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <memory>

#define MAX_VALUES 25000

class PmergeMe {
	private:
		PmergeMe(); // Not implemented, can't have no values

		size_t _size;
		int _values[MAX_VALUES];

		template<class Container>
		clock_t sortContainer(Container &c);

		template<template <typename, typename> class Container, typename Iter>
		void mergeInsert(Container<int, std::allocator<int> > &container, const Iter &begin, const Iter &end);

	public:
		PmergeMe(int argc, char **argv);
		PmergeMe(const PmergeMe &other);
		~PmergeMe();
		PmergeMe &operator=(const PmergeMe &other);

		void sort();
};
