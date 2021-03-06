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

#include "operations.hpp"

namespace operations {
	
	string op_type_to_string(const op_type& t) {
		string s = "none";
		if (t == op_type::var_var) {
			s = "var_var";
		}
		else if (t == op_type::var_num) {
			s = "var_num";
		}
		else if (t == op_type::num_var) {
			s = "num_var";
		}
		else if (t == op_type::num_num) {
			s = "num_num";
		}
		return s;
	}
	
	bool read_var(const memory& data, const string& var, factoradic& f, integer& i) {
		bool is_var = true;
		caddress avar1 = data.find(var);
		if (avar1 == data.end()) {
			is_var = false;
			i = integer(var);
		}
		else {
			f = avar1->second;
		}
		return is_var;
	}
	
	op_type read(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2) {
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var1 = read_var(data, var1, f1, i1);
		
		// check whether second string is a variable or not.
		// If not, convert the string into a number
		bool is_var2 = read_var(data, var2, f2, i2);
		
		op_type T = op_type::none;
		if (is_var1 and is_var2) {
			T = var_var;
		}
		else if (is_var1 and not is_var2) {
			T = var_num;
		}
		else if (not is_var1 and is_var2) {
			T = num_var;
		}
		else if (not is_var1 and not is_var2) {
			T = num_num;
		}
		
		return T;
	}

	op_type add(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, factoradic& sum) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			sum = f1 + f2;
		}
		else if (T == var_num) {
			sum = f1 + i2;
		}
		else if (T == num_var) {
			sum = f2 + i1;
		}
		else if (T == num_num) {
			sum = i1 + i2;
		}
		return T;
	}

	op_type sub(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, factoradic& sub) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			sub = f1 - f2;
		}
		else if (T == var_num) {
			sub = f1 - i2;
		}
		else if (T == num_var) {
			sub = f2 - i1;
		}
		else if (T == num_num) {
			sub = i1 - i2;
		}
		return T;
	}

	op_type mul(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, factoradic& mul) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			mul = f1 * f2;
		}
		else if (T == var_num) {
			mul = f1 * i2;
		}
		else if (T == num_var) {
			mul = f2 * i1;
		}
		else if (T == num_num) {
			mul = i1 * i2;
		}
		return T;
	}

	op_type div(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, factoradic& div) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			div = f1 / f2;
		}
		else if (T == var_num) {
			div = f1 / i2;
		}
		else if (T == num_var) {
			div = f2 / i1;
		}
		else if (T == num_num) {
			div = i1 / i2;
		}
		return T;
	}
	
	op_type power(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, factoradic& p) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			p = f1 ^ f2;
		}
		else if (T == var_num) {
			p = f1 ^ i2;
		}
		else if (T == num_var) {
			p = f2 ^ i1;
		}
		else if (T == num_num) {
			p = i1 ^ i2;
		}
		return T;
	}
	
	op_type comp_gt(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, bool& comp) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			comp = f1 > f2;
		}
		else if (T == var_num) {
			comp = f1 > i2;
		}
		else if (T == num_var) {
			comp = f2 > i1;
		}
		else if (T == num_num) {
			comp = i1 > i2;
		}
		return T;
	}
	
	op_type comp_ge(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, bool& comp) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			comp = f1 >= f2;
		}
		else if (T == var_num) {
			comp = f1 >= i2;
		}
		else if (T == num_var) {
			comp = f2 >= i1;
		}
		else if (T == num_num) {
			comp = i1 >= i2;
		}
		return T;
	}
	
	op_type comp_eq(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, bool& comp) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			comp = f1 == f2;
		}
		else if (T == var_num) {
			comp = f1 == i2;
		}
		else if (T == num_var) {
			comp = f2 == i1;
		}
		else if (T == num_num) {
			comp = i1 == i2;
		}
		return T;
	}
	
	op_type comp_le(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, bool& comp) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			comp = f1 <= f2;
		}
		else if (T == var_num) {
			comp = f1 <= i2;
		}
		else if (T == num_var) {
			comp = f2 <= i1;
		}
		else if (T == num_num) {
			comp = i1 <= i2;
		}
		return T;
	}
	
	op_type comp_lt(const memory& data, const string& var1, const string& var2, factoradic& f1, integer& i1, factoradic& f2, integer& i2, bool& comp) {
		op_type T = read(data, var1, var2, f1, i1, f2, i2);
		if (T == var_var) {
			comp = f1 < f2;
		}
		else if (T == var_num) {
			comp = f1 < i2;
		}
		else if (T == num_var) {
			comp = f2 < i1;
		}
		else if (T == num_num) {
			comp = i1 < i2;
		}
		return T;
	}
	
	void apply_op(const memory& data, const string& var1, const string& var2, const string& op, factoradic& R, bool print) {
		factoradic f1, f2;
		integer i1, i2;
		op_type T = op_type::none;
		
		string newop = op;
		if (newop == "+" or newop == "-") {
			newop = " " + newop + " ";
		}
		
		if (newop == " + ") {
			T = add(data, var1, var2, f1, i1, f2, i2, R);
		}
		else if (newop == " - ") {
			T = sub(data, var1, var2, f1, i1, f2, i2, R);
		}
		else if (newop == "*") {
			T = mul(data, var1, var2, f1, i1, f2, i2, R);
		}
		else if (newop == "/") {
			T = div(data, var1, var2, f1, i1, f2, i2, R);
		}
		else if (newop == "^") {
			T = power(data, var1, var2, f1, i1, f2, i2, R);
		}
		else {
			cerr << "    Operator: '" << op << "' invalid" << endl;
		}
		
		if (print) {
			if (T == op_type::var_var) {
				cout << "    " << var1 << newop << var2 << " = "
					 << "(" << f1 << ")" << newop << "(" << f2 << ")"
					 << " = " << R << " (" << R.to_integer() << ")" << endl;
			}
			else if (T == op_type::var_num) {
				cout << "    " << var1 << newop << var2 << " = "
					 << "(" << f1 << ")" << newop << i2
					 << " = " << R << " (" << R.to_integer() << ")" << endl;
			}
			else if (T == op_type::num_var) {
				cout << "    " << var1 << newop << var2 << " = "
					 << i1 << newop << "(" << f2 << ")"
					 << " = " << R << " (" << R.to_integer() << ")" << endl;
			}
			else if (T == op_type::num_num) {
				cout << "    " << var1 << newop << var2 << " = "
					 << i1 << newop << i2
					 << " = " << R << " (" << R.to_integer() << ")" << endl;
			}
		}
	}
	
	void apply_comp(const memory& data, const string& var1, const string& var2, const string& op) {
		factoradic f1, f2;
		integer i1, i2;
		op_type T = op_type::none;
		bool comp = true;
		
		string newop = " " + op + " ";
		
		if (newop == " > ") {
			T = comp_gt(data, var1, var2, f1, i1, f2, i2, comp);
		}
		else if (newop == " >= ") {
			T = comp_ge(data, var1, var2, f1, i1, f2, i2, comp);
		}
		else if (newop == " == ") {
			T = comp_eq(data, var1, var2, f1, i1, f2, i2, comp);
		}
		else if (newop == " <= ") {
			T = comp_le(data, var1, var2, f1, i1, f2, i2, comp);
		}
		else if (newop == " < ") {
			T = comp_lt(data, var1, var2, f1, i1, f2, i2, comp);
		}
		else {
			cerr << "    Operator: '" << op << "' invalid" << endl;
		}
		
		if (T == op_type::var_var) {
			cout << "    " << var1 << newop << var2
				 << " --> " << (comp ? "true" : "false") << endl;
		}
		else if (T == op_type::var_num) {
			cout << "    " << var1 << newop << i2
				 << " --> " << (comp ? "true" : "false") << endl;
		}
		else if (T == op_type::num_var) {
			cout << "    " << i1 << newop << var2
				 << " --> " << (comp ? "true" : "false") << endl;
		}
		else if (T == op_type::num_num) {
			cout << "    " << i1 << newop << i2
				 << " --> " << (comp ? "true" : "false") << endl;
		}
	}
	
	void increment_value(memory& data, const string& var1) {
		factoradic f1, R;
		
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		caddress avar1 = data.find(var1);
		if (avar1 == data.end()) {
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
			is_var = true;
		}
		
		R = f1;
		++R;
		
		if (is_var) {
			data.find(var1)->second = R;
		}
	}
	
	void decrement_value(memory& data, const string& var1) {
		factoradic f1, R;
		
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		caddress avar1 = data.find(var1);
		if (avar1 == data.end()) {
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
			is_var = true;
		}
		
		R = f1;
		--R;
		
		if (is_var) {
			data.find(var1)->second = R;
		}
	}
	
	void double_value(memory& data, const string& var1) {
		factoradic f1, R;
		
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		caddress avar1 = data.find(var1);
		if (avar1 == data.end()) {
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
			is_var = true;
		}
		
		R = f1;
		R.mult2();
		
		if (is_var) {
			data.find(var1)->second = R;
		}
	}
	
	void halve_value(memory& data, const string& var1) {
		factoradic f1, R;
		
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		caddress avar1 = data.find(var1);
		if (avar1 == data.end()) {
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
			is_var = true;
		}
		
		R = f1;
		R.div2();
		
		if (is_var) {
			data.find(var1)->second = R;
		}
	}
	
	void square_value(memory& data, const string& var1) {
		factoradic f1, R;
		
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		caddress avar1 = data.find(var1);
		if (avar1 == data.end()) {
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
			is_var = true;
		}
		
		R = f1;
		R.square();
		
		if (is_var) {
			data.find(var1)->second = R;
		}
	}
	
}

