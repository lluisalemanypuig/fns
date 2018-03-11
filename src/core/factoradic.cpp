#include "factoradic.hpp"

/// NON-CLASS

/// PRIVATE

void factoradic::increment() {
	if (is_zero()) {
		// this number is 0
		from_decimal(1);
		return;
	}
	
	// let a := *this
	
	// if a < 0 then
	// -a + 1 = -(a - 1)
	if (neg) {
		neg = false;	// *this <- a
		decrement();	// *this <- a - 1
		neg = true;		// *this <- -(a - 1)
		return;
	}
	
	// if a > 0 then apply algorithm
	
	size_t carry = 1;
	size_t l = 1;
	
	while (l < radixs.size() and carry > 0) {
		size_t sum = size_t(radixs[l]) + carry;
		size_t mod = sum%(l + 1);
		radixs[l] = mod;
		
		carry = (sum - mod)/(l + 1);
		
		++l;
	}
	
	while (carry > 0) {
		
		size_t mod = carry%(l + 1);
		radixs.push_back(mod);
		
		carry = (l + 1 - mod)/(l + 1);
	}
}

void factoradic::decrement() {
	if (is_zero()) {
		// this number is 0
		from_decimal(-1);
		return;
	}
	
	// let a := *this
	
	// if a < 0 then
	// (-a) - 1 = -(a + 1)
	if (neg) {
		neg = false;	// *this = a
		increment();	// *this = a + 1
		neg = true;		// *this = -(a + 1)
		return;
	}
	
	size_t l = 0;
	
	ushort carry = 1;
	while (l < radixs.size()) {
		
		// difference between the pointed radixs
		int dif = radixs[l] - carry;
		
		// normalise
		if (dif < 0) {
			dif += l + 1;
			carry = 1;
		}
		else {
			carry = 0;
		}
		
		radixs[l] = dif;
		
		++l;
	}
}

void factoradic::__accumulate(const factoradic& f) {
	size_t l = 0;
	
	size_t carry = 0;
	while (l < radixs.size() and l < f.radixs.size()) {
		// if the sum of the two digits is greater than the greatest
		// radix allowed then compute the modulus of the result.
		// The greatest radix allowed is equal to the index
		
		// sum of the two currently pointed radixs plus the carry
		size_t sum_dig = size_t(radixs[l]) + size_t(f.radixs[l]) + carry;
		
		// new digit
		size_t mod = sum_dig%(l + 1);
		radixs[l] = mod;
		
		// value of carry
		carry = (sum_dig - mod)/(l + 1);
		
		++l;
	}
	
	// if this number and f do not have the same length then keed adding
	// the carry until the full length of the longest number has been
	// iterated over
	if (radixs.size() < f.radixs.size()) {
		for (size_t it = l; it < f.radixs.size(); ++it) {
			// sum of the currently pointed radix plus the carry
			size_t sum_dig = size_t(f.radixs[it]) + carry;
			
			// new digit
			size_t mod = sum_dig%(it + 1);
			radixs.push_back(mod);
			
			// value of carry
			carry = (sum_dig - mod)/(it + 1);
		}
	}
	else if (radixs.size() > f.radixs.size()) {
		for (size_t it = l; it < radixs.size(); ++it) {
			// sum of the currently pointed radix plus the carry
			size_t sum_dig = size_t(radixs[it]) + carry;
			
			// new digit
			size_t mod = sum_dig%(it + 1);
			radixs[it] = mod;
			
			// value of carry
			carry = (sum_dig - mod)/(it + 1);
		}
	}
	
	// the two numbers may have different lengths, or not. In any case
	// there is still more carry
	if (carry != 0) {
		
		while (carry > 0) {
			size_t new_digit;
			if (carry > l) {
				new_digit = carry/l;
				carry = carry/l;
			}
			else {
				new_digit = carry;
				carry = 0;
			}
			radixs.push_back(new_digit);
			++l;
		}
	}
}

void factoradic::__substract(const factoradic& f) {
	size_t l = 0;
	
	ushort carry = 0;
	while (l < radixs.size() and l < f.radixs.size()) {
		
		// difference between the pointed radixs
		int dif = radixs[l] - (f.radixs[l] + carry);
		
		// normalise
		if (dif < 0) {
			dif += l + 1;
			carry = 1;
		}
		else {
			carry = 0;
		}
		
		radixs[l] = dif;
		
		++l;
	}
	
	// likewise in the accumulate algorithm, there are
	// more radixs to deal with
	if (radixs.size() > f.radixs.size()) {
		for (; l < radixs.size(); ++l) {
			int dif = radixs[l] - carry;
			if (dif < 0) {
				carry = 1;
				dif += l + 1;
			}
			else {
				carry = 0;
			}
			radixs[l] = dif;
		}
	}
	else if (radixs.size() < f.radixs.size()) {
		
		// even if this was true, since we are calling this function
		// assuming that (*this > f) this implies that the extra
		// radixs in f are all 0s
		
	}
	
	// claim: carry cannot be different from 0 because we are calling
	// this procedure under the assumption that     *this > f
	
	assert(carry == 0);
}

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
	
	/*
	/// ----------
	// 'slow' multiplication algorithm
	// add b times *this to itself
	
	neg = false;
	factoradic a = *this;
	
	integer k;
	f.to_decimal(k);
	if (k < 0) {
		k = -k;
	}
	
	for (integer t = 1; t < k; ++t) {
		*this += a;
	}
	/// ----------
	*/
	
	
	/// ----------
	// 'fast' multiplication algorithm
	
	// if b is even:
	//     a*b = a*(b/2) + a*(b/2) = 2*(a*(b/2))
	// if b is odd:
	//     a*b = a*((b - 1)/2) + a*((b - 1)/2) + a = 2*(a*((b - 1)/2)) + a
	//	       = a*(b - 1) + a = a*b
	
	if (f > 1) {
		factoradic fc = f;		// fc := b
		if (fc.is_even()) {
			fc.halve();
			
			*this *= fc;
			
			mult2();
		}
		else {
			factoradic copy = *this;	// copy := a
			
			fc.decrement();		// fc := b - 1
			fc.halve();			// fc := (b - 1)/2
			
			*this *= fc;		// this := a*(b - 1)/2
			
			mult2();			// this := 2*(a*(b - 1)/2)
			
			*this += copy;		// this := 2*(a*(b - 1)/2) + a = a*b
		}
	}
	/// ----------
	
	
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

void factoradic::mult2() {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	factoradic tc = *this;
	*this += tc;
}

void factoradic::halve() {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	ushort carry = 0;
	size_t r = radixs.size() - 1;
	
	do {
		ushort pradix = radixs[r];
		
		// calculate new r-th radix
		radixs[r] = (carry + pradix)/2;
		
		// calculate carry
		if ((carry + pradix) & 0x1 == 1) {
			carry = r;
		}
		else {
			carry = 0;
		}
		
		--r;
	}
	while (r > 0);
	
}

// COMPARISON

bool factoradic::operator== (int k) const {
	factoradic as_factoradic;
	as_factoradic.from_decimal(k);
	return *this == as_factoradic;
}

bool factoradic::operator== (integer k) const {
	factoradic as_factoradic;
	as_factoradic.from_decimal(k);
	return *this == as_factoradic;
}

bool factoradic::operator== (const factoradic& k) const {
	if (neg and not k.neg or not neg and k.neg) {
		return false;
	}
	
	size_t L = radixs.size();
	size_t kL = k.radixs.size();
	
	// compare the "common radixs". If any is different then
	// so are the numbers
	
	// *this:  0 1 2 3 4 0 0 0 0 ....
	//     k:  0 1 2 3 4
	
	bool equal = true;
	size_t r = 0;
	while (equal and r < L and r < kL) {
		if (radixs[r] != k.radixs[r]) {
			equal = false;
		}
		++r;
	}
	
	// if 'r' reached the end of one number but not of the other then,
	// in order for the two of them to be equal, what is left of the
	// number with more radixs must be all 0's
	
	if (L != kL and equal) {
		// longest radix vector
		const vector<ushort>& rest = (L < kL ? k.radixs : radixs);
		while (r < rest.size() and rest[r] == 0) {
			++r;
		}
		
		// the iterator could not reach the end ->
		// found something different from zero -> numbers not equal
		if (r < rest.size()) {
			equal = false;
		}
	}
	
	return equal;
}

bool factoradic::operator> (int k) const {
	factoradic as_factoradic;
	as_factoradic.from_decimal(k);
	return *this > as_factoradic;
}

bool factoradic::operator> (integer k) const {
	factoradic as_factoradic;
	as_factoradic.from_decimal(k);
	return *this > as_factoradic;
}

bool factoradic::operator> (const factoradic& k) const {
	if (neg and not k.neg) {
		// (< 0) >? (> 0) -> No
		return false;
	}
	if (not neg and k.neg) {
		// (> 0) >? (< 0) -> Yes
		return true;
	}
	
	// consider both numbers positive, and compute the comparison.
	// *this is greater than k if the first highest-weight non-zero
	// radix in *this is greater than the corresponding radix in k
	
	// positions of the first non-zero radix for *this and k
	size_t pnZ_t = radixs.size() - 1;
	while (pnZ_t > 0 and radixs[pnZ_t] == 0) {
		--pnZ_t;
	}
	size_t pnZ_k = k.radixs.size() - 1;
	while (pnZ_k > 0 and k.radixs[pnZ_k] == 0) {
		--pnZ_k;
	}
	
	// pointers not in the same position. Now we can decide '>'
	if (pnZ_t != pnZ_k) {
		
		// if both are positive:
		if (not neg and not k.neg) {
			return pnZ_t > pnZ_k;
		}
		
		// if both are negative:
		return pnZ_t < pnZ_k;
	}
	
	// when in the same position, move the pointers to the first 
	// pair of different radixs
	
	while (pnZ_t > 0 and radixs[pnZ_t] == k.radixs[pnZ_t]) {
		--pnZ_t;
	}
	
	// if both are positive:
	if (not neg and not k.neg) {
		return radixs[pnZ_t] > k.radixs[pnZ_t];
	}
	
	// if both are negative:
	return radixs[pnZ_t] < k.radixs[pnZ_t];
}

bool factoradic::operator>= (int k) const				{ return *this > k or *this == k; }
bool factoradic::operator>= (integer k) const			{ return *this > k or *this == k; }
bool factoradic::operator>= (const factoradic& k) const	{ return *this > k or *this == k; }

bool factoradic::operator< (int k) const {
	factoradic as_factoradic;
	as_factoradic.from_decimal(k);
	return as_factoradic > *this;
}

bool factoradic::operator< (integer k) const {
	factoradic as_factoradic;
	as_factoradic.from_decimal(k);
	return as_factoradic > *this;
}

bool factoradic::operator< (const factoradic& k) const {
	return k > *this;
}

bool factoradic::operator<= (int k) const				{ return *this < k or *this == k; }
bool factoradic::operator<= (integer k) const			{ return *this < k or *this == k; }
bool factoradic::operator<= (const factoradic& k) const	{ return *this < k or *this == k; }

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

bool factoradic::is_zero() const {
	bool all_zeros = true;
	size_t r = 0;
	while (r < radixs.size() and all_zeros) {
		all_zeros = radixs[r] == 0;
		++r;
	}
	return all_zeros;
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

/// GETTERS

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

