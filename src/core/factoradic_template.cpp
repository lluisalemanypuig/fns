#include "factoradic.hpp"

template<class T>
void factoradic::__from_decimal(const T& n) {
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

