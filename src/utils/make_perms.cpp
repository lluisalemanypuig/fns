#include "make_perms.hpp"

namespace make_perms {
	
	template<class T, class U>
	void kth_permutation(const U& k, const vector<T>& L, vector<T>& kth_perm) {
		// k in factoradic system number
		factoradic K(k);
		
		factoradic MAX;
		MAX.from_factorial(L.size());
		
		assert(K < MAX);
		
		vector<size_t> rs;
		K.get_radixs(rs, L.size());
		reverse(rs.begin(), rs.end());
		
		vector<T> list_copy = L;
		kth_perm = vector<T>(L.size());
		
		for (size_t i = 0; i < rs.size(); ++i) {
			kth_perm[i] = list_copy[ rs[i] ];
			list_copy.erase( list_copy.begin() + rs[i] );
		}
	}
	
	template<class T>
	inline
	size_t find_pos_element(const vector<T>& L, const T& e) {
		size_t p = L.size() + 1;
		for (size_t i = 0; i < L.size() and p == L.size() + 1; ++i) {
			if (L[i] == e) {
				p = i;
			}
		}
		return p;
	}
	
	template<class T>
	void permutation_index(const vector<T>& P, const vector<T>& L, integer& idx) {
		assert(P.size() == L.size());
		const size_t N = L.size();
		
		// radixs of the new number in big-endian
		vector<size_t> radixs(N);
		
		// copy the list used as reference to be
		// able to remove values from it
		vector<T> copy_list = L;
		
		size_t P_it = 0;
		while (P_it < N and copy_list.size() > 0) {
			
			const T& elem = P[P_it];
			size_t pos = find_pos_element(copy_list, elem);
			assert(pos != copy_list.size() + 1);
			
			radixs[P_it] = pos;
			copy_list.erase(copy_list.begin() + pos);
			
			++P_it;
		}
		
		factoradic fidx(radixs, false, false);
		fidx.to_integer(idx);
	}
	
}

