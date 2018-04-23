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
