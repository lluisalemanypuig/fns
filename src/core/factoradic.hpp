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
		// radixs of this number: little-endian format
		vector<size_t> radixs;
		
		// neg = true -> this number is negative
		// neg = false -> this number is positive
		bool neg;
		
		/// SINGLE INCREMENTS/DECREMENTS
		void __increment();	// increment by 1 a positive number (> 0)
		void __decrement();	// decrement by 1 a positive number (> 0)
		void increment();	// increment this number by 1
		void decrement();	// decrement this number by 1
		
		/// ACCUMULATE
		// accumulate to this positive number the positive number f
		void __accumulate(const factoradic& f);
		void accumulate(const factoradic& f);
		
		/// SUBSTRACT
		// substract from this positive number the poisitve number f
		// precondition: *this > f, f >= 0
		void __substract(const factoradic& f);
		void substract(const factoradic& i);
		
		/// MULTIPLY
		// fast multiplication algorithm
		template<class T> void integer_multiply(const T& i); // i: decimal
		void factoradic_multiply(const factoradic& f);
		
		/// DIVIDE
		// divides this number by 'i' (decimal number)
		void int_divide(size_t i);
		void integer_divide(const integer& i);
		
		/// POWER
		// fast exponentiation algorithm
		template<class T> void integer_power(const T& i); // i: decimal
		void factoradic_power(const factoradic& f);
		
		/// FROM/TO BASE-10 CONVERSIONS
		// converts this factoradic number to base 10
		template<class T> void __to_integer(T& i) const;
		// convert into factorial number system any positive number in base 10
		template<class T> void __from_decimal(const T& i);
		
	public:
		factoradic();
		factoradic(int i);
		factoradic(size_t i);
		factoradic(const integer& i);
		factoradic(const string& i);
		factoradic(const factoradic& f);
		
		// Rs is the radixs of a number in factorial
		// base in little endian format
		factoradic(const vector<size_t>& Rs, bool neg = false, bool little_endian = true);
		~factoradic();
		
		/// OPERATORS
		
		inline friend
		ostream& operator<< (ostream& os, const factoradic& f) {
			os << f.to_string();
			return os;
		}
		
		factoradic& operator= (const factoradic& f);
		
		factoradic operator-() const;
		factoradic& operator-();
		
		factoradic operator+ (int i) const;
		factoradic operator+ (const integer& i) const;
		factoradic operator+ (const factoradic& f) const;
		factoradic& operator+= (int i);
		factoradic& operator+= (const integer& i);
		factoradic& operator+= (const factoradic& f);
		
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
		
		factoradic operator/ (int i) const;
		factoradic operator/ (const integer& i) const;
		factoradic operator/ (const factoradic& f) const;
		factoradic& operator/= (int i);
		factoradic& operator/= (const integer& i);
		factoradic& operator/= (const factoradic& f);
		
		factoradic operator^ (uint i) const;
		factoradic operator^ (const integer& i) const;
		factoradic operator^ (const factoradic& f) const;
		factoradic& operator^= (uint i);
		factoradic& operator^= (const integer& i);
		factoradic& operator^= (const factoradic& f);
		
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
		void square();	// ad-hoc algorithm for raising to the power 2
		
		/// GETTERS
		
		bool is_one() const;		// returns true if this number is one
		bool is_zero() const;		// returns true if this number is zero
		bool is_negative() const;	// returns true if this number is negative (<0)
		bool is_even() const;		// returns true if there exists some integer k such that 2*k = *this
		
		void get_radixs(vector<size_t>& rs, size_t n_digits = 0) const;
		
		/// CONVERSIONS
		
		// convert a base-10 number into factoradic number system
		void from_int(int i);
		void from_uint(size_t i);
		void from_integer(const integer& i);
		
		// build the number in factoradic system for the value n!
		void from_factorial(size_t n);
		
		// convert to a base-10 integer
		integer to_integer() const;
		void to_integer(integer& i) const;
		int to_int() const;
		size_t to_uint() const;
		
		// use, at least, n_digits to represent this number
		// n_digits = 0 is interpreted as using the minimum amount of
		// digits.
		string to_string(size_t n_digits = 0) const;
		void to_string(string& s, size_t n_digits = 0) const;
		
		/// OTHERS
		
		// removes all leading zeros
		void shrink();
		
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
	F.from_integer(f1);
	swap(F, f2);
}

inline static
void swap(factoradic& f1, integer& f2) {
	factoradic F;
	F.from_integer(f2);
	swap(f1, F);
}

#include "factoradic_template.cpp"
