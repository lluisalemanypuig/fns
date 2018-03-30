#include "factoradic.hpp"

/// OPERATORS

// COMPARISON

bool factoradic::operator== (int i) const {
	factoradic as_factoradic;
	as_factoradic.from_int(i);
	return *this == as_factoradic;
}

bool factoradic::operator== (const integer& i) const {
	factoradic as_factoradic;
	as_factoradic.from_integer(i);
	return *this == as_factoradic;
}

bool factoradic::operator== (const factoradic& f) const {
	if ((neg and not f.neg) or (not neg and f.neg)) {
		return false;
	}
	
	size_t R = radixs.size();
	size_t kR = f.radixs.size();
	
	// compare the "common radixs". If any is different then
	// so are the numbers
	
	// *this:  0 1 2 3 4 0 0 0 0 ....
	//     k:  0 1 2 3 4
	
	bool equal = true;
	size_t r = 0;
	while (equal and r < R and r < kR) {
		if (radixs[r] != f.radixs[r]) {
			equal = false;
		}
		++r;
	}
	
	// if 'r' reached the end of one number but not of the other then,
	// in order for the two of them to be equal, what is left of the
	// number with more radixs must be all 0's
	
	if (R != kR and equal) {
		// longest radix vector
		const vector<size_t>& rest = (R < kR ? f.radixs : radixs);
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

bool factoradic::operator> (int i) const {
	factoradic as_factoradic;
	as_factoradic.from_int(i);
	return *this > as_factoradic;
}

bool factoradic::operator> (const integer& i) const {
	factoradic as_factoradic;
	as_factoradic.from_integer(i);
	return *this > as_factoradic;
}

bool factoradic::operator> (const factoradic& f) const {
	if (neg and not f.neg) {
		// (< 0) >? (> 0) -> No
		return false;
	}
	if (not neg and f.neg) {
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
	size_t pnZ_k = f.radixs.size() - 1;
	while (pnZ_k > 0 and f.radixs[pnZ_k] == 0) {
		--pnZ_k;
	}
	
	// pointers not in the same position. Now we can decide '>'
	if (pnZ_t != pnZ_k) {
		
		// if both are positive:
		if (not neg and not f.neg) {
			return pnZ_t > pnZ_k;
		}
		
		// if both are negative:
		return pnZ_t < pnZ_k;
	}
	
	// when in the same position, move the pointers to the first 
	// pair of different radixs
	
	while (pnZ_t > 0 and radixs[pnZ_t] == f.radixs[pnZ_t]) {
		--pnZ_t;
	}
	
	// if both are positive:
	if (not neg and not f.neg) {
		return radixs[pnZ_t] > f.radixs[pnZ_t];
	}
	
	// if both are negative:
	return radixs[pnZ_t] < f.radixs[pnZ_t];
}

bool factoradic::operator>= (int i) const				{ return *this > i or *this == i; }
bool factoradic::operator>= (const integer& i) const	{ return *this > i or *this == i; }
bool factoradic::operator>= (const factoradic& f) const	{ return *this > f or *this == f; }

bool factoradic::operator< (int i) const {
	factoradic as_factoradic;
	as_factoradic.from_int(i);
	return as_factoradic > *this;
}

bool factoradic::operator< (const integer& i) const {
	factoradic as_factoradic;
	as_factoradic.from_integer(i);
	return as_factoradic > *this;
}

bool factoradic::operator< (const factoradic& f) const 	{ return f > *this; }

bool factoradic::operator<= (int i) const				{ return *this < i or *this == i; }
bool factoradic::operator<= (const integer& i) const	{ return *this < i or *this == i; }
bool factoradic::operator<= (const factoradic& f) const	{ return *this < f or *this == f; }

