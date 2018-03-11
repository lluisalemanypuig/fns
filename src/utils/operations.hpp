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
	
	op_type read(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2);
	op_type add(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& sum);
	op_type sub(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& sub);
	op_type mul(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& mul);
	op_type div(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& div);
	
	op_type comp_gt(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp);
	op_type comp_ge(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp);
	op_type comp_eq(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp);
	op_type comp_le(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp);
	op_type comp_lt(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp);
	
	op_type apply_op(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& R, const string& op);
	op_type apply_comp(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& R, const string& op);
	
	void halve_value(memory& data, const string& var1, factoradic& f1, factoradic& R);
	void double_value(memory& data, const string& var1, factoradic& f1, factoradic& R);
	void increment_value(memory& data, const string& var1, factoradic& f1, factoradic& R);
	void decrement_value(memory& data, const string& var1, factoradic& f1, factoradic& R);
	
}

