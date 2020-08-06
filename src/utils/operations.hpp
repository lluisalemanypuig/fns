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
#include <string>
#include <map>
using namespace std;

/// Custom includes
#include "core/factoradic.hpp"
#include "core/integer.hpp"
#include "defs.hpp"

namespace operations {
	
	enum op_type {
		none = -1,
		var_var, var_num, num_var, num_num
	};
	
	string op_type_to_string(const op_type& t);
	
	// If 'var' contains a variable's name stored in data, store the
	// contents in 'f' and return true. Otherwise, interpret the string
	// as a base-10 integer number, store the value in 'i' and return false
	bool read_var(const memory& data, const string& var, factoradic& f, integer& i);
	
	void apply_op(const memory& data, const string& var1, const string& var2, const string& op, factoradic& R, bool print = true);
	void apply_comp(const memory& data, const string& var1, const string& var2, const string& op);
	
	void increment_value(memory& data, const string& var1);
	void decrement_value(memory& data, const string& var1);
	void double_value(memory& data, const string& var1);
	void halve_value(memory& data, const string& var1);
	void square_value(memory& data, const string& var1);
	
}

