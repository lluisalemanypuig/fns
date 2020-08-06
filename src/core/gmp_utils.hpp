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

#pragma once

/// C includes
#include <gmp.h>

namespace gmp_utils {
	
	/* Other arithmetic operations */
	
	void mpz_pow_mpz(mpz_t& r, const mpz_t& b, const mpz_t& e);
	
	/* Fast initializers */
	
	void zero(mpz_t& z);
	void one(mpz_t& o);
	
	/* Getters of mpz_t objects */
	
	size_t bytes(const mpz_t& v);
	
}
