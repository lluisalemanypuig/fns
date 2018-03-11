#include "factoradic.hpp"

/// PUBLIC

factoradic::factoradic() {
	neg = false;
}
factoradic::factoradic(int n) {
	from_decimal(n);
}
factoradic::factoradic(const integer& n) {
	from_decimal(n);
}
factoradic::factoradic(const string& n) {
	integer I(n);
	from_decimal(n);
}
factoradic::~factoradic() { }

/// OPERATORS

factoradic& factoradic::operator= (const factoradic& f) {
	radixs = f.radixs;
	neg = f.neg;
	return *this;
}

// ADDITION

factoradic factoradic::operator+ (const integer& k) const {
	factoradic fc = *this;
	fc += k;
	return fc;
}

factoradic factoradic::operator+ (const factoradic& f) const {
	factoradic fc = *this;
	fc += f;
	return fc;
}

factoradic& factoradic::operator+= (const integer& k) {
	accumulate(k);
	return *this;
}

factoradic& factoradic::operator+= (const factoradic& f) {
	accumulate(f);
	return *this;
}

// SUBSTRACTION

factoradic factoradic::operator-() const {
	factoradic f = *this;
	f.neg = not f.neg;
	return f;
}

factoradic& factoradic::operator-() {
	neg = not neg;
	return *this;
}

factoradic factoradic::operator- (const factoradic& f) const {
	factoradic fc = *this;
	fc -= f;
	return fc;
}

factoradic& factoradic::operator-= (const factoradic& f) {
	substract(f);
	return *this;
}

// MULTIPLICATION

factoradic factoradic::operator* (const factoradic& f) const {
	factoradic fc = *this;
	fc *= f;
	return fc;
}

factoradic& factoradic::operator*= (const factoradic& f) {
	bool res_neg = neg and not f.neg or not neg and f.neg;
	
	fast_multiply(f);
	
	neg = res_neg;
	return *this;
}

// DIVISION

factoradic factoradic::operator/ (const factoradic& f) const {
	factoradic fc = *this;
	fc /= f;
	return fc;
}

factoradic& factoradic::operator/= (const factoradic& f) {
	
	cout << "Division not implemented" << endl;
	
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
	if (radixs[1] == 0) {
		// since all n!, for values of n=2,3,4..., are even
		// the only way we can have an odd number is to have
		// 1*(1!) + sum_{n=2}^{C} n*(n!) for some C integer, C >= 2
		
		return radixs[1] != 1;
	}
	
	return false;
}

void factoradic::get_radixs(vector<ushort>& rs, size_t n_digits) const {
	rs = radixs;
	if (n_digits > 0) {
		while (rs.size() < n_digits) {
			rs.push_back(0);
		}
	}
}

/// CONVERSIONS

void factoradic::from_decimal(int k) {
	__from_decimal(k);
}

void factoradic::from_decimal(const integer& I) {
	__from_decimal(I);
}

void factoradic::from_factorial(size_t n) {
	radixs = vector<ushort>(n, 0);
	radixs.push_back(1);
}

integer factoradic::to_decimal() const {
	integer i;
	to_decimal(i);
	return i;
}

void factoradic::to_decimal(integer& i) const {
	i = 0;
	const size_t L = radixs.size();
	
	for (size_t l = L - 1; l > 0; --l) {
		ushort di = radixs[l];
		i = (i + di)*l;
	}
	
	if (neg) {
		i = -i;
	}
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
		s = std::to_string(radixs[0]);
		for (size_t i = 1; i < radixs.size(); ++i) {
			s += "," + std::to_string(radixs[i]);
		}
		
		if (n_digits > 0) {
			size_t actual_length = radixs.size() - 1;
			while (actual_length < n_digits) {
				s += ",0";
				++actual_length;
			}
		}
		
		if (neg) {
			s += ",-";
		}
		reverse(s.begin(), s.end());
	}
}

