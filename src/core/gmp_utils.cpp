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

#include "gmp_utils.hpp"

namespace gmp_utils {

	void mpz_pow_mpz(mpz_t& r, const mpz_t& b, const mpz_t& e) {
		if (mpz_fits_ulong_p(e)) {
			mpz_pow_ui(r, b, mpz_get_ui(e));
			return;
		}
		
		if (mpz_even_p(e)) {
			// r = (b^(e/2))^2
			
			mpz_t e_half;
			mpz_init(e_half);
			mpz_div_ui(e_half, e, 2);	// e_half = e/2
			
			mpz_pow_mpz(r, b, e_half);	// r = b^(e/2)
			mpz_mul(r, r, r);			// r = (b^(e/2))^2 = b^e
			mpz_clear(e_half);
		}
		else {
			// r = (b^(e - 1))*b
			
			mpz_t e_minus_one;
			mpz_init(e_minus_one);
			mpz_sub_ui(e_minus_one, e, 1);	// e_minus_one = e - 1
			
			mpz_pow_mpz(r, b, e_minus_one);	// r = b^(e - 1)
			mpz_mul(r, r, b);				// r = (b^(e - 1))*b = b^e
			mpz_clear(e_minus_one);
		}
	}
	
	void zero(mpz_t& z) {
		mpz_init_set_ui(z, 0);
	}
	
	void one(mpz_t& o) {
		mpz_init_set_ui(o, 1);
	}
	
	size_t bytes(const mpz_t& v) {
		return sizeof(mp_limb_t)*v[0]._mp_alloc;
	}
	
}

