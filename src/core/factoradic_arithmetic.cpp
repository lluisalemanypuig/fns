#include "factoradic.hpp"

/// PRIVATE

void factoradic::__increment() {
	size_t carry = 1;
	size_t l = 1;
	
	while (l < radixs.size() and carry > 0) {
		size_t sum = size_t(radixs[l]) + carry;
		size_t mod = sum%(l + 1);
		
		radixs[l] = mod;
		
		assert(sum >= mod);
		carry = (sum - mod)/(l + 1);
		
		++l;
	}
	
	while (carry > 0) {
		size_t mod = carry%(l + 1);
		radixs.push_back(mod);
		
		assert(l + 1 >= mod);
		carry = (l + 1 - mod)/(l + 1);
	}
}

void factoradic::__decrement() {
	size_t l = 0;
	
	size_t carry = 1;
	while (l < radixs.size() and carry > 0) {
		
		if (radixs[l] < carry) {
			assert(radixs[l] + l + 1 >= carry);
			radixs[l] = radixs[l] + l + 1 - carry;
			
			carry = 1;
		}
		else {
			radixs[l] = radixs[l] - carry;
			carry = 0;
		}
		
		++l;
	}
	
	// the algorithm ensures that carry is 0 at the end of the while loop.
	assert(carry == 0);
}

void factoradic::increment() {
	if (is_zero()) {
		// this number is 0
		from_uint(1);
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
		from_int(-1);
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
		
		// value of carry.
		// by construction:
		//     S := sum_dig
		//     mod := S%(l + 1)
		//     sum_dig > mod    <=>    S > S%(l + 1)
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
			// by construction:
			//     S := sum_dig
			//     mod := S%(l + 1)
			//     sum_dig > mod    <=>    S > S%(l + 1)
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
			// by construction:
			//     S := sum_dig
			//     mod := S%(l + 1)
			//     sum_dig > mod    <=>    S > S%(l + 1)
			carry = (sum_dig - mod)/(it + 1);
		}
	}
	
	// the two numbers may have different lengths, or not. In any case
	// there is still more carry
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

void factoradic::accumulate(const factoradic& f) {
	if (not is_negative() and not f.is_negative()) {
		// a + b
		
		__accumulate(f);
	}
	else if (not is_negative() and f.is_negative()) {
		// a + (-b) = a - b
		// have to compute 'a - b'
		
		factoradic fb = -f;	// copy contains b
		
		if (*this >= fb) {
			// a >= b
			// call this->substract(x) where
			//     this contains max(a, b) = a
			//     x contains min(a, b) = b
			
			// this contains a
			// fb contains b
			
			__substract(fb); // valid because *this >= copy
			
			// after substract
			// this contains a - b
		}
		else {
			// a < b
			// call this->substract(x) where
			//     this contains max(a, b) = b
			//     x contains min(a, b) = a
			
			// before swap
			//     this contains a
			//     fb contains b
			
			swap(*this, fb);
			
			// after swap
			//     this contains b
			//     fb contains a
			
			__substract(fb); // valid because *this >= fb
			
			// this contains b - a
			
			neg = true;
			
			// now this contains -(b - a) = a - b
			// have computed what we wanted
		}
	}
	else if (is_negative() and not f.is_negative()) {
		// (-a) + b = b - a
		// have to compute 'b - a'
		
		neg = false; // this contains a
		
		if (*this >= f) {
			// a >= b
			// call this->substract(x) where
			//     this contains max(a, b) = a
			//     x contains min(a, b) = b
			
			__substract(f); // valid because *this >= f
			
			// this contains a - b
			
			neg = true;
			
			// this contains -(a - b) = b - a
			// have computed what we wanted
		}
		else {
			// a < b
			// call this->substract(x) where
			//     this contains max(a, b) = b
			//     x contains min(a, b) = a
			
			factoradic fb = f;
			
			// before swap
			//     this contains a
			//     fb contains b
			
			swap(*this, fb);
			
			// after swap
			//     this contains b
			//     fb contains a
			
			__substract(fb); // valid because *this >= fb
			
			// this contains b - a
			// have computed what we wanted
		}
	}
	else {
		// both are is_negative()ative: (-a) + (-b) = -(a + b)
		
		neg = false;		// *this = a = -(*this)
		factoradic pf = -f;	// pf = b = -f
		__accumulate(pf);	// *this = a + b
		neg = true;			// *this = -(a + b)
	}
}

void factoradic::__substract(const factoradic& f) {
	size_t l = 0;
	size_t carry = 0;
	
	while (l < radixs.size() and l < f.radixs.size()) {
		
		if (radixs[l] < f.radixs[l] + carry) {
			assert(radixs[l] + l + 1 >= f.radixs[l] + carry);
			
			radixs[l] = radixs[l] + l + 1 - (f.radixs[l] + carry);
			carry = 1;
		}
		else {
			radixs[l] = radixs[l] - (f.radixs[l] + carry);
			carry = 0;
		}
		
		++l;
	}
	
	// likewise in the accumulate algorithm, there are still
	// more radixs to deal with
	if (radixs.size() > f.radixs.size()) {
		for (; l < radixs.size(); ++l) {
			
			if (radixs[l] < carry) {
				assert(radixs[l] + l + 1 >= carry);
				
				radixs[l] = radixs[l] + l + 1 - carry;
				carry = 1;
			}
			else {
				radixs[l] = radixs[l] - carry;
				carry = 0;
			}
			
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

void factoradic::substract(const factoradic& f) {
	// a := *this
	// b := f
	
	if (not is_negative() and not f.is_negative()) {
		// a - b
		// have to compute 'a - b'
		
		if (*this >= f) {
			// a >= b
			// call this->substract(x) where
			//     this contains max(a, b) = a
			//     x contains min(a, b) = b
			
			// this contains a
			
			__substract(f); // valid because *this >= f
			
			// have computed what we wanted
		}
		else {
			// a < b
			// call this->substract(x) where
			//     this contains max(a, b) = b
			//     x contains min(a, b) = a
			
			factoradic fb = f;
			
			// before swap
			//     this contains a
			//     fb contains b
			
			swap(*this, fb);
			
			// after swap
			//     this contains b
			//     fb contains a
			
			__substract(fb); // valid because *this >= fb
			
			// this contains b - a
			
			neg = true;
			
			// this contains -(b - a) = a - b
			// have computed what we wanted
		}
	}
	else if (not is_negative() and f.is_negative()) {
		// a - (-b) = a + b
		__accumulate(f);
	}
	else if (is_negative() and not f.is_negative()) {
		// (-a) - b = -a - b = -(a + b)
		
		neg = false;		// a = positive *this
		__accumulate(f);	// *this = a + b
		neg = true;			// *this = -(a + b)
	}
	else {
		// (-a) - (-b) = -a + b = b - a
		// have to compute 'b - a'
		
		neg = false;		// this contains a
		factoradic fb = -f;	// fb contains b
		
		if (*this >= fb) {
			// a >= b
			// call this->substract(x) where
			//     this contains max(a, b) = a
			//     x contains min(a, b) = b
			
			__substract(fb); // valid because *this >= f (a >= b)
			
			// this contains a - b
			
			neg = true;
			
			// this contains -(a - b) = b - a
			// have computed what we wanted
		}
		else {
			// a < b
			// call this->substract(x) where
			//     this contains max(a, b) = b
			//     x contains min(a, b) = a
			
			// before swap
			//     this contains a
			//     fb contains b
			
			swap(*this, fb);
			
			// after swap
			//     this contains b
			//     fb contains a
			
			__substract(fb); // valid because *this >= fb
			
			// this contains b - a
			// have computed what we wanted
		}
	}
}

void factoradic::factoradic_multiply(const factoradic& f) {
	// if b is even:
	//     a*b = a*(b/2) + a*(b/2) = 2*(a*(b/2))
	// if b is odd:
	//     a*b = a*((b - 1)/2) + a*((b - 1)/2) + a = 2*(a*((b - 1)/2)) + a
	//	       = a*(b - 1) + a = a*b
	
	if (not f.is_one()) {
		factoradic fc = f;				// fc := b
		
		if (fc.is_even()) {
			fc.div2();					// fc := b/2
			factoradic_multiply(fc);	// this := a*(b/2)
			mult2();					// this := 2*a*(b/2) = a*b
		}
		else {
			factoradic copy = *this;	// copy := a
			fc.decrement();				// fc := b - 1
			fc.div2();					// fc := (b - 1)/2
			factoradic_multiply(fc);	// this := a*(b - 1)/2
			mult2();					// this := 2*(a*(b - 1)/2)
			__accumulate(copy);			// this := 2*(a*(b - 1)/2) + a = a*b
		}
	}
}

void factoradic::factoradic_power(const factoradic& f) {
	// if b is even:
	//     a^b = (a^(b/2))^2 = a^(2*(b/2)) = a^b
	// if b is odd:
	//     a^b = (a^((b - 1)/2))^2 * a = 
	//	       = a^(2*((b - 1)/2) + 1) = a^b
	
	if (not f.is_one()) {
		factoradic fc = f;				// fc := b
		
		if (fc.is_even()) {
			fc.div2();					// fc := b/2
			factoradic_power(fc);		// this := a^(b/2)
			square();					// this := a^(2*(b/2)) = a^b
		}
		else {
			factoradic copy = *this;	// copy := a
			fc.decrement();				// fc := b - 1
			fc.div2();					// fc := (b - 1)/2
			factoradic_power(fc);		// this := a^((b - 1)/2)
			square();					// this := (a^((b - 1)/2))^2 = a^(b - 1)
			factoradic_multiply(copy);	// this := a^(b - 1)*a = a^b
		}
	}
}

void factoradic::int_divide(size_t i) {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	size_t carry = 0;
	size_t r = radixs.size() - 1;
	
	do {
		size_t radix = radixs[r];
		size_t s = radix + carry;
		
		// i is a divisor of s
		if (s%i == 0) {
			size_t d = s/i;
			radixs[r] = d%(r + 1);
			carry = d - d%r;
		}
		else {
			
			if (s < i) {
				// radix + carry < i
				radixs[r] = 0;
				carry = s*r;
			}
			else {
				// radix + carry > i
				radixs[r] = s/i;
				carry = (s%i)*r;
			}
		}
		
		--r;
	}
	while (r > 0);
}

void factoradic::integer_divide(const integer& i) {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	integer carry = 0;
	size_t r = radixs.size() - 1;
	integer radix;
	
	do {
		radix.init_ui(radixs[r]);
		integer s = radix + carry;
		
		// i is a divisor of s
		if (s%i == 0) {
			// perfect division
			
			integer d = s;
			d /= i;
			radixs[r] = d%(r + 1);
			carry = d - d%r;
		}
		else {
			
			if (s < i) {
				// radix + carry < i
				radixs[r] = 0;
				carry = s*r;
			}
			else {
				// radix + carry > i
				radixs[r] = (s/i).to_uint();
				carry = (s%i)*r;
			}
		}
		
		--r;
	}
	while (r > 0);
}

void factoradic::mult2() {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	// double every radix of this number and then
	// propagate the carry from the lowest-weight radix
	// to the highest-weight radix direction (similar
	// to the procedure used in __accumulate)
	
	size_t l = 0;
	size_t carry = 0;
	while (l < radixs.size()) {
		
		// sum of the currently pointed radix and the carry
		size_t sum_rad = (radixs[l] << 1) + carry;
		
		// new radix
		size_t mod = sum_rad%(l + 1);
		radixs[l] = mod;
		
		// value of carry.
		// by construction:
		//     S := sum_rad
		//     mod := S%(l + 1)
		//     sum_rad > mod    <=>    S > S%(l + 1)
		carry = (sum_rad - mod)/(l + 1);
		
		++l;
	}
	
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

void factoradic::div2() {
	if (radixs.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	size_t carry = 0;
	size_t r = radixs.size() - 1;
	
	do {
		size_t radix = radixs[r];
		
		// calculate new r-th radix
		radixs[r] = ((carry + radix) >> 1);
		
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

void factoradic::square() {
	factoradic copy = *this;
	factoradic_multiply(copy);
}

