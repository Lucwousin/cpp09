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

#include <vector>
#include <deque>
#include <ctime>

#define MAX_VALUES 50000

class PmergeMe {
	private:
		PmergeMe(); // Not implemented, can't have no values

		size_t _size;
		int _values[MAX_VALUES];
		std::vector<int> _vec;
		std::deque<int> _deq;

		void	mergeInsertVec(size_t length, size_t pair_size);
		clock_t sortVec();
		clock_t sortDeq();

	public:
		PmergeMe(int argc, char **argv);
		PmergeMe(const PmergeMe &other);
		~PmergeMe();
		PmergeMe &operator=(const PmergeMe &other);

		void sort();
};

typedef std::vector<int>::iterator VecIter;
