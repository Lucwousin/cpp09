/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   PairIterator.tpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/30 18:21:22 by lsinke        #+#    #+#                 */
/*   Updated: 2023/04/30 18:21:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <list>

// Ctors/dtors and all that

template<class Iter>
PairIterator<Iter>::PairIterator(): _it(), _size() {}

template<class Iter>
PairIterator<Iter>::PairIterator(Iter it, size_t size): _it(it), _size(size) {}

template<class Iter>
PairIterator<Iter>::PairIterator(const PairIterator &other): _it(other._it), _size(other._size) {}

template<class Iter>
PairIterator<Iter>::~PairIterator() {}

template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator=(const PairIterator &rhs) {
	if (*this == rhs)
		return *this;
	_it = rhs._it;
	_size = rhs._size;
	return *this;
}

// Getters

template<class Iter>
const Iter &PairIterator<Iter>::base() const {
	return _it;
}

template<class Iter>
size_t PairIterator<Iter>::size() const {
	return _size;
}

// Useful functions

template<class Iter>
void PairIterator<Iter>::swap(PairIterator<Iter> &other) {
	Iter end = _it;
	std::advance(end, _size);
	std::swap_ranges(_it, end, other._it);
}

template<class Iter>
template<typename OIter>
void PairIterator<Iter>::copy(OIter dst) {
	Iter end = _it;
	std::advance(end, _size);
	std::copy(_it, end, dst);
}

// Operator overloads ahead

template<class Iter>
const PairIterator<Iter> PairIterator<Iter>::operator++(int) {
	PairIterator temp = *this;
	std::advance(_it, _size);
	return temp;
}

template<class Iter>
const PairIterator<Iter> PairIterator<Iter>::operator--(int) {
	PairIterator temp = *this;
	std::advance(_it, - (diff_t) _size);
	return temp;
}

template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator++() {
	std::advance(_it, _size);
	return *this;
}


template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator--() {
	std::advance(_it, - (diff_t) _size);
	return *this;
}

template<class Iter>
PairIterator<Iter> PairIterator<Iter>::operator+(diff_t n) const {
	Iter tmp = _it;
	std::advance(tmp, n * (diff_t) _size);
	return PairIterator(tmp, _size);
}

template<class Iter>
PairIterator<Iter> PairIterator<Iter>::operator-(diff_t n) const {
	return operator+(-n);
}

template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator+=(diff_t n) {
	*this = operator+(n);
	return *this;
}

template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator-=(diff_t n) {
	return *this += -n;
}

template<class Iter>
typename PairIterator<Iter>::diff_t PairIterator<Iter>::operator-(const PairIterator<Iter> &rhs) const {
	return ((_it - rhs._it) / _size);
}

template<class Iter>
bool PairIterator<Iter>::operator==(const PairIterator<Iter> &rhs) const {
	return _size == rhs._size && _it == rhs._it;
}

template<class Iter>
bool PairIterator<Iter>::operator!=(const PairIterator<Iter> &rhs) const {
	return _size != rhs._size || _it != rhs._it;
}

template<class Iter>
bool PairIterator<Iter>::operator<(const PairIterator<Iter> &rhs) const {
	return _it < rhs._it;
}

template<class Iter>
bool PairIterator<Iter>::operator>(const PairIterator<Iter> &rhs) const {
	return _it > rhs._it;
}

template<class Iter>
bool PairIterator<Iter>::operator<=(const PairIterator<Iter> &rhs) const {
	return _it <= rhs._it;
}

template<class Iter>
bool PairIterator<Iter>::operator>=(const PairIterator<Iter> &rhs) const {
	return _it >= rhs._it;
}

template<class Iter>
typename PairIterator<Iter>::value_type &PairIterator<Iter>::operator*() const {
	return *_it;
}

template<class Iter>
typename PairIterator<Iter>::value_type &PairIterator<Iter>::operator[](diff_t offs) const {
	return _it[offs * _size];
}

template<typename Iter>
PairIterator<Iter> operator+(typename PairIterator<Iter>::diff_t lhs, const PairIterator<Iter> &rhs) {
	return rhs + lhs;
}

template<typename Iter>
PairIterator<Iter> operator-(typename PairIterator<Iter>::diff_t lhs, const PairIterator<Iter> &rhs) {
	return rhs + -lhs;
}

// Comparison object functions (for lower/upper_bounds)

template<class Iter>
bool PairIterator<Iter>::Comp::operator()(const PairIterator<Iter> &a, const PairIterator<Iter> &b) const {
	return *a < *b;
}

template<class Iter>
bool PairIterator<Iter>::Comp::operator()(const PairIterator<Iter> &it, const int &v) const {
	return *it < v;
}

template<class Iter>
bool PairIterator<Iter>::Comp::operator()(const int &v, const PairIterator<Iter> &it) const {
	return v < *it;
}


// Specializations for sequential iterator

typedef PairIterator<std::list<int>::iterator> SeqPIter;

// warning: do not try to subtract a bigger iterator from a smaller one
// also lol linear
template<>
inline SeqPIter::diff_t SeqPIter::operator-(const SeqPIter &rhs) const {
	diff_t n = 0;
	for (SeqPIter tmp = *this; rhs != tmp; --tmp) {
		++n;
	}
	return n;
}

template<>
inline void SeqPIter::swap(SeqPIter &other) {
	std::list<int>::iterator a = _it, b = other._it;
	for (size_t n = _size; n > 0; n--)
		std::iter_swap(a++, b++);
}
