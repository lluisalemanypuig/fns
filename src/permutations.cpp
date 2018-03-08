/// C includes
#include <sys/resource.h>
#include <stdlib.h>
#include <omp.h>

/// C++ includes
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/// Custom includes
#include "core/factoradic.hpp"

template<typename T, typename U = size_t>
void print(const vector<T>& v, const U& idx) {
	cout << idx << ":";
	for (const auto& u : v) {
		cout << " " << u;
	}
	cout << endl;
}

// v contains the sorted list of elements!
template<class T, class U = size_t>
void kth_permutation(const vector<T>& v, vector<T>& kth_perm, const U& k) {
	// k in factoradic system number
	factoradic K(k);
	
	vector<ushort> rs;
	K.get_radixs(rs, v.size());
	reverse(rs.begin(), rs.end());
	
	vector<T> list_copy = v;
	kth_perm = vector<T>(v.size());
	
	for (size_t i = 0; i < rs.size(); ++i) {
		kth_perm[i] = list_copy[ rs[i] ];
		list_copy.erase( list_copy.begin() + rs[i] );
	}
}

void small_N(size_t N, size_t n_threads) {
	static const size_t factorials[13] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
	
	// # permutations per thread
	const size_t n_perms_thread = factorials[N]/n_threads;
	
	vector<size_t> v(N);
	for (size_t i = 0; i < N; ++i) {
		v[i] = i + 1;
	}
	
	#pragma omp parallel for num_threads(n_threads)
	for (size_t t = 0; t < n_threads; ++t) {
		
		// get the index of the k-th permutation
		size_t idx = t*n_perms_thread;
		
		// compute k-th permutation
		vector<size_t> ith_perm;
		kth_permutation(v, ith_perm, idx);
		
		// process all permutations with index [k, k + n_perms_thread]
		for (size_t p = 0; p < n_perms_thread; ++p) {
			
			// 'critical' is used only for printing
			#pragma omp critical
			{
				cout << "thread " << t << " is processing permutation " << endl;
				cout << "    ";
				print(ith_perm, t*n_perms_thread + p);
			}
			
			next_permutation(ith_perm.begin(), ith_perm.end());
		}
	}
}

void large_N(size_t N, size_t n_threads) {
	factoradic N_F;
	N_F.from_factorial(N);
	integer fact = N_F.to_decimal();
	
	// # permutations per thread
	const integer n_perms_thread = fact/n_threads;
	
	vector<size_t> v(N);
	for (size_t i = 0; i < N; ++i) {
		v[i] = i + 1;
	}
	
	#pragma omp parallel for num_threads(n_threads)
	for (size_t t = 0; t < n_threads; ++t) {
		
		// get the index of the k-th permutation
		integer idx = n_perms_thread*t;
		
		// compute k-th permutation
		vector<size_t> ith_perm;
		kth_permutation(v, ith_perm, idx);
		
		// process all permutations with index [k, k + n_perms_thread]
		for (integer p = 0; p < n_perms_thread; ++p) {
			
			// 'critical' is used only for printing
			#pragma omp critical
			{
				cout << "thread " << t << " is processing permutation " << endl;
				cout << "    ";
				print(ith_perm, n_perms_thread*t + p);
			}
			
			next_permutation(ith_perm.begin(), ith_perm.end());
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Incorrect number of parameters:" << endl;
		cout << "    N: to calculate all permutations of a list of N elements" << endl;
		cout << "    t: number of threads" << endl;
		return 1;
	}
	
	const size_t N = atoi(argv[1]);
	const size_t n_threads = atoi(argv[2]);
	
	if (N <= 12) {
		small_N(N, n_threads);
	}
	else {
		large_N(N, n_threads);
	}
}

