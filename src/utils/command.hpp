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

