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
#include <assert.h>

/// C++ includes
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

/// Custom includes
#include "core/factoradic.hpp"
#include "core/type_defs.hpp"
#include "core/integer.hpp"

namespace make_perms {
	
	// Stores in 'perm' the 'k'-th permutation of the list 'L'.
	// k >= 0: index of the permutation to make
	// L: the list to obtain the permutation from
	template<class T, class U = integer>
	void kth_permutation(const U& k, const vector<T>& L, vector<T>& perm);
	
	// Stores in 'idx' the index of the permutation 'P' of list 'L'.
	// P: permutation
	// L: original list (P is a permutation of L).
	template<class T>
	void permutation_index(const vector<T>& P, const vector<T>& L, integer& idx);
	
}

#include "make_perms.cpp"
