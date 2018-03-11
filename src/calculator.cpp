/// C++ includes
#include <iostream>
#include <map>
using namespace std;

/// Custom includes
#include "core/operations.hpp"
#include "core/factoradic.hpp"
#include "core/integer.hpp"
#include "core/defs.hpp"
#include "utils/time.hpp"
using namespace operations;

void print_usage() {
	cout << endl;
	
	cout << "    Factoradic calculator command description" << endl;
	cout << endl;
	cout << "      > var s v: stores in memory a variable with name 's' and contents" << endl;
	cout << "            'v', where 'v' is a number in decimal notation." << endl;
	cout << "      > op s1 O s2: operates contents of s1 and s2 with operator X." << endl;
	cout << "            The operators supported are +,-" << endl;
	cout << "            s1 and s2 must be either a variable name or a number in base 10" << endl;
	cout << "      > cmp s1 C s2: compares the contents of s1 and s2 with C." << endl;
	cout << "            The comparisons supported are >,>=,==,<=,<" << endl;
	cout << "            s1 and s2 must be either a variable name or a number in base 10" << endl;
	cout << "      > halve s1: apply ad-hoc algorithm to halve the contents of s1." << endl;
	cout << "            s1 must be either a variable name or a number in base 10" << endl;
	cout << "      > double s1: apply ad-hoc algorithm to double the contents of s1." << endl;
	cout << "            s1 must be either a variable name or a number in base 10" << endl;
	cout << "      > inc s1: apply ad-hoc algorithm to increment by 1 the contents of s1." << endl;
	cout << "            s1 must be either a variable name or a number in base 10" << endl;
	cout << "      > dec s1: apply ad-hoc algorithm to decrement by 1 the contents of s1." << endl;
	cout << "            s1 must be either a variable name or a number in base 10" << endl;
	cout << "      > def varname s1 X s2: creates a variable with name 'varname'" << endl;
	cout << "            with contents the result of operating the contents of s1" << endl;
	cout << "            and s2 with operator X." << endl;
	cout << "            s1 and s2 must be either a variable name or a number in base 10" << endl;
	cout << "      > ls: lists all variables and their content" << endl;
	cout << "      > exit: leave the program ('CTRL + D' is also valid)" << endl;
	cout << "      > help: print this message" << endl;
	
	cout << endl;
	cout << "    For variable names:" << endl;
	cout << "        When asked for a variable name, the user is allowed to enter" << endl;
	cout << "        any string. This string is then interpreted: if there is a variable" << endl;
	cout << "        in the memory with the same name as the string then that variable's" << endl;
	cout << "        will be used. If not, then the string will be interpreted as a number" << endl;
	cout << "        in decimal notation." << endl;
	cout << "        This allows to have variables named '1', but if the variable named '1'" << endl;
	cout << "        has value '3' (in decimal) then the command:" << endl;
	cout << "            op 1 + 3" << endl;
	cout << "        gives as result '6' (decimal) and not '4' (decimal)" << endl;
	cout << endl;
	
	cout << "    For numbers in base 10 notation:" << endl;
	cout << "        They are represented with a string of characters from '0' to '9'" << endl;
	cout << "        with an optional leading '-' to indicate a negative number." << endl;
	cout << "        Floating point numbers are not allowed" << endl;
	cout << endl;
}

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
	
	double begin, end;
	bool valid_option;
	
	while (cin >> option and option != "exit") {
		valid_option = true;
		begin = timing::now();
		
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
		else if (option == "cmp") {
			compare_variables(data);
		}
		else if (option == "halve") {
			cin >> var1;
			halve_value(data, var1, f1, R);
		}
		else if (option == "double") {
			cin >> var1;
			double_value(data, var1, f1, R);
		}
		else if (option == "inc") {
			cin >> var1;
			increment_value(data, var1, f1, R);
		}
		else if (option == "dec") {
			cin >> var1;
			decrement_value(data, var1, f1, R);
		}
		else if (option == "def") {
			define_variable(data);
		}
		else if (option == "ls") {
			list_all_variables(data);
		}
		else if (option == "help") {
			print_usage();
		}
		else {
			cerr << "Error: unknown command '" << option << "'" << endl;
			valid_option = false;
		}
		
		end = timing::now();
		if (valid_option) {
			cout << "    In " << timing::elapsed_time(begin, end) << " s" << endl;
		}
		
		cout << "> ";
	}
	
}

