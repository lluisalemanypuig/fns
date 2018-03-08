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
		string radixs;
		
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
		
		/// CONVERSIONS
		
		void from_decimal(size_t n);
		void from_decimal(const integer& i);
		
		integer to_decimal() const;
		void to_decimal(integer& i) const;
		
		string to_string() const;
		void to_string(string& s) const;
		
};
