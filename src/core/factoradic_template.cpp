#include "factoradic.hpp"

/// PRIVATE

template<class T>
void factoradic::integer_multiply(const T& i) {
	// if b is even:
	//     a*b = a*(b/2) + a*(b/2) = 2*(a*(b/2))
	// if b is odd:
	//     a*b = a*((b - 1)/2) + a*((b - 1)/2) + a = 2*(a*((b - 1)/2)) + a
	//	       = a*(b - 1) + a = a*b
	
	if (i != 1) {
		T fc = i;						// fc := b
		
		if (fc%2 == 0) {
			fc /= 2;					// fc := b/2
			integer_multiply(fc);		// this := a*(b/2)
			mult2();					// this := 2*a*(b/2) = a*b
		}
		else {
			factoradic copy = *this;	// copy := a
			--fc;						// fc := b - 1
			fc /= 2;					// fc := (b - 1)/2
			integer_multiply(fc);		// this := a*(b - 1)/2
			mult2();					// this := 2*(a*(b - 1)/2)
			__accumulate(copy);			// this := 2*(a*(b - 1)/2) + a = a*b
		}
	}
}

template<class T>
void factoradic::integer_power(const T& i) {
	// if b is even:
	//     a^b = (a^(b/2))^2 = a^(2*(b/2)) = a^b
	// if b is odd:
	//     a^b = (a^((b - 1)/2))^2 * a = 
	//	       = a^(2*((b - 1)/2) + 1) = a^b
	
	if (i > 1) {
		T fc = i;						// fc := b
		
		if (fc%2 == 0) {
			fc /= 2;					// fc := b/2
			integer_power(fc);			// this := a^(b/2)
			square();					// this := a^(2*(b/2)) = a^b
		}
		else {
			factoradic copy = *this;	// copy := a
			--fc;						// fc := b - 1
			fc /= 2;					// fc := (b - 1)/2
			integer_power(fc);			// this := a^((b - 1)/2)
			square();					// this := (a^((b - 1)/2))^2 = a^(b - 1)
			factoradic_multiply(copy);	// this := a^(b - 1)*a = a^b
		}
	}
}

template<class T>
void factoradic::__to_integer(T& i) const {
	T pos_dec = 0;
	const size_t R = radixs.size();
	
	for (size_t r = R - 1; r > 0; --r) {
		size_t di = radixs[r];
		pos_dec = (pos_dec + di)*r;
	}
	
	i = pos_dec;
	if (neg) {
		i = -i;
	}
}

/// PUBLIC

template<class T>
void factoradic::__from_decimal(const T& n) {
	if (n == 0) {
		radixs = vector<size_t>(1, 0);
		return;
	}
	
	T copy_n = n;
	radixs = vector<size_t>();
	
	size_t i = 1;
	while (copy_n > 0) {
		size_t d = copy_n%i;
		radixs.push_back(d);
		
		copy_n /= i;
		++i;
	}
}


