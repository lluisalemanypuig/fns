/*********************************************************************
 * Factorial Number System
 * Copyright (C) 2018  Lluís Alemany Puig
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

#include "make_perms.hpp"

namespace make_perms {
	
	template<class T, class U>
	void kth_permutation(const U& k, const vector<T>& L, vector<T>& kth_perm) {
		// k in factoradic system number
		factoradic K(k);
		
		factoradic MAX;
		MAX.from_factorial(uint32_t(L.size()));
		
		assert(K < MAX);
		
		vector<digit_t> ds;
		K.get_digits(ds, L.size());
		reverse(ds.begin(), ds.end());
		
		vector<T> list_copy = L;
		kth_perm = vector<T>(L.size());
		
		for (size_t i = 0; i < ds.size(); ++i) {
			kth_perm[i] = list_copy[ ds[i] ];
			list_copy.erase( list_copy.begin() + ds[i] );
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
		vector<digit_t> radixs(N);
		
		// copy the list used as reference to be
		// able to remove values from it
		vector<T> copy_list = L;
		
		size_t P_it = 0;
		while (P_it < N and copy_list.size() > 0) {
			
			const T& elem = P[P_it];
			size_t pos = find_pos_element(copy_list, elem);
			assert(pos != copy_list.size() + 1);
			
			radixs[P_it] = digit_t(pos);
			copy_list.erase(copy_list.begin() + pos);
			
			++P_it;
		}
		
		factoradic fidx(radixs, false, false);
		fidx.to_integer(idx);
	}
	
}

