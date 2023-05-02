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
#include <list>
#include <ctime>
#include <PairIterator.hpp>

#define MAX_VALUES 50000

typedef std::vector<int>::iterator VecIter;
typedef PairIterator<VecIter> VecPIter;

typedef std::list<int>::iterator ListIter;
typedef PairIterator<ListIter> ListPIter;

class PmergeMe {
	private:
		PmergeMe(); // Not implemented, can't have no values

		size_t _size;
		int _values[MAX_VALUES];
		std::vector<int> _vec;
		std::deque<int> _deq;

		template<class Container>
		clock_t sortContainer(Container &c);

		void mergeInsert(const VecPIter &begin, const VecPIter &end);
		void mergeInsert(const ListPIter &begin, const ListPIter &end);

	public:
		PmergeMe(int argc, char **argv);
		PmergeMe(const PmergeMe &other);
		~PmergeMe();
		PmergeMe &operator=(const PmergeMe &other);

		void sort();
};
