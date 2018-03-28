#include "factoradic.hpp"

/// PUBLIC

factoradic::factoradic() {
	neg = false;
	from_int(0);
}
factoradic::factoradic(int i) {
	from_int(i);
}
factoradic::factoradic(size_t i) {
	from_uint(i);
}
factoradic::factoradic(const integer& i) {
	from_integer(i);
}
factoradic::factoradic(const string& i) {
	from_integer(integer(i));
}
factoradic::factoradic(const factoradic& f) {
	radixs = f.radixs;
	neg = f.neg;
}
factoradic::factoradic(const vector<size_t>& Rs, bool _neg, bool le) {
	radixs = Rs;
	neg = _neg;
	
	// format of input: big-endian -> reverse the radixs
	if (not le) {
		reverse(radixs.begin(), radixs.end());
	}
}
factoradic::~factoradic() { }

/// OPERATORS

factoradic& factoradic::operator= (const factoradic& f) {
	radixs = f.radixs;
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

factoradic factoradic::operator^ (uint i) const {
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

factoradic& factoradic::operator^= (uint i) {
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
	if (radixs.size() == 1) {
		return false;
	}
	
	// by construction radixs[0] = 0
	
	// radixs.size() >= 2
	if (radixs[1] == 0) {
		return false;
	}
	
	// the radix corresponding to 1! is 1. The rest must be zero
	
	size_t r = 2;
	while (r < radixs.size() and radixs[r] == 0) {
		++r;
	}
	
	// if 'r' reached the end then all values were 0
	return r == radixs.size();
}

bool factoradic::is_zero() const {
	if (radixs.size() == 1) {
		// if this number has only one radix, then for sure it is zero
		return true;
	}
	
	// by construction radixs[0] = 0
	
	size_t r = 1;
	while (r < radixs.size() and radixs[r] == 0) {
		++r;
	}
	
	// if 'r' reached the end then all values were 0
	return r == radixs.size();
}

bool factoradic::is_negative() const {
	return neg;
}

bool factoradic::is_even() const {
	if (radixs.size() == 1) {
		// this number is 0
		return true;
	}
	
	// since all n!, for values of n=2,3,4..., are even
	// the only way we can have an odd number is to have
	// 1*(1!) + sum_{n=2}^{C} n*(n!) for some C integer, C >= 2
	
	return radixs[1] != 1;
}

void factoradic::get_radixs(vector<size_t>& rs, size_t n_digits) const {
	rs = radixs;
	if (n_digits > 0) {
		while (rs.size() < n_digits) {
			rs.push_back(0);
		}
	}
}

/// CONVERSIONS

void factoradic::from_int(int i) {
	size_t copy_i;
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

void factoradic::from_uint(size_t i) {
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

void factoradic::from_factorial(size_t i) {
	radixs = vector<size_t>(i, 0);
	radixs.push_back(1);
}

integer factoradic::to_integer() const {
	integer i;
	to_integer(i);
	return i;
}

void factoradic::to_integer(integer& i) const {
	__to_integer(i);
}

int factoradic::to_int() const {
	size_t i;
	__to_integer(i);
	
	// although this gives a warning:
	// "conversion to ‘int’ from ‘size_t {aka long unsigned int}’ may alter its value"
	// take this into account: this is only called when the value in
	// base 10 fits in an 'int' value
	int r = i;
	if (neg) {
		r = -r;
	}
	
	return r;
}

size_t factoradic::to_uint() const {
	size_t i;
	__to_integer(i);
	return i;
}

string factoradic::to_string(size_t n_digits) const {
	string s;
	to_string(s, n_digits);
	return s;
}

void factoradic::to_string(string& s, size_t n_digits) const {
	if (radixs.size() == 0) {
		s = "";
	}
	else {
		vector<size_t> radixs_copy = radixs;
		reverse(radixs_copy.begin(), radixs_copy.end());
		
		if (neg) {
			s = "-,";
		}
		
		s += std::to_string(radixs_copy[0]);
		for (size_t i = 1; i < radixs_copy.size(); ++i) {
			s += "," + std::to_string(radixs_copy[i]);
		}
		
		if (n_digits > 0) {
			size_t actual_length = radixs_copy.size() - 1;
			while (actual_length < n_digits) {
				s += ",0";
				++actual_length;
			}
		}
	}
}

void factoradic::shrink() {
	size_t r = radixs.size() - 1;
	while (radixs[r] == 0) {
		--r;
	}
	
	if (r < radixs.size() - 1) {
		radixs.resize(r + 1);
	}
}

