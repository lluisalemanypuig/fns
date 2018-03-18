#include "factoradic.hpp"

/// PRIVATE

void factoradic::__increment() {
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

void factoradic::__decrement() {
	size_t l = 0;
	
	ushort carry = 1;
	while (l < radixs.size() and carry > 0) {
		
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
	
	// the algorithm ensures that carry is 0 at the end of the while loop.
	assert(carry == 0);
}

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
		neg = false;	// *this := a
		__decrement();	// *this := a - 1
		neg = true;		// *this := -(a - 1)
		return;
	}
	
	// if a > 0 then apply algorithm
	__increment();
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
		neg = false;	// *this := a
		__increment();	// *this := a + 1
		neg = true;		// *this := -(a + 1)
		return;
	}
	
	__decrement();
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
		// -->
		// if I'm wrong then this assertion will make the program
		// terminate its execution
		
		assert( false );
	}
	
	// claim: carry cannot be different from 0 because we are calling
	// this procedure under the assumption that     *this > f
	
	assert(carry == 0);
}

void factoradic::factoradic_fast_multiply(const factoradic& f) {
	// if b is even:
	//     a*b = a*(b/2) + a*(b/2) = 2*(a*(b/2))
	// if b is odd:
	//     a*b = a*((b - 1)/2) + a*((b - 1)/2) + a = 2*(a*((b - 1)/2)) + a
	//	       = a*(b - 1) + a = a*b
	
	if (not f.is_one()) {
		// fc := b
		factoradic fc = f;
		
		if (fc.is_even()) {
			fc.div2();
			factoradic_fast_multiply(fc);
			mult2();
		}
		else {
			factoradic copy = *this;		// copy := a
			fc.decrement();					// fc := b - 1
			fc.div2();						// fc := (b - 1)/2
			factoradic_fast_multiply(fc);	// this := a*(b - 1)/2
			mult2();						// this := 2*(a*(b - 1)/2)
			__accumulate(copy);				// this := 2*(a*(b - 1)/2) + a = a*b
		}
	}
}

void factoradic::int_divide(uint i) {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	ushort carry = 0;
	size_t r = radixs.size() - 1;
	
	do {
		ushort radix = radixs[r];
		size_t s = radix + carry;
		
		cout << "r= " << r << endl;
		cout << "radixs[" << r << "]= " << radixs[r] << endl;
		cout << "carry= " << carry << endl;
		cout << "s= " << s << endl;
		
		// i is a divisor of s
		if (s%i == 0) {
			cout << "    perfect division: s=" << s << ". i=" << i << endl;
			
			size_t d = s/i;
			cout << "    d= " << d << endl;
			
			radixs[r] = d%(r + 1);
			carry = d - d%r;
		}
		else {
			
			if (s < i) {
				cout << "    s(" << s << ") < i(" << i << ")" << endl;
				
				radixs[r] = 0;
				carry = s*r;
			}
			else {
				cout << "    s(" << s << ") > i(" << i << ")" << endl;
				
				// radix + carry > i
				radixs[r] = s/i;
				carry = (s%i)*r;
			}
		}
		
		cout << "radixs[" << r << "]= " << radixs[r] << endl;
		cout << "carry= " << carry << endl;
		cout << "---------" << endl;
		
		/*
		// calculate carry
		if ((carry + pradix)%i == i - 1) {
			carry = r;
		}
		else {
			carry = 0;
		}
		*/
		
		--r;
	}
	while (r > 0);
	
	if (carry > 0) {
		// there is still some carry
		cout << "************" << endl;
		cout << "carry left: " << carry << endl;
	}
}

void factoradic::integer_divide(const integer& i) {
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
		ushort radix = radixs[r];
		
		// calculate new r-th radix
		radixs[r] = (carry + radix)/2;
		
		// calculate carry
		if (((carry + radix) & 0x1) == 1) {
			carry = r;
		}
		else {
			carry = 0;
		}
		
		--r;
	}
	while (r > 0);
}

