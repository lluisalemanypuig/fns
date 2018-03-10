/// C++ includes
#include <iostream>
#include <map>
using namespace std;

/// Custom includes
#include "core/operations.hpp"
#include "core/factoradic.hpp"
#include "core/integer.hpp"
#include "core/defs.hpp"
using namespace operations;

// Global variables used only to avoid declaring the same variables
// over and over again.
factoradic f1, f2, R;
string new_var, var1, op, var2;

void define_variable(memory& data) {
	cin >> new_var >> var1 >> op >> var2;
	
	apply_op(data, var1, var2, f1, f2, R, op);
	
	if (data.find(new_var) == data.end()) {
		data.insert( make_pair(new_var, R) );
	}
	else {
		data[new_var] = R;
	}
	
	cout << "    "
		 << new_var << " := "
		 << var1 << op << var2 << " = "
		 << f1 << op << f2 << " = "
		 << R << " (" << R.to_decimal() << ")"
		 << endl;
}

void compare_variables(const memory& data) {
	string var1, op, var2;
	cin >> var1 >> op >> var2;
	
	apply_comp(data, var1, var2, f1, f2, R, op);
}

void list_all_variables(const memory& data) {
	cout << endl;
	for (const auto& P : data) {
		cout << "    " << P.first << " = " << P.second << " (" << P.second.to_decimal() << ")" << endl;
	}
	cout << endl;
}

int main(int argc, char *argv[]) {
	memory data;
	
	string option;
	cout << "> ";
	
	while (cin >> option and option != "exit") {
		
		if (option == "var") {
			string varname;
			cin >> varname;
			
			integer value;
			cin >> value;
			
			data.insert( make_pair(varname, factoradic(value)) );
		}
		else if (option == "op") {
			cin >> var1 >> op >> var2;
			apply_op(data, var1, var2, f1, f2, R, op);
		}
		else if (option == "def") {
			define_variable(data);
		}
		else if (option == "cmp") {
			compare_variables(data);
		}
		else if (option == "ls") {
			list_all_variables(data);
		}
		else {
			cerr << "Error: unknown command '" << option << "'" << endl;
		}
		
		cout << "> ";
	}
	
}

