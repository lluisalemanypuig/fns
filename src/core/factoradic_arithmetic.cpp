#include "factoradic.hpp"

/// PRIVATE

void factoradic::__increment() {
	digit_t carry = 1;
	index_t r = 1;
	
	while (r < digits.size() and carry > 0) {
		digit_t sum = digits[r] + carry;
		digit_t mod = sum%(r + 1);
		
		digits[r] = mod;
		
		assert(sum >= mod);
		carry = (sum - mod)/(r + 1);
		
		++r;
	}
	
	while (carry > 0) {
		digit_t mod = carry%(r + 1);
		digits.push_back(mod);
		
		assert(r + 1 >= mod);
		carry = ((r + 1) - mod)/(r + 1);
		
		// the radix also has to be incremented
		// albeit always out of bounds
		++r;
	}
}

void factoradic::__decrement() {
	index_t r = 0;
	
	digit_t carry = 1;
	while (r < digits.size() and carry > 0) {
		
		if (digits[r] < carry) {
			assert(digits[r] + r + 1 >= carry);
			
			digits[r] = (digits[r] + r + 1) - carry;
			carry = 1;
		}
		else {
			digits[r] = digits[r] - carry;
			carry = 0;
		}
		
		++r;
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
	index_t r = 0;
	digit_t carry = 0;
	
	while (r < digits.size() and r < f.digits.size()) {
		// if the sum of the two digits is greater than the greatest
		// radix allowed then compute the modulus of the result.
		// The greatest radix allowed is equal to the index
		
		// sum of the two currently pointed digits plus the carry
		digit_t sum_dig = digits[r] + f.digits[r] + carry;
		
		// new digit
		digit_t mod = sum_dig%(r + 1);
		digits[r] = mod;
		
		// value of carry.
		// by construction:
		//     S := sum_dig
		//     mod := S%(r + 1)
		//     sum_dig > mod    <=>    S > S%(r + 1)
		carry = (sum_dig - mod)/(r + 1);
		
		++r;
	}
	
	// if this number and f do not have the same length then keed adding
	// the carry until the full length of the longest number has been
	// iterated over
	if (digits.size() < f.digits.size()) {
		for (; r < f.digits.size(); ++r) {
			// sum of the currently pointed radix plus the carry
			digit_t sum_dig = f.digits[r] + carry;
			
			// new digit
			digit_t mod = sum_dig%(r + 1);
			digits.push_back(mod);
			
			// value of carry
			// by construction:
			//     S := sum_dig
			//     mod := S%(r + 1)
			//     sum_dig > mod    <=>    S > S%(r + 1)
			carry = (sum_dig - mod)/(r + 1);
		}
	}
	else if (digits.size() > f.digits.size()) {
		for (; r < digits.size(); ++r) {
			// sum of the currently pointed radix plus the carry
			digit_t sum_dig = digits[r] + carry;
			
			// new digit
			digit_t mod = sum_dig%(r + 1);
			digits[r] = mod;
			
			// value of carry
			// by construction:
			//     S := sum_dig
			//     mod := S%(r + 1)
			//     sum_dig > mod    <=>    S > S%(r + 1)
			carry = (sum_dig - mod)/(r + 1);
		}
	}
	
	// the two numbers may have different lengths, or not. In any case
	// there is still more carry
	while (carry > 0) {
		digit_t new_digit;
		if (carry > r) {
			new_digit = carry/r;
			carry = carry/r;
		}
		else {
			new_digit = carry;
			carry = 0;
		}
		digits.push_back(new_digit);
		++r;
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
	index_t r = 0;
	digit_t carry = 0;
	
	while (r < digits.size() and r < f.digits.size()) {
		
		if (digits[r] < f.digits[r] + carry) {
			assert(digits[r] + r + 1 >= f.digits[r] + carry);
			
			digits[r] = (digits[r] + r + 1) - (f.digits[r] + carry);
			carry = 1;
		}
		else {
			digits[r] = digits[r] - (f.digits[r] + carry);
			carry = 0;
		}
		
		++r;
	}
	
	// likewise in the accumulate algorithm, there are still
	// more digits to deal with
	if (digits.size() > f.digits.size()) {
		for (; r < digits.size(); ++r) {
			
			if (digits[r] < carry) {
				assert(digits[r] + r + 1 >= carry);
				
				digits[r] = (digits[r] + r + 1) - carry;
				carry = 1;
			}
			else {
				digits[r] = digits[r] - carry;
				carry = 0;
			}
			
		}
	}
	else if (digits.size() < f.digits.size()) {
		
		// even if this was true, since we are calling this function
		// assuming that (*this > f) this implies that the extra
		// digits in f are all 0s
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

void factoradic::int_divide(unsigned int i) {
	if (digits.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	digit_t carry = 0;
	index_t r = digits.size() - 1;
	
	do {
		digit_t radix = digits[r];
		digit_t s = radix + carry;
		
		// i is a divisor of s
		if (s%i == 0) {
			digit_t d = s/i;
			digits[r] = d%(r + 1);
			carry = d - d%r;
		}
		else {
			
			if (s < i) {
				// radix + carry < i
				digits[r] = 0;
				carry = s*r;
			}
			else {
				// radix + carry > i
				digits[r] = s/i;
				carry = (s%i)*r;
			}
		}
		
		--r;
	}
	while (r > 0);
}

void factoradic::integer_divide(const integer& i) {
	if (digits.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	index_t r = digits.size() - 1;
	
	integer carry = 0;
	integer radix;
	
	do {
		radix.init_ui(digits[r]);
		integer s = radix + carry;
		
		// i is a divisor of s
		if (s%i == 0) {
			// perfect division
			
			integer d = s;
			d /= i;
			digits[r] = digit_t(d%(r + 1));
			carry = d - digit_t(d%r);
		}
		else {
			
			if (s < i) {
				// radix + carry < i
				digits[r] = 0;
				carry = s*r;
			}
			else {
				// radix + carry > i
				digits[r] = (s/i).to_uint();
				carry = (s%i)*r;
			}
		}
		
		--r;
	}
	while (r > 0);
}

void factoradic::mult2() {
	if (digits.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	// double every radix of this number and then
	// propagate the carry from the lowest-weight radix
	// to the highest-weight radix direction (similar
	// to the procedure used in __accumulate)
	
	index_t r = 0;
	digit_t carry = 0;
	while (r < digits.size()) {
		
		// sum of the currently pointed radix and the carry
		digit_t sum = (digits[r] << 1) + carry;
		
		// new radix
		digit_t mod = sum%(r + 1);
		digits[r] = mod;
		
		// value of carry.
		// by construction:
		//     S := sum_rad
		//     mod := S%(r + 1)
		//     sum_rad > mod    <=>    S > S%(r + 1)
		carry = (sum - mod)/(r + 1);
		
		++r;
	}
	
	while (carry > 0) {
		digit_t new_digit;
		if (carry > r) {
			new_digit = carry/r;
			carry = carry/r;
		}
		else {
			new_digit = carry;
			carry = 0;
		}
		digits.push_back(new_digit);
		++r;
	}
	
}

void factoradic::div2() {
	if (digits.size() == 1) {
		// this number is 0 -> no work to do
		return;
	}
	
	index_t r = digits.size() - 1;
	digit_t carry = 0;
	do {
		digit_t radix = digits[r];
		
		// calculate new r-th radix
		digits[r] = ((carry + radix) >> 1);
		
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

