#pragma once

/// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

/// Custom includes
#include "core/integer.hpp"

class command {
	private:
		map<string, size_t> stats;
		
		// assume the command to have its action assigned
		istream& read_partial_command(istream& is);
		
		// reads the action and the partial command
		istream& read_full_command(istream& is);
		
	public:
		string action;
		string new_var;
		string var1, var2;
		string op;
		
		integer big_value;
		uint32_t small_value;
		
		list<command> sub_command;
		vector<string> sorted_list;
		vector<string> permutation;
		
	public:
		command();
		command(const string& a);
		~command();
		
		void clear();
		
		/// OPERATORS
		
		inline friend
		istream& operator>> (istream& is, command& c) {
			return c.read_full_command(is);
		}
};

