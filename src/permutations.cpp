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

template<typename T>
void print(const vector<T>& v, size_t idx) {
	cout << idx << ":";
	for (const auto& u : v) {
		cout << " " << u;
	}
	cout << endl;
}

// v contains the sorted list of elements!
template<class T>
void kth_permutation(const vector<T>& v, vector<T>& kth_perm, size_t k) {
	
	// k in factoradic system number
	factoradic K(k);
	string k_fact = K.to_string();
	
	vector<T> list_copy = v;
	kth_perm = vector<T>(v.size());
	
	for (size_t i = 0; i < k_fact.length(); ++i) {
		kth_perm[i] = list_copy[ k_fact[i] - '0' ];
		list_copy.erase(list_copy.begin() + k_fact[i] - '0');
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Incorrect number of parameters:" << endl;
		cout << "N: to calculate all permutations of a list of N elements" << endl;
		cout << "t: number of threads" << endl;
		return 1;
	}
	
	const size_t N = atoi(argv[1]);
	const size_t n_threads = atoi(argv[2]);
	const size_t factorials[13] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
	
	vector<size_t> v(N);
	for (size_t i = 0; i < N; ++i) {
		v[i] = i + 1;
	}
	
	const size_t n_perms_thread = factorials[N]/n_threads;
	
	#pragma omp parallel for num_threads(n_threads)
	for (size_t t = 0; t < n_threads; ++t) {
		
		size_t idx = t*n_perms_thread;
		
		vector<size_t> ith_perm;
		kth_permutation(v, ith_perm, idx);
		
		for (size_t p = 0; p < n_perms_thread; ++p) {
			
			// this is used only for printing
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

