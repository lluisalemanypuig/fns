#include "factoradic.hpp"

/// OPERATORS

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
	if ((neg and not k.neg) or (not neg and k.neg)) {
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

