#pragma once

/// C++ includes
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

/// Custom includes
#include "core/factoradic.hpp"
#include "core/integer.hpp"

namespace make_perms {
	
	template<class T, class U = integer>
	void kth_permutation(const U& k, const vector<T>& sorted_list, vector<T>& perm);
	
}

#include "make_perms.cpp"
