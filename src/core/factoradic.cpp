/*********************************************************************
 * Factorial Number System
 * Copyright (C) 2018  Lluís Alemany Puig
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

#include "factoradic.hpp"

/// PUBLIC

factoradic::factoradic() {
	neg = false;
	from_int(0);
}
factoradic::factoradic(int64_t i) {
	from_long_int(i);
}
factoradic::factoradic(const integer& i) {
	from_integer(i);
}
factoradic::factoradic(const string& i) {
	from_integer(integer(i));
}
factoradic::factoradic(const factoradic& f) {
	digits = f.digits;
	neg = f.neg;
}
factoradic::factoradic(const vector<digit_t>& Rs, bool _neg, bool le) {
	digits = Rs;
	neg = _neg;
	
	// format of input: big-endian -> reverse the digits
	if (not le) {
		reverse(digits.begin(), digits.end());
	}
}
factoradic::~factoradic() { }

/// OPERATORS

factoradic& factoradic::operator= (const factoradic& f) {
	digits = f.digits;
	neg = f.neg;
	return *this;
}

// NEGATION

factoradic factoradic::operator-() const {
	factoradic f = *this;
	f.neg = not f.neg;
	return f;
}

factoradic& factoradic::operator-() {
	neg = not neg;
	return *this;
}

// ADDITION

factoradic factoradic::operator+ (int i) const {
	factoradic fc = *this;
	fc += i;
	return fc;
}

factoradic factoradic::operator+ (const integer& i) const {
	factoradic fc = *this;
	fc += i;
	return fc;
}

factoradic factoradic::operator+ (const factoradic& f) const {
	factoradic fc = *this;
	fc += f;
	return fc;
}

factoradic& factoradic::operator+= (int i) {
	accumulate(factoradic(i));
	return *this;
}

factoradic& factoradic::operator+= (const integer& i) {
	accumulate(factoradic(i));
	return *this;
}

factoradic& factoradic::operator+= (const factoradic& f) {
	accumulate(f);
	return *this;
}

// SUBSTRACTION

factoradic factoradic::operator- (int i) const {
	factoradic fc = *this;
	fc -= i;
	return fc;
}

factoradic factoradic::operator- (const integer& i) const {
	factoradic fc = *this;
	fc -= i;
	return fc;
}

factoradic factoradic::operator- (const factoradic& f) const {
	factoradic fc = *this;
	fc -= f;
	return fc;
}

factoradic& factoradic::operator-= (int i) {
	substract(factoradic(i));
	return *this;
}

factoradic& factoradic::operator-= (const integer& i) {
	substract(factoradic(i));
	return *this;
}

factoradic& factoradic::operator-= (const factoradic& f) {
	substract(f);
	return *this;
}

// MULTIPLICATION

factoradic factoradic::operator* (int i) const {
	factoradic fc = *this;
	fc *= i;
	return fc;
}

factoradic factoradic::operator* (const integer& i) const {
	factoradic fc = *this;
	fc *= i;
	return fc;
}

factoradic factoradic::operator* (const factoradic& f) const {
	factoradic fc = *this;
	fc *= f;
	return fc;
}

factoradic& factoradic::operator*= (int i) {
	bool res_neg = (neg and i >= 0) or (not neg and i < 0);
	
	integer_multiply(i);
	
	neg = res_neg;
	return *this;
}

factoradic& factoradic::operator*= (const integer& i) {
	bool res_neg = (neg and not i.is_negative()) or (not neg and i.is_negative());
	
	integer_multiply(i);
	
	neg = res_neg;
	return *this;
}

factoradic& factoradic::operator*= (const factoradic& f) {
	bool res_neg = (neg and not f.neg) or (not neg and f.neg);
	
	factoradic_multiply(f);
	
	neg = res_neg;
	return *this;
}

// DIVISION

factoradic factoradic::operator/ (int i) const {
	factoradic fc = *this;
	fc /= i;
	return fc;
}

factoradic factoradic::operator/ (const integer& i) const {
	factoradic fc = *this;
	fc /= i;
	return fc;
}

factoradic factoradic::operator/ (const factoradic& f) const {
	factoradic fc = *this;
	fc /= f;
	return fc;
}

factoradic& factoradic::operator/= (int i) {
	bool res_neg = (neg and i >= 0) or (not neg and i < 0);
	
	int_divide( (i < 0 ? -i : i) );
	
	neg = res_neg;
	return *this;
}

factoradic& factoradic::operator/= (const integer& i) {
	bool res_neg = (neg and not i.is_negative()) or (not neg and i.is_negative());
	
	integer_divide(i);
	
	neg = res_neg;
	return *this;
}

factoradic& factoradic::operator/= (const factoradic& f) {
	integer i;
	f.to_integer(i);
	
	bool res_neg = (neg and not f.neg) or (not neg and f.neg);
	
	integer_divide(i);
	
	neg = res_neg;
	return *this;
}

factoradic factoradic::operator^ (uint32_t i) const {
	factoradic copy = *this;
	copy ^= i;
	return copy;
}

factoradic factoradic::operator^ (const integer& i) const {
	factoradic copy = *this;
	copy ^= i;
	return copy;
}

factoradic factoradic::operator^ (const factoradic& f) const {
	factoradic copy = *this;
	copy ^= f;
	return copy;
}

factoradic& factoradic::operator^= (uint32_t i) {
	neg = neg and (i%2 == 1);
	integer_power(i);
	return *this;
}

factoradic& factoradic::operator^= (const integer& i) {
	neg = neg and (i%2 == 1);
	integer_power(i);
	return *this;
}

factoradic& factoradic::operator^= (const factoradic& f) {
	neg = neg and (not f.is_even());
	factoradic_power(f);
	return *this;
}

factoradic& factoradic::operator++ () {
	increment();
	return *this;
}

factoradic factoradic::operator++ (int) {
	factoradic copy = *this;
	increment();
	return copy;
}

factoradic& factoradic::operator-- () {
	decrement();
	return *this;
}

factoradic factoradic::operator-- (int) {
	factoradic copy = *this;
	decrement();
	return copy;
}

/// GETTERS

bool factoradic::is_one() const {
	if (digits.size() == 1) {
		return false;
	}
	
	// by construction digits[0] = 0
	
	// digits.size() >= 2
	if (digits[1] == 0) {
		return false;
	}
	
	// the radix corresponding to 1! is 1. The rest must be zero
	
	index_t r = 2;
	while (r < digits.size() and digits[r] == 0) {
		++r;
	}
	
	// if 'r' reached the end then all values were 0
	return r == digits.size();
}

bool factoradic::is_zero() const {
	if (digits.size() == 1) {
		// if this number has only one radix, then for sure it is zero
		return true;
	}
	
	// by construction digits[0] = 0
	
	index_t r = 1;
	while (r < digits.size() and digits[r] == 0) {
		++r;
	}
	
	// if 'r' reached the end then all values were 0
	return r == digits.size();
}

bool factoradic::is_negative() const {
	return neg;
}

bool factoradic::is_even() const {
	if (digits.size() == 1) {
		// this number is 0
		return true;
	}
	
	// since all n!, for values of n=2,3,4..., are even
	// the only way we can have an odd number is to have
	// 1*(1!) + sum_{n=2}^{C} n*(n!) for some C integer, C >= 2
	
	return digits[1] != 1;
}

void factoradic::get_digits(vector<digit_t>& rs, size_t n_digits) const {
	rs = digits;
	if (n_digits > 0) {
		while (rs.size() < n_digits) {
			rs.push_back(0);
		}
	}
}

/// CONVERSIONS

void factoradic::from_int(int32_t i) {
	uint32_t copy_i;
	if (i < 0) {
		copy_i = -i;
		neg = true;
	}
	else {
		neg = false;
		copy_i = i;
	}
	__from_decimal(copy_i);
}

void factoradic::from_uint(uint32_t i) {
	__from_decimal(i);
}

void factoradic::from_long_int(int64_t i) {
	uint64_t copy_i;
	if (i < 0) {
		copy_i = -i;
		neg = true;
	}
	else {
		neg = false;
		copy_i = i;
	}
	__from_decimal(copy_i);
}

void factoradic::from_long_uint(uint64_t i) {
	__from_decimal(i);
}

void factoradic::from_integer(const integer& i) {
	integer copy_i;
	if (i < 0) {
		copy_i = -i;
		neg = true;
	}
	else {
		neg = false;
		copy_i = i;
	}
	__from_decimal(copy_i);
}

void factoradic::from_factorial(uint32_t i) {
	digits = vector<digit_t>(i, 0);
	digits.push_back(1);
}

integer factoradic::to_integer() const {
	integer i;
	to_integer(i);
	return i;
}

void factoradic::to_integer(integer& i) const {
	__to_integer(i);
}

int32_t factoradic::to_int() const {
	uint32_t i;
	__to_integer(i);
	
	// although this gives a warning:
	// "conversion to ‘int’ from ‘size_t {aka long unsigned int}’ may alter its value"
	// take this into account: this is only called when the value in
	// base 10 fits in an 'int' value
	int32_t r = i;
	if (neg) {
		r = -r;
	}
	
	return r;
}

uint32_t factoradic::to_uint() const {
	uint32_t i;
	__to_integer(i);
	return i;
}

string factoradic::to_string(size_t n_digits) const {
	string s;
	to_string(s, n_digits);
	return s;
}

void factoradic::to_string(string& s, size_t n_digits) const {
	if (digits.size() == 0) {
		s = "";
	}
	else {
		vector<digit_t> digits_copy = digits;
		reverse(digits_copy.begin(), digits_copy.end());
		
		if (neg) {
			s = "-,";
		}
		
		s += std::to_string(digits_copy[0]);
		for (index_t i = 1; i < digits_copy.size(); ++i) {
			s += "," + std::to_string(digits_copy[i]);
		}
		
		if (n_digits > 0) {
			index_t actual_length = digits_copy.size() - 1;
			while (actual_length < n_digits) {
				s += ",0";
				++actual_length;
			}
		}
	}
}

void factoradic::shrink() {
	index_t r = digits.size() - 1;
	while (digits[r] == 0) {
		--r;
	}
	
	if (r < digits.size() - 1) {
		digits.resize(r + 1);
	}
}

