#pragma once

/// C includes
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
		
	public:
		factoradic();
		// converts the number in decimal base d into factorial number system
		factoradic(size_t d);
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
		
		factoradic operator- (const factoradic& f) const;
		factoradic& operator-= (const factoradic& f);
		
		factoradic operator* (const factoradic& f) const;
		factoradic& operator*= (const factoradic& f);
		
		factoradic operator/ (const factoradic& f) const;
		factoradic& operator/= (const factoradic& f);
		
		/// GETTERS
		
		void get_radixs(vector<ushort>& rs, size_t n_digits = 0) const;
		
		/// CONVERSIONS
		
		void from_decimal(size_t n);
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
