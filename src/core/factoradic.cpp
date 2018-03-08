#include "factoradic.hpp"

/// NON-CLASS

template<class T>
inline void __from_decimal(const T& n, vector<ushort>& radixs) {
	if (n == 0) {
		radixs = vector<ushort>(1, 0);
		return;
	}
	
	T copy_n = n;
	radixs = vector<ushort>();
	
	size_t i = 1;
	while (copy_n > 0) {
		ushort d = copy_n%i;
		radixs.push_back(d);
		
		copy_n /= i;
		++i;
	}
}

/// PUBLIC

factoradic::factoradic() { }
factoradic::factoradic(size_t n) {
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
	return *this;
}

// ADDITION

factoradic factoradic::operator+ (const factoradic& f) const {
	factoradic fc = *this;
	fc += f;
	return fc;
}

factoradic& factoradic::operator+= (const factoradic& f) {
	/*
	size_t l = 0;
	
	size_t carry = 0;
	while (l < radixs.length() and l < f.radixs.length()) {
		
		// if the sum of the two digits is greater than the greatest
		// radix allowed then compute the modulus of the result.
		// The greatest radix allowed is equal to the index
		
		// sum of the two currently pointed radixs plus the carry
		size_t sum_dig = size_t(radixs[l] - '0') + size_t(f.radixs[l] - '0') + carry;
		
		// new digit
		size_t mod = sum_dig%(l + 1);
		radixs[l] = char(mod + '0');
		
		// value of carry
		carry = (sum_dig - mod)/(l + 1);
		
		++l;
	}
	
	// if this number and f do not have the same length then keed adding
	// the carry until the full length of the longest number has been
	// iterated over
	if (radixs.length() < f.radixs.length()) {
		for (size_t it = l; it < f.radixs.length(); ++it) {
			
			// sum of the currently pointed radix plus the carry
			size_t sum_dig = size_t(f.radixs[it] - '0') + carry;
			
			// new digit
			size_t mod = sum_dig%(it + 1);
			radixs.push_back( char(mod + '0') );
			
			// value of carry
			carry = (sum_dig - mod)/(it + 1);
		}
	}
	else if (radixs.length() > f.radixs.length()) {
		for (size_t it = l; it < radixs.length(); ++it) {
			
			// sum of the currently pointed radix plus the carry
			size_t sum_dig = size_t(radixs[it] - '0') + carry;
			
			// new digit
			size_t mod = sum_dig%(it + 1);
			radixs[it] = char(mod + '0');
			
			// value of carry
			carry = (sum_dig - mod)/(it + 1);
		}
	}
	
	// the two numbers may have different lengths, or not. In any case
	// there is still more carry
	if (carry != 0) {
		
		while (carry > 0) {
			char new_digit;
			if (carry > l) {
				new_digit = carry/l + '0';
				carry = carry/l;
			}
			else {
				new_digit = carry + '0';
				carry = 0;
			}
			
			radixs.push_back(new_digit);
			++l;
		}
	}
	*/
	
	return *this;
}

// SUBSTRACTION

factoradic factoradic::operator- (const factoradic& f) const {
	factoradic fc = *this;
	fc -= f;
	return fc;
}

factoradic& factoradic::operator-= (const factoradic& f) {
	
	cout << "Substraction not implemented" << endl;
	
	return *this;
}

// MULTIPLICATION

factoradic factoradic::operator* (const factoradic& f) const {
	factoradic fc = *this;
	fc *= f;
	return fc;
}

factoradic& factoradic::operator*= (const factoradic& f) {
	
	cout << "Multiplication not implemented" << endl;
	
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

void factoradic::from_decimal(size_t k) {
	__from_decimal(k, radixs);
}

void factoradic::from_decimal(const integer& I) {
	__from_decimal(I, radixs);
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
}

string factoradic::to_string(size_t n_digits) const {
	string s;
	to_string(s, n_digits);
	return s;
}

void factoradic::to_string(string& s, size_t n_digits) const {
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
	
	reverse(s.begin(), s.end());
}

