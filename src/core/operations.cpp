#include "operations.hpp"

namespace operations {
	
	op_type read(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2) {
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var1 = false;
		address avar1 = data.find(var1);
		if (avar1 == data.end()) {
			is_var1 = false;
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
		}
		
		// check whether second string is a variable or not.
		// If not, convert the string into a number
		bool is_var2 = false;
		address avar2 = data.find(var2);
		if (avar2 == data.end()) {
			f2 = factoradic(var2);
			is_var2 = false;
		}
		else {
			f2 = avar2->second;
		}
		
		op_type T;
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

	op_type add(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& sum) {
		op_type T = read(data, var1, var2, f1, f2);
		sum = f1 + f2;
		return T;
	}

	op_type sub(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& sub) {
		op_type T = read(data, var1, var2, f1, f2);
		sub = f1 - f2;
		return T;
	}

	op_type mul(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& mul) {
		op_type T = read(data, var1, var2, f1, f2);
		mul = f1*f2;
		return T;
	}

	op_type div(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& div) {
		op_type T = read(data, var1, var2, f1, f2);
		div = f1/f2;
		return T;
	}
	
	op_type comp_gt(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp) {
		op_type T = read(data, var1, var2, f1, f2);
		comp = (f1 > f2);
		return T;
	}
	
	op_type comp_ge(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp) {
		op_type T = read(data, var1, var2, f1, f2);
		comp = (f1 >=f2);
		return T;
	}
	
	op_type comp_eq(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp) {
		op_type T = read(data, var1, var2, f1, f2);
		comp = (f1 == f2);
		return T;
	}
	
	op_type comp_le(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp) {
		op_type T = read(data, var1, var2, f1, f2);
		comp = (f1 <= f2);
		return T;
	}
	
	op_type comp_lt(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, bool& comp) {
		op_type T = read(data, var1, var2, f1, f2);
		comp = (f1 < f2);
		return T;
	}
	
	op_type apply_op(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& R, const string& op) {
		op_type T;
		
		string newop = op;
		if (newop == "+" or newop == "-") {
			newop = " " + newop + " ";
		}
		
		if (newop == " + ") {
			T = add(data, var1, var2, f1, f2, R);
		}
		else if (newop == " - ") {
			T = sub(data, var1, var2, f1, f2, R);
		}
		else if (newop == "*") {
			T = mul(data, var1, var2, f1, f2, R);
		}
		else if (newop == "/") {
			T = div(data, var1, var2, f1, f2, R);
		}
		
		cout << "    " << var1 << newop << var2 << " = "
			 << "(" << f1 << ")" << newop << "(" << f2 << ")"
			 << " = " << R << " (" << R.to_decimal() << ")" << endl;
		
		return T;
	}
	
	op_type apply_comp(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& R, const string& op) {
		op_type T;
		bool comp;
		
		string newop = " " + op + " ";
		
		if (newop == " > ") {
			T = comp_gt(data, var1, var2, f1, f2, comp);
		}
		else if (newop == " >= ") {
			T = comp_ge(data, var1, var2, f1, f2, comp);
		}
		else if (newop == " == ") {
			T = comp_eq(data, var1, var2, f1, f2, comp);
		}
		else if (newop == " <= ") {
			T = comp_le(data, var1, var2, f1, f2, comp);
		}
		else if (newop == " < ") {
			T = comp_lt(data, var1, var2, f1, f2, comp);
		}
		
		cout << "    " << var1 << newop << var2 << " --> "
			 << "(" << f1 << ")" << newop << "(" << f2 << ")"
			 << " --> " << (comp ? "true" : "false") << endl;
		
		return T;
	}
	
	void halve_value(memory& data, const string& var1, factoradic& f1, factoradic& R) {
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		address avar1 = data.find(var1);
		if (avar1 == data.end()) {
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
			is_var = true;
		}
		
		R = f1;
		R.halve();
		
		if (is_var) {
			cout << "    " << var1 << "/2 = " << "(" << f1 << ")" << "/2 = " << R << " (" << R.to_decimal() << ")" << endl;
			data.find(var1)->second = R;
		}
		else {
			cout << "    " << "(" << f1 << ")" << "/2 = " << R << " (" << R.to_decimal() << ")" << endl;
		}
	}
	
	void double_value(memory& data, const string& var1, factoradic& f1, factoradic& R) {
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		address avar1 = data.find(var1);
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
			cout << "    " << var1 << "*2 = " << "(" << f1 << ")" << "*2 = " << R << " (" << R.to_decimal() << ")" << endl;
			data.find(var1)->second = R;
		}
		else {
			cout << "    " << "(" << f1 << ")" << "*2 = " << R << " (" << R.to_decimal() << ")" << endl;
		}
	}
	
	void increment_value(memory& data, const string& var1, factoradic& f1, factoradic& R) {
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		address avar1 = data.find(var1);
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
			cout << "    " << var1 << " + 1 = " << "(" << f1 << ")" << " + 1 = " << R << " (" << R.to_decimal() << ")" << endl;
			data.find(var1)->second = R;
		}
		else {
			cout << "    " << "(" << f1 << ")" << " + 1 = " << R << " (" << R.to_decimal() << ")" << endl;
		}
	}
	
	void decrement_value(memory& data, const string& var1, factoradic& f1, factoradic& R) {
		// check whether first string is a variable or not.
		// If not, convert the string into a number
		bool is_var = false;
		address avar1 = data.find(var1);
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
			cout << "    " << var1 << " - 1 = " << "(" << f1 << ")" << " - 1 = " << R << " (" << R.to_decimal() << ")" << endl;
			data.find(var1)->second = R;
		}
		else {
			cout << "    " << "(" << f1 << ")" << " - 1 = " << R << " (" << R.to_decimal() << ")" << endl;
		}
	}
	
}

