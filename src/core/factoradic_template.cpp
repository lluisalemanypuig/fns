/*********************************************************************
 * Factorial Number System
 * Copyright (C) 2018-2020  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 *
 ********************************************************************/

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
	const index_t R = digits.size();
	
	for (index_t r = R - 1; r > 0; --r) {
		digit_t di = digits[r];
		pos_dec = static_cast<T>((pos_dec + di)*r);
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
		digits = vector<digit_t>(1, 0);
		return;
	}
	
	T copy_n = n;
	digits = vector<digit_t>();
	
	digit_t i = 1;
	while (copy_n > 0) {
		digit_t d = digit_t(copy_n%i);
		digits.push_back(d);
		
		copy_n /= T(i);
		++i;
	}
}


