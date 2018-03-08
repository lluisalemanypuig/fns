#include "operations.hpp"

namespace operations {
	
	op_type read(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2) {
		bool is_var1 = false;
		bool is_var2 = false;
		
		address avar1 = data.find(var1);
		address avar2 = data.find(var2);
		
		if (avar1 == data.end()) {
			is_var1 = false;
			f1 = factoradic(var1);
		}
		else {
			f1 = avar1->second;
		}
		
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
	
	op_type apply_op(const memory& data, const string& var1, const string& var2, factoradic& f1, factoradic& f2, factoradic& R, const string& op) {
		op_type T;
		
		if (op == " + ") {
			T = add(data, var1, var2, f1, f2, R);
		}
		else if (op == " - ") {
			T = sub(data, var1, var2, f1, f2, R);
		}
		else if (op == "*") {
			T = mul(data, var1, var2, f1, f2, R);
		}
		else if (op == "/") {
			T = div(data, var1, var2, f1, f2, R);
		}
		
		cout << "    " << var1 << op << var2 << " = " << f1 << op << f2 << " = " << R << " (" << R.to_decimal() << ")" << endl;
		
		return T;
	}
	
}

