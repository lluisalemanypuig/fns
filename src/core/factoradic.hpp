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
		
		void increment();	// increment this number by 1
		void decrement();	// decrement this number by 1
		
		// accumulate to this positive number the positive number f
		void __accumulate(const factoradic& f);
		template<class T> void accumulate(const T& f);
		
		// substract from this positive number the poisitve number f
		// precondition: *this > f, f >= 0
		void __substract(const factoradic& f);
		template<class T> void substract(const T& f);
		
		// fast multiplication algorithm
		template<class T> void integer_fast_multiply(const T& i);
		void factoradic_fast_multiply(const factoradic& f);
		
		// converts this factoradic number to base 10
		template<class T> void __to_decimal(T& i) const;
		
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
		
		factoradic operator+ (int i) const;
		factoradic operator+ (const integer& i) const;
		factoradic operator+ (const factoradic& f) const;
		factoradic& operator+= (int i);
		factoradic& operator+= (const integer& i);
		factoradic& operator+= (const factoradic& f);
		
		factoradic operator-() const;
		factoradic& operator-();
		factoradic operator- (int i) const;
		factoradic operator- (const integer& i) const;
		factoradic operator- (const factoradic& f) const;
		factoradic& operator-= (int i);
		factoradic& operator-= (const integer& i);
		factoradic& operator-= (const factoradic& f);
		
		factoradic operator* (int i) const;
		factoradic operator* (const integer& i) const;
		factoradic operator* (const factoradic& f) const;
		factoradic& operator*= (int i);
		factoradic& operator*= (const integer& i);
		factoradic& operator*= (const factoradic& f);
		
		factoradic operator/ (const factoradic& f) const;
		factoradic& operator/= (const factoradic& f);
		
		bool operator== (int i) const;
		bool operator== (const integer& i) const;
		bool operator== (const factoradic& f) const;
		
		bool operator> (int i) const;
		bool operator> (const integer& i) const;
		bool operator> (const factoradic& f) const;
		
		bool operator>= (int i) const;
		bool operator>= (const integer& i) const;
		bool operator>= (const factoradic& f) const;
		
		bool operator< (int i) const;
		bool operator< (const integer& i) const;
		bool operator< (const factoradic& f) const;
		
		bool operator<= (int i) const;
		bool operator<= (const integer& i) const;
		bool operator<= (const factoradic& f) const;
		
		factoradic& operator++ ();		// prefix:	++a
        factoradic operator++ (int);	// postfix:	a++
        
        factoradic& operator-- ();		// prefix:	++a
        factoradic operator-- (int);	// postfix:	a++
		
		/// MODIFIERS
		
		void mult2();	// ad-hoc algorithm for multiplying by 2
		void div2();	// ad-hoc algorithm for dividing by 2
		
		/// GETTERS
		
		bool is_one() const;		// returns true if this number is one
		bool is_zero() const;		// returns true if this number is zero
		bool is_negative() const;	// returns true if this number is negative (<0)
		bool is_even() const;		// returns true if there exists some integer k such that 2*k = *this
		
		void get_radixs(vector<ushort>& rs, size_t n_digits = 0) const;
		
		/// CONVERSIONS
		
		// convert to factorial number system any number in base 10
		template<class T> void from_decimal(const T& x);
		
		// build the number in factoradic system for the value n!
		void from_factorial(size_t n);
		
		integer to_decimal() const;
		void to_decimal(integer& i) const;
		int to_small_decimal() const;
		
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

inline static
void swap(integer& f1, factoradic& f2) {
	factoradic F;
	F.from_decimal(f1);
	swap(F, f2);
}

inline static
void swap(factoradic& f1, integer& f2) {
	factoradic F;
	F.from_decimal(f2);
	swap(f1, F);
}

#include "factoradic_template.cpp"
