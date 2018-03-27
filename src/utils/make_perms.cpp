#include "make_perms.hpp"

namespace make_perms {
	
	template<class T, class U>
	void kth_permutation(const U& k, const vector<T>& sorted_list, vector<T>& kth_perm) {
		// k in factoradic system number
		factoradic K(k);
		
		factoradic MAX;
		MAX.from_factorial(sorted_list.size());
		
		assert(K < MAX);
		
		vector<size_t> rs;
		K.get_radixs(rs, sorted_list.size());
		reverse(rs.begin(), rs.end());
		
		vector<T> list_copy = sorted_list;
		kth_perm = vector<T>(sorted_list.size());
		
		for (size_t i = 0; i < rs.size(); ++i) {
			kth_perm[i] = list_copy[ rs[i] ];
			list_copy.erase( list_copy.begin() + rs[i] );
		}
	}
	
}

