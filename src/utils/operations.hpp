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
	
	void apply_op(const memory& data, const string& var1, const string& var2, const string& op, factoradic& R, bool print = true);
	void apply_comp(const memory& data, const string& var1, const string& var2, const string& op);
	
	void halve_value(memory& data, const string& var1);
	void double_value(memory& data, const string& var1);
	void increment_value(memory& data, const string& var1);
	void decrement_value(memory& data, const string& var1);
	
}

