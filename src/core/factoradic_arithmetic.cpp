#include "factoradic.hpp"

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

void factoradic::fast_multiply(const factoradic& f) {
	// if b is even:
	//     a*b = a*(b/2) + a*(b/2) = 2*(a*(b/2))
	// if b is odd:
	//     a*b = a*((b - 1)/2) + a*((b - 1)/2) + a = 2*(a*((b - 1)/2)) + a
	//	       = a*(b - 1) + a = a*b
	
	if (f > 1) {
		factoradic fc = f;		// fc := b
		if (fc.is_even()) {
			fc.div2();
			*this *= fc;
			mult2();
		}
		else {
			factoradic copy = *this;	// copy := a
			fc.decrement();		// fc := b - 1
			fc.div2();			// fc := (b - 1)/2
			*this *= fc;		// this := a*(b - 1)/2
			mult2();			// this := 2*(a*(b - 1)/2)
			*this += copy;		// this := 2*(a*(b - 1)/2) + a = a*b
		}
	}
}

void factoradic::mult2() {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	factoradic tc = *this;
	*this += tc;
}

void factoradic::div2() {
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

