/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   PairIterator.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/30 18:21:22 by lsinke        #+#    #+#                 */
/*   Updated: 2023/04/30 18:21:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <iterator>

template<class Iter>
class PairIterator {
	private:
		Iter _it;
		std::size_t _size;

	public:
		PairIterator();
		PairIterator(Iter it, size_t size);
		PairIterator(const PairIterator &other);
		virtual ~PairIterator();

		PairIterator &operator=(const PairIterator &rhs);

		typedef typename Iter::difference_type diff_t;
		typedef typename Iter::value_type value_type;

		// Getters
		const Iter &base() const;
		size_t size() const;

		/**
		 * Swap the pair pointed to by this iterator with the pair pointed to
		 * by the other iterator
		 */
		void swap(PairIterator &other);

		/**
		 * Copy the pair pointed to by this iterator to the output operator dst
		 */
		template<typename OIter>
		void copy(OIter dst);

		// Beware: lots of operator overloads ahead ;'(

		const PairIterator operator++(int);
		const PairIterator operator--(int);
		PairIterator &operator++();
		PairIterator &operator--();
		PairIterator operator+(diff_t n) const;
		PairIterator operator-(diff_t n) const;
		PairIterator &operator+=(diff_t n);
		PairIterator &operator-=(diff_t n);

		diff_t operator-(const PairIterator<Iter> &rhs) const;

		bool operator==(const PairIterator<Iter> &rhs) const;
		bool operator!=(const PairIterator<Iter> &rhs) const;
		bool operator<(const PairIterator<Iter> &rhs) const;
		bool operator>(const PairIterator<Iter> &rhs) const;
		bool operator<=(const PairIterator<Iter> &rhs) const;
		bool operator>=(const PairIterator<Iter> &rhs) const;

		value_type &operator*() const;
		value_type &operator[](diff_t offs) const;

		struct Comp {
			bool operator()(const PairIterator<Iter> &a, const PairIterator<Iter> &b) const;
			bool operator()(const PairIterator<Iter> &it, const int &v) const;
			bool operator()(const int &v, const PairIterator<Iter> &it) const;
		};
};

template<class Iter>
PairIterator<Iter> operator+(typename PairIterator<Iter>::diff_t lhs, const PairIterator<Iter> &rhs);
template<class Iter>
PairIterator<Iter> operator-(typename PairIterator<Iter>::diff_t lhs, const PairIterator<Iter> &rhs);

#include <PairIterator.tpp>
