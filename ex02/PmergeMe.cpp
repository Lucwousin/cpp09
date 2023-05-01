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

PmergeMe::PmergeMe(int argc, char **argv): _size(argc - 1), _values() {
	if (_size > MAX_VALUES + 1)
		throw std::invalid_argument("Too many values!\n");
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
		_vec = other._vec;
		_deq = other._deq;
		for (size_t i = 0; i < other._size; i++)
			_values[i] = other._values[i];
	}
	return *this;
}

template<typename Iter>
static void sort_pairs(const PairIterator<Iter> &begin, const PairIterator<Iter> &end) {
	for (PairIterator<Iter> a = begin, b = a + 1; b < end; a += 2, b += 2)
		if (*a > *b)
			a.swap(b);
}

template<typename Iter>
static std::vector<PairIterator<Iter> > get_sorted(const PairIterator<Iter> &begin, const PairIterator<Iter> &end) {
	std::vector<PairIterator<Iter> > sorted;
	sorted.reserve(end - begin);

	for (PairIterator<Iter> cur = begin; cur < (end - 1); cur += 2)
		sorted.push_back(cur);

	return sorted;
}

template<class SortedType>
static void insert_value(SortedType &sorted, const PairIterator<VecIter> value, size_t search_size) {
	typedef typename SortedType::iterator s_iter;

	s_iter search_start = (search_size >= sorted.size()) ? sorted.begin() : sorted.end() - search_size;
	s_iter insertion_point = std::lower_bound(search_start, sorted.end(), *value, PairIterator<VecIter>::Comp());

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

template<typename SortedType>
static void insert_values(SortedType &sorted, const PairIterator<VecIter> &begin, const PairIterator<VecIter> &end, bool odd) {
	typedef PairIterator<VecIter>::diff_t diff_t; // sneaky typedef

	PairIterator<VecIter> cur = end - 1;

	if (cur <= begin)
		return;
	// Always add the highest value to the end first, it'll always be larger than everything sorted
	sorted.push_back(cur);

	// After this you would usually perform a binary search only up to the index of the smallest value of every pair
	// I'm settling for the worst-case performance every time though, as inserting into a vector invalidates most iterators
	// and not dealing with this greatly decreases complexity
	for (int jacob_idx = 0; cur > begin + 1; jacob_idx++) {
		const size_t search_size = (2 << (jacob_idx + 1)) - 1; // 3, 7, 15, 31 ...
		const size_t values_left = (cur - begin + 1) / 2 - 1;

		const diff_t amt_to_insert = (diff_t) std::min(values_left, jacobsthal_lens[jacob_idx]);

		cur -= amt_to_insert * 2;

		for (diff_t i = 0; i < amt_to_insert; i++)
			insert_value(sorted, cur + 2 * i, search_size);
	}

	// Finally add the last value if there was one
	if (odd)
		insert_value(sorted, end, sorted.size());
}

static std::vector<int> move_values(std::vector<PairIterator<VecIter> > &sorted, const size_t size) {
	std::vector<int> temp;
	temp.reserve(size);

	for (std::vector<PairIterator<VecIter> >::iterator it = sorted.begin(); it < sorted.end(); it++)
		it->copy(std::back_inserter(temp));

	return temp;
}

void PmergeMe::mergeInsertVec(size_t length, size_t pair_size) {
	const bool odd = length % 2;

	if (length < 2)
		return;

	const PairIterator<VecIter> begin(_vec.begin(), pair_size);
	const PairIterator<VecIter> end = begin + (PairIterator<VecIter>::diff_t) (length - odd);

	// By viewing the container through a "PairIterator" we've pretty much split
	// it up into <length> values. The iterator only gets the first value in the
	// <pair_size> sized chunks of container, but it is able to swap all of them
	// I totally did not get this idea from https://codereview.stackexchange.com/questions/116367/ford-johnson-merge-insertion-sort

	// Sort these pairs
	sort_pairs(begin, end);

	// Recursively sort the smaller value of each pair (by just widening our "view")
	mergeInsertVec(length / 2, pair_size * 2);

	// Time to add all recursively sorted values to a sorted array
	std::vector<PairIterator<VecIter> > sorted = get_sorted(begin, end);

	// And now insert the values that are matched with the sorted values
	insert_values(sorted, begin, end, odd);

	// Now move all the values to a temporary array and back into the main vector
	// we can't just assign to _vec, as sorted does not contain any "odd" values
	std::vector<int> tmp = move_values(sorted, _vec.size());
	std::copy(tmp.begin(), tmp.end(), _vec.begin());
}

clock_t PmergeMe::sortVec() {
	std::clock_t start = clock();

	_vec.reserve(_size);
	for (size_t i = 0; i < _size; i++)
		_vec.push_back(_values[i]);

	mergeInsertVec(_size, 1);

	return clock() - start;
}

static void print_time(const std::string &str, clock_t cycles) {
	static const long CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000;
	static const long CLOCKS_PER_US = CLOCKS_PER_MS / 1000;
	const clock_t us = cycles / CLOCKS_PER_US;

	std::cout << str << us << "µs!\n";
}

void PmergeMe::sort() {
	clock_t vec_time = sortVec();
	//clock_t deq_time = sortDeq();

	/*if (!std::is_sorted(_vec.begin(), _vec.end()))
		std::cerr << "Something went wrong when sorting with a vector - it's not sorted!\n";*/

	std::cout << "Sorted: ";
	for (VecIter i = _vec.begin(); i < _vec.end(); i++)
		std::cout << *i << ' ';
	std::cout << std::endl;

	print_time("Vector duration: ", vec_time);
}
