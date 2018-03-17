#include "factoradic.hpp"

/// PRIVATE

template<class T>
void factoradic::accumulate(const T& f) {
	if (not is_negative() and not f.is_negative()) {
		// a + b
		
		__accumulate(f);
	}
	else if (not is_negative() and f.is_negative()) {
		// a + (-b) = a - b
		// have to compute 'a - b'
		
		T fb = -f;	// copy contains b
		
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
			
			T fb = f;
			
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
		
		neg = false;	// *this = a = -(*this)
		T pf = -f;		// pf = b = -f
		*this += pf;	// *this = a + b
		neg = true;		// *this = -(a + b)
	}
}

template<class T>
void factoradic::substract(const T& f) {
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
			
			T fb = f;
			
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
		
		neg = false;	// a = positive *this
		*this += f;		// *this = a + b
		neg = true;		// *this = -(a + b)
	}
	else {
		// (-a) - (-b) = -a + b = b - a
		// have to compute 'b - a'
		
		neg = false;		// this contains a
		T fb = -f;	// fb contains b
		
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

template<class T>
void factoradic::integer_fast_multiply(const T& i) {
	// if b is even:
	//     a*b = a*(b/2) + a*(b/2) = 2*(a*(b/2))
	// if b is odd:
	//     a*b = a*((b - 1)/2) + a*((b - 1)/2) + a = 2*(a*((b - 1)/2)) + a
	//	       = a*(b - 1) + a = a*b
	
	if (i != 1) {
		// fc := b
		T fc = i;
		
		if (fc%2 == 0) {
			fc /= 2;
			factoradic_fast_multiply(fc);
			mult2();
		}
		else {
			factoradic copy = *this;		// copy := a
			--fc;							// fc := b - 1
			fc /= 2;						// fc := (b - 1)/2
			factoradic_fast_multiply(fc);	// this := a*(b - 1)/2
			mult2();						// this := 2*(a*(b - 1)/2)
			__accumulate(copy);				// this := 2*(a*(b - 1)/2) + a = a*b
		}
	}
}

template<class T>
void factoradic::__to_decimal(T& i) const {
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

/// PUBLIC

template<class T>
void factoradic::from_decimal(const T& n) {
	if (n == 0) {
		radixs = vector<ushort>(1, 0);
		return;
	}
	
	T copy_n = n;
	if (copy_n < 0) {
		copy_n = -copy_n;
		neg = true;
	}
	else {
		neg = false;
	}
	
	radixs = vector<ushort>();
	
	size_t i = 1;
	while (copy_n > 0) {
		ushort d = copy_n%i;
		radixs.push_back(d);
		
		copy_n /= i;
		++i;
	}
}


