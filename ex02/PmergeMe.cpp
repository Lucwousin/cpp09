/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   PmergeMe.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/01 01:26:24 by lsinke        #+#    #+#                 */
/*   Updated: 2023/05/01 01:26:24 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <PmergeMe.hpp>
#include <PairIterator.hpp>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>

PmergeMe::PmergeMe(int argc, char **argv): _size(argc - 1), _values() {
	if (_size == 0)
		throw std::invalid_argument("Not enough values!");
	if (_size > MAX_VALUES)
		throw std::invalid_argument("Too many values!");
	for (int i = 1; i < argc; i++) {
		std::istringstream stream(argv[i]);
		if (!(stream >> _values[i - 1]) || _values[i - 1] < 0) {
			throw std::invalid_argument(argv[i]);
		}
	}
	std::cout << "Before:\t";
	for (size_t i = 0; i < _size; i++) {
		std::cout << _values[i] << ' ';
	}
	std::cout << std::endl;
}

PmergeMe::PmergeMe(const PmergeMe &other): _size(), _values() {
	*this = other;
}

// Destructor
PmergeMe::~PmergeMe() {}

// Operators
PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
	if (this != &other) {
		_size = other._size;
		for (size_t i = 0; i < other._size; i++)
			_values[i] = other._values[i];
	}
	return *this;
}

template<typename Iter, typename OIter>
static void get_sorted(const Iter &begin, const Iter &end, OIter dst) {
	for (Iter cur = begin; cur != end; cur += 2)
		*dst++ = cur;
}

template<typename Container, typename Iter>
static void insert_value(Container &sorted, const Iter value, size_t search_size, size_t &sorted_size) {
	typedef typename Container::iterator s_iter;

	s_iter search;
	if (sorted_size++ <= search_size)
		search = sorted.begin();
	else
		search = sorted.end(), std::advance(search, -search_size);
	s_iter insertion_point = std::lower_bound(search, sorted.end(), *value, typename Iter::Comp());

	sorted.insert(insertion_point, value);
}

static const size_t jacobsthal_lens[] = {
	2u, 2u, 6u, 10u, 22u, 42u, 86u, 170u, 342u, 682u, 1366u,
	2730u, 5462u, 10922u, 21846u, 43690u, 87382u, 174762u, 349526u, 699050u,
	1398102u, 2796202u, 5592406u, 11184810u, 22369622u, 44739242u, 89478486u,
	178956970u, 357913942u, 715827882u, 1431655766u, 2863311530u, 5726623062u,
	11453246122u, 22906492246u, 45812984490u, 91625968982u, 183251937962u,
	366503875926u, 733007751850u, 1466015503702u, 2932031007402u, 5864062014806u,
	11728124029610u, 23456248059222u, 46912496118442u, 93824992236886u, 187649984473770u,
	375299968947542u, 750599937895082u, 1501199875790165u, 3002399751580331u,
	6004799503160661u, 12009599006321322u, 24019198012642644u, 48038396025285288u,
	96076792050570576u, 192153584101141152u, 384307168202282304u, 768614336404564608u,
	1537228672809129216u, 3074457345618258432u, 6148914691236516864u
};

template<typename Container, typename Iter>
static void insert_values(Container &sorted, const Iter &begin, const Iter &end, bool odd) {
	typedef typename Iter::diff_t diff_t; // sneaky typedef

	Iter cur = end - 1;

	// Always add the highest value to the end first, it'll always be larger than everything sorted
	sorted.push_back(cur);

	diff_t values_left = (cur - begin + 1) / 2 - 1;
	if (values_left <= 0 && !odd)
		return;

	size_t sorted_size = sorted.size();

	// After this you would usually perform a binary search only up to the index of the smallest value of every pair
	// I'm settling for the worst-case performance every time though, as inserting into a vector invalidates most iterators
	// and not dealing with this greatly decreases complexity. List iterators also don't even use binary search for
	// std::lower_bound, so the gain there would be minimal as well (if only we could use both containers at the same time...)
	for (int jacob_idx = 0; values_left > 0; jacob_idx++) {
		const size_t search_size = (2 << (jacob_idx + 1)) - 1; // 3, 7, 15, 31 ...

		const diff_t amt_to_insert = std::min(values_left, (diff_t) jacobsthal_lens[jacob_idx]);

		cur -= amt_to_insert * 2;
		values_left -= amt_to_insert;

		Iter tmp = cur;
		for (diff_t i = 0; i < amt_to_insert; i++) {
			insert_value(sorted, tmp, search_size, sorted_size);
			tmp += 2;
		}
	}

	// Finally add the last value if there was one
	if (odd)
		insert_value(sorted, end, sorted_size, sorted_size);
}

template<typename Container, typename Sorted>
static void move_values(Sorted &sorted, Container &dst) {
	typedef typename Sorted::iterator iter;

	for (iter it = sorted.begin(); it != sorted.end(); it++)
		it->copy(std::back_inserter(dst));
}

template<typename Iter>
static bool sort_pairs(const Iter &begin, const Iter &end) {
	Iter a, b;

	for (a = begin, b = a + 1; a != end && b != end; a += 2, b += 2) {
		if (*a > *b)
			a.swap(b);
	}
	return (b == end);
}

template<template <typename, typename> class Container, typename Iter>
void PmergeMe::mergeInsert(Container<int, std::allocator<int> > &container, const Iter &begin, const Iter &end) {

	if (end == begin + 1)
		return;

	// By viewing the container through a "PairIterator" we've pretty much split
	// it up into <length> values. The iterator only gets the first value in the
	// <pair_size> sized chunks of container, but it is able to swap all of them
	// I totally did not get this idea from https://codereview.stackexchange.com/questions/116367/ford-johnson-merge-insertion-sort

	// Sort all pairs (this returns oddness as checking the length of a std::list is linear
	const bool odd = sort_pairs(begin, end);
	const Iter actual_end = end - odd;

	// Recursively sort the smaller value of each pair (by just widening our "view")
	mergeInsert(container,
			Iter(begin.base(), begin.size() * 2),
			Iter(actual_end.base(), begin.size() * 2));

	// Get all sorted values...
	Container<Iter, std::allocator<Iter> > sorted;
	get_sorted(begin, actual_end, std::back_inserter(sorted));

	// And now insert the values that are matched with the sorted values
	insert_values(sorted, begin, actual_end, odd);

	// Now move all the values to a temporary container and back into the main one
	// we can't just assign to container, as sorted does not contain any "odd" values
	Container<int, std::allocator<int> > tmp;
	move_values(sorted, tmp);
	std::copy(tmp.begin(), tmp.end(), container.begin());
}

// need to add ft_ as cluster macs have a weird is_sorted version which algorithms lib on linux does not
template<typename Iter>
static bool ft_is_sorted(Iter begin, Iter end) {
	for (Iter a = begin, b = ++begin; b != end; ++a, ++b) {
		if (*a > *b) {
			return false;
		}
	}
	return true;
}

template<class Container>
clock_t PmergeMe::sortContainer(Container &c) {
	const clock_t start = clock();

	for (size_t i = 0; i < _size; i++)
		c.push_back(_values[i]);

	typedef typename Container::iterator Iter;
	mergeInsert(c, PairIterator<Iter>(c.begin(), 1), PairIterator<Iter>(c.end(), 1));

	const clock_t duration = clock() - start;
	if (!ft_is_sorted(c.begin(), c.end()))
		std::cerr << "Failed to sort container!\n";

	return duration;
}

static void print_time(const std::string &str, clock_t cycles) {
	static const long CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000;
	static const long CLOCKS_PER_US = CLOCKS_PER_MS / 1000;
	const clock_t us = cycles / CLOCKS_PER_US;

	std::cout << str << us << "µs!\n";
}

void PmergeMe::sort() {
	std::vector<int> vec;
	std::list<int> lst;
	std::deque<int> deq;

	clock_t vec_time = sortContainer(vec);
	clock_t lst_time = sortContainer(lst);
	clock_t deq_time = sortContainer(deq);

	std::cout << "Sorted: ";
	for (std::vector<int>::iterator i = vec.begin(); i < vec.end(); i++)
		std::cout << *i << ' ';
	std::cout << std::endl;

	print_time("Vector duration: ", vec_time);
	print_time("List duration:   ", lst_time);
	print_time("Deque duration:  ", deq_time);
}
