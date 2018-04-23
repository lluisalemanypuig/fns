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
#include "utils/make_perms.hpp"

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
		make_perms::kth_permutation(idx, v, ith_perm);
		
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

void large_N(unsigned int N, unsigned int n_threads) {
	factoradic N_F;
	N_F.from_factorial(N);
	integer fact = N_F.to_integer();
	
	// # permutations per thread
	const integer n_perms_thread = fact/n_threads;
	
	vector<unsigned int> v(N);
	for (unsigned int i = 0; i < N; ++i) {
		v[i] = i + 1;
	}
	
	integer n_perms_processed = 0;
	
	#pragma omp parallel for num_threads(n_threads)
	for (unsigned int t = 0; t < n_threads; ++t) {
		
		// get the index of the k-th permutation
		integer idx = n_perms_thread*t;
		
		// compute k-th permutation
		vector<unsigned int> ith_perm;
		make_perms::kth_permutation(idx, v, ith_perm);
		
		// process all permutations with index [k, k + n_perms_thread]
		for (integer p = 0; p < n_perms_thread; ++p) {
			
			#pragma omp critical
			{
				cout << "thread " << t << " is processing permutation " << endl;
				cout << "    ";
				print(ith_perm, n_perms_thread*t + p);
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
		cout << "    N: to calculate all permutations of a list of N elements" << endl;
		cout << "    t: number of threads. Must be a divisor of N!" << endl;
		return 1;
	}
	
	const unsigned int N = atoi(argv[1]);
	const unsigned int n_threads = atoi(argv[2]);
	
	if (N <= 14) {
		small_N(N, n_threads);
	}
	else {
		large_N(N, n_threads);
	}
}

