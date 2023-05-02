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
	std::swap_ranges(_it, _it + _size, other._it);
}

template<class Iter>
template<typename OIter>
void PairIterator<Iter>::copy(OIter dst) {
	std::copy(base(), _it + _size, dst);
}

// Operator overloads ahead

template<class Iter>
const PairIterator<Iter> PairIterator<Iter>::operator++(int) {
	PairIterator temp = *this;
	_it += _size;
	return temp;
}

template<class Iter>
const PairIterator<Iter> PairIterator<Iter>::operator--(int) {
	PairIterator temp = *this;
	_it -= _size;
	return temp;
}

template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator++() {
	_it += _size;
	return *this;
}


template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator--() {
	_it -= _size;
	return *this;
}

template<class Iter>
PairIterator<Iter> PairIterator<Iter>::operator+(diff_t n) const {
	return PairIterator(_it + n * _size, _size);
}

template<class Iter>
PairIterator<Iter> PairIterator<Iter>::operator-(diff_t n) const {
	return PairIterator(_it - n * _size, _size);
}

template<class Iter>
PairIterator<Iter> &PairIterator<Iter>::operator+=(diff_t n) {
	_it += n * _size;
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

template<>
inline const SeqPIter SeqPIter::operator++(int) {
	PairIterator temp = *this;
	for (size_t n = _size; n > 0; n--)
		_it++;
	return temp;
}

template<>
inline const SeqPIter SeqPIter::operator--(int) {
	PairIterator temp = *this;
	for (size_t n = _size; n > 0; n--)
		_it--;
	return temp;
}

template<>
inline SeqPIter &SeqPIter::operator++() {
	for (size_t n = _size; n > 0; n--)
		_it++;
	return *this;
}

template<>
inline SeqPIter &SeqPIter::operator--() {
	for (size_t n = _size; n > 0; n--)
		_it--;
	return *this;
}

template<>
inline SeqPIter SeqPIter::operator+(diff_t n) const {
	PairIterator temp = *this;

	if (n < 0)
		while (n++ != 0)
			temp++;
	else
		while (n-- != 0)
			temp--;

	return temp;
}

template<>
inline SeqPIter SeqPIter::operator-(diff_t n) const {
	return *this + (-n);
}

template<>
inline SeqPIter &SeqPIter::operator+=(diff_t n) {
	return (*this = *this + n);
}

template<>
inline SeqPIter &SeqPIter::operator-=(diff_t n) {
	return (*this = *this - n);
}

template<>
inline void SeqPIter::swap(SeqPIter &other) {
	std::list<int>::iterator a = _it, b = other._it;
	for (size_t n = _size; n > 0; n--)
		std::iter_swap(a++, b++);
}

/*
// Ew, don't look at this! We need this to make a PairIterator for list actually implement this overload..
template<>
PairIterator<std::list<int>::iterator>::diff_t PairIterator<std::list<int>::iterator>::operator-(const PairIterator<std::list<int>::iterator> &rhs) const {
	typedef PairIterator<std::list<int>::iterator> iter_t;
	diff_t diff = 0;

	for (iter_t it = *this; it != rhs; it--) {
		++diff;
	}
}*/
