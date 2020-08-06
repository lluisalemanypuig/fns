/*********************************************************************
 * Factorial Number System
 * Copyright (C) 2018-2020  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 *
 ********************************************************************/

#pragma once

/// C includes
#include <stdlib.h>
#include <gmp.h>

/// C++ includes
#include <fstream>
#include <cstdint>
#include <string>
using namespace std;

/// Custom includes
#include "gmp_utils.hpp"

class integer {
	private:
		mpz_t val;
		bool initialized;
	
	public:
		integer();
		integer(int32_t i);
		integer(const char *s, int32_t base = 10);
		integer(const string& s, int32_t base = 10);
		integer(const integer& i);
		~integer();
		
		/// ALLOC AND DEALLOC
		
		void init();
		void init_si(int32_t i);
		void init_ui(uint32_t i);
		void init(const char *s, int32_t base = 10);
		void init(const string& s, int32_t base = 10);
		void init(const integer& i);
		
		void clear();
		
		/// SET VALUE
		
		void set_si(int32_t i);
		void set_ui(uint32_t i);
		void set(const char *s, int32_t base = 10);
		void set(const string& v, int32_t base = 10);
		void set(const integer& i);
		
		/// OPERATORS
		
		integer& operator= (int32_t i);
		integer& operator= (const char *s);
		integer& operator= (const string& v);
		integer& operator= (const integer& i);
		
		bool operator== (int32_t i) const;
		bool operator== (const char *s) const;
		bool operator== (const string& s) const;
		bool operator== (const integer& i) const;
		
		bool operator!= (int32_t i) const;
		bool operator!= (const char *s) const;
		bool operator!= (const string& s) const;
		bool operator!= (const integer& i) const;
		
		bool operator< (int32_t i) const;
		bool operator< (const char *s) const;
		bool operator< (const string& s) const;
		bool operator< (const integer& i) const;
		
		bool operator<= (int32_t i) const;
		bool operator<= (const char *s) const;
		bool operator<= (const string& s) const;
		bool operator<= (const integer& i) const;
		
		bool operator> (int32_t i) const;
		bool operator> (const char *s) const;
		bool operator> (const string& s) const;
		bool operator> (const integer& i) const;
		
		bool operator>= (int32_t i) const;
		bool operator>= (const char *s) const;
		bool operator>= (const string& s) const;
		bool operator>= (const integer& i) const;
		
		integer operator+ (int32_t i) const;
		integer operator+ (const char *s) const;
		integer operator+ (const string& s) const;
		integer operator+ (const integer& i) const;
		
		integer& operator+= (int32_t i);
		integer& operator+= (const char *s);
		integer& operator+= (const string& s);
		integer& operator+= (const integer& i);
		
		integer operator- () const;
		integer operator- (int32_t i) const;
		integer operator- (const char *s) const;
		integer operator- (const string& s) const;
		integer operator- (const integer& i) const;
		
		integer& operator- ();
		integer& operator-= (int32_t i);
		integer& operator-= (const char *s);
		integer& operator-= (const string& s);
		integer& operator-= (const integer& i);
		
		integer operator* (int32_t i) const;
		integer operator* (const char *s) const;
		integer operator* (const string& s) const;
		integer operator* (const integer& i) const;
		
		integer& operator*= (int32_t i);
		integer& operator*= (const char *s);
		integer& operator*= (const string& s);
		integer& operator*= (const integer& i);
		
		integer operator/ (int32_t i) const;
		integer operator/ (const char *s) const;
		integer operator/ (const string& s) const;
		integer operator/ (const integer& i) const;
		
		integer& operator/= (int32_t i);
		integer& operator/= (const char *s);
		integer& operator/= (const string& s);
		integer& operator/= (const integer& i);
		
		integer operator^ (uint32_t i) const;
		integer operator^ (const integer& i) const;
		
		integer& operator^= (uint32_t i);
		integer& operator^= (const integer& i);
		
		integer operator% (const integer& i) const;
		uint64_t operator% (uint64_t i) const;
		
		integer& operator++ ();			// prefix:	++a
        integer operator++ (int32_t);	// postfix:	a++
        integer& operator-- ();			// prefix:	--a
        integer operator-- (int32_t);	// postfix:	a--
		
		inline friend
		istream& operator>> (istream& is, integer& i) {
			string s;
			is >> s;
			i.init(s, 10);
			return is;
		}
		
		inline friend
		ostream& operator<< (ostream& os, const integer& i) {
			os << i.to_string();
			return os;
		}
		
		/// GETTERS
		
		bool is_initialized() const;
		int32_t get_sign() const;
		bool is_negative() const;
		
		size_t bytes() const;
		const mpz_t& get_raw_value() const;
		int32_t to_int() const;
		uint32_t to_uint() const;
		int64_t to_long_int() const;
		uint64_t to_long_uint() const;
		
		/// CONVERTERS
		
		string to_string() const;
		void to_string(string& s) const;
};

static inline
void swap(integer& a, integer& b) {
	integer copy = a;
	a = b;
	b = copy;
}

