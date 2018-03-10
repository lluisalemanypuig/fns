#pragma once

/// C includes
#include <assert.h>
#include <gmp.h>

/// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/// Custom includes
#include "integer.hpp"

class factoradic {
	private:
		vector<ushort> radixs;
		// neg = true -> this number is negative
		// neg = false -> this number is positive
		bool neg;
		
		// accumulate to this positive number the positive number f
		void accumulate(const factoradic& f);
		
		// substract from this positive number the poisitve number f
		// precondition: *this > f, f >= 0
		void substract(const factoradic& f);
		
	public:
		factoradic();
		// converts the number in decimal base d into factorial number system
		factoradic(int d);
		factoradic(const integer& d);
		factoradic(const string& d);
		~factoradic();
		
		/// OPERATORS
		
		inline friend
		ostream& operator<< (ostream& os, const factoradic& f) {
			os << f.to_string();
			return os;
		}
		
		factoradic& operator= (const factoradic& f);
		
		factoradic operator+ (const factoradic& f) const;
		factoradic& operator+= (const factoradic& f);
		
		factoradic operator-() const;
		factoradic& operator-();
		factoradic operator- (const factoradic& f) const;
		factoradic& operator-= (const factoradic& f);
		
		factoradic operator* (const factoradic& f) const;
		factoradic& operator*= (const factoradic& f);
		
		factoradic operator/ (const factoradic& f) const;
		factoradic& operator/= (const factoradic& f);
		
		// ad-hoc algorithm for dividing by 2
		void halve();
		
		bool operator== (int k) const;
		bool operator== (integer k) const;
		bool operator== (const factoradic& k) const;
		
		bool operator> (int k) const;
		bool operator> (integer k) const;
		bool operator> (const factoradic& k) const;
		
		bool operator>= (int k) const;
		bool operator>= (integer k) const;
		bool operator>= (const factoradic& k) const;
		
		bool operator< (int k) const;
		bool operator< (integer k) const;
		bool operator< (const factoradic& k) const;
		
		bool operator<= (int k) const;
		bool operator<= (integer k) const;
		bool operator<= (const factoradic& k) const;
		
		bool is_negative() const;
		
		// returns true if there exists some integer k such that
		// 2*k = this number
		bool is_even() const;
		
		/// GETTERS
		
		void get_radixs(vector<ushort>& rs, size_t n_digits = 0) const;
		
		/// CONVERSIONS
		
		void from_decimal(int n);
		void from_decimal(const integer& i);
		
		// build the number in factoradic system for the value n!
		void from_factorial(size_t n);
		
		integer to_decimal() const;
		void to_decimal(integer& i) const;
		
		// use, at least, n_digits to represent this number
		// n_digits = 0 is interpreted as using the minimum amount of
		// digits.
		string to_string(size_t n_digits = 0) const;
		void to_string(string& s, size_t n_digits = 0) const;
		
};

inline static
void swap(factoradic& f1, factoradic& f2) {
	factoradic copy = f1;
	f1 = f2;
	f2 = copy;
}

