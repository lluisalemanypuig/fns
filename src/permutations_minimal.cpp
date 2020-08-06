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

/// C includes
#include <sys/resource.h>
#include <stdlib.h>
#include <omp.h>

/// C++ includes
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

typedef uint32_t digit_t;

class factoradic {
	public:
		factoradic() {
			m_neg = false;
			from_int(0);
		}
		factoradic(int64_t i) {
			from_long_int(i);
		}
		~factoradic() = default;

		bool operator> (const factoradic& f) const {
			if (m_neg and not f.m_neg) {
				// (< 0) >? (> 0) -> No
				return false;
			}
			if (not m_neg and f.m_neg) {
				// (> 0) >? (< 0) -> Yes
				return true;
			}

			// consider both numbers positive, and compute the comparison.
			// *this is greater than k if the first highest-weight non-zero
			// radix in *this is greater than the corresponding radix in k

			// positions of the first non-zero radix for *this and k
			uint32_t pnZ_t = m_digits.size() - 1;
			while (pnZ_t > 0 and m_digits[pnZ_t] == 0) {
				--pnZ_t;
			}
			uint32_t pnZ_k = f.m_digits.size() - 1;
			while (pnZ_k > 0 and f.m_digits[pnZ_k] == 0) {
				--pnZ_k;
			}

			// pointers not in the same position. Now we can decide '>'
			if (pnZ_t != pnZ_k) {

				// if both are positive:
				if (not m_neg and not f.m_neg) {
					return pnZ_t > pnZ_k;
				}

				// if both are negative:
				return pnZ_t < pnZ_k;
			}

			// when in the same position, move the pointers to the first
			// pair of different digits

			while (pnZ_t > 0 and m_digits[pnZ_t] == f.m_digits[pnZ_t]) {
				--pnZ_t;
			}

			// if both are positive:
			if (not m_neg and not f.m_neg) {
				return m_digits[pnZ_t] > f.m_digits[pnZ_t];
			}

			// if both are negative:
			return m_digits[pnZ_t] < f.m_digits[pnZ_t];
		}
		bool operator< (const factoradic& f) const {
			return f > *this;
		}

		void get_digits(vector<digit_t>& rs, size_t n_digits = 0) const {
			rs = m_digits;
			if (n_digits > 0) {
				while (rs.size() < n_digits) {
					rs.push_back(0);
				}
			}
		}

		// convert a base-10 number into factoradic number system
		void from_int(int32_t i) {
			uint32_t copy_i;
			if (i < 0) {
				copy_i = -i;
				m_neg = true;
			}
			else {
				m_neg = false;
				copy_i = i;
			}
			__from_decimal(copy_i);
		}
		void from_long_int(int64_t i) {
			uint64_t copy_i;
			if (i < 0) {
				copy_i = -i;
				m_neg = true;
			}
			else {
				m_neg = false;
				copy_i = i;
			}
			__from_decimal(copy_i);
		}
		// build the number in factoradic system for the value n!
		void from_factorial(uint32_t i) {
			m_digits = vector<digit_t>(i, 0);
			m_digits.push_back(1);
		}

	private:
		// digits of this number: little-endian format
		vector<digit_t> m_digits;

		// neg = true -> this number is negative
		// neg = false -> this number is positive
		bool m_neg;

		// convert into factorial number system any positive number in base 10
		template<class T> void __from_decimal(const T& n) {
			if (n == 0) {
				m_digits = vector<digit_t>(1, 0);
				return;
			}

			T copy_n = n;
			m_digits = vector<digit_t>();

			digit_t i = 1;
			while (copy_n > 0) {
				digit_t d = digit_t(copy_n%i);
				m_digits.push_back(d);

				copy_n /= T(i);
				++i;
			}
		}
};

// Stores in 'perm' the 'k'-th permutation of the list 'L'.
// k >= 0: index of the permutation to make
// L: the list to obtain the permutation from
template<class U, class T>
void kth_permutation(const U& k, const vector<T>& L, vector<T>& kth_perm) {
	// k in factoradic system number
	factoradic K(k);

	factoradic MAX;
	MAX.from_factorial(uint32_t(L.size()));

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

template<typename T, typename U = size_t>
void print(const vector<T>& v, const U& idx) {
	cout << idx << ":";
	for (const auto& u : v) {
		cout << " " << u;
	}
	cout << endl;
}

void small_N(unsigned int N, unsigned int n_threads) {
	static const size_t factorials[15] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600, 6227020800, 87178291200};
	
	// # permutations per thread
	const size_t n_perms_thread = factorials[N]/n_threads;
	
	vector<unsigned int> v(N);
	for (unsigned int i = 0; i < N; ++i) {
		v[i] = i + 1;
	}
	
	size_t n_perms_processed = 0;
	
	#pragma omp parallel for num_threads(n_threads)
	for (unsigned int t = 0; t < n_threads; ++t) {
		
		// get the index of the k-th permutation
		size_t idx = t*n_perms_thread;
		
		// compute k-th permutation
		vector<unsigned int> ith_perm;
		kth_permutation(idx, v, ith_perm);
		
		// process all permutations with index [k, k + n_perms_thread]
		for (size_t p = 0; p < n_perms_thread; ++p) {
			
			#pragma omp critical
			{
				cout << "thread " << t << " is processing permutation " << endl;
				cout << "    ";
				print(ith_perm, t*n_perms_thread + p);
				++n_perms_processed;
			}
			
			next_permutation(ith_perm.begin(), ith_perm.end());
		}
	}
	
	cout << "Total amount of permutations processed= " << n_perms_processed << endl;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Incorrect number of parameters:" << endl;
		cout << "Usage: ./process_permutations N t" << endl;
		cout << "    N: to calculate all permutations of a list of N elements" << endl;
		cout << "    t: number of threads. Must be a divisor of N!" << endl;
		return 1;
	}
	
	const unsigned int N = atoi(argv[1]);
	const unsigned int n_threads = atoi(argv[2]);
	
	if (N <= 15) {
		small_N(N, n_threads);
	}
	else {
		cout << "N=" << N << " is way too much..." << endl;
	}
}

