/// C++ includes
#include <iostream>
#include <map>
using namespace std;

/// Custom includes
#include "core/factoradic.hpp"
#include "core/integer.hpp"
#include "utils/operations.hpp"
#include "utils/command.hpp"
#include "utils/time.hpp"
#include "utils/defs.hpp"
using namespace operations;

void print_usage() {
	cout << endl;
	
	cout << "    Factoradic calculator command description" << endl;
	cout << endl;
	
	cout << "      > var v s: stores in memory a variable with name 'v' and contents" << endl;
	cout << "            's', where 's' is a number in base 10." << endl;
	cout << "      > op s1 X s2: operates contents of s1 and s2 with operator X." << endl;
	cout << "            The operators supported are +,-,*" << endl;
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
	cout << "      > ff v n: compute the factorial of n! in factorial base." << endl;
	cout << "            n must be a number in base 10" << endl;
	cout << "      > def varname s1 X s2: creates a variable with name 'varname'" << endl;
	cout << "            with contents the result of operating the contents of s1" << endl;
	cout << "            and s2 with operator X." << endl;
	cout << "            s1 and s2 must be either a variable name or a number in base 10" << endl;
	cout << "      > del v: delete variable with name 'v'" << endl;
	cout << "      > even s1: checks if the contents of 's1' is even." << endl;
	cout << "            s1 must be either a variable name or a number in base 10" << endl;
	cout << "      > repeat n OPTION: repeats the given OPTION n times." << endl;
	cout << "            n must be a number in base 10" << endl;
	cout << "      > ls: lists all variables and their content (in factorial base)" << endl;
	cout << "      > ls-dec: lists all variables and their content (in base 10)" << endl;
	cout << "      > print v: prints the contets of variable v (in factorial base)" << endl;
	cout << "      > print-dec v: prints the contents of v (in base 10)" << endl;
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

/// <GLOBAL VARIABLES>
// *Some* Global variables. Only used here
memory data;
bool print_time;
/// </GLOBAL VARIABLES>

void print_variable(const string& name, const factoradic& f) {
	cout << "    " << name << " = ";
	cout << f << endl;
}

void print_variable(const string& name, const integer& i) {
	cout << "    " << name << " = ";
	cout << i << endl;
}

void print_variable(const string& name, bool fact) {
	
	memory::const_iterator it;
	if ((it = data.find(name)) == data.end()) {
		cout << endl;
		cerr << "    Error: variable with name '" << name << "' does not exist." << endl;
		cout << endl;
		return;
	}
	
	cout << endl;
	if (fact) {
		print_variable(name, it->second);
	}
	else {
		print_variable(name, it->second.to_integer());
	}
	cout << endl;
}

void list_all_variables(bool fact) {
	cout << endl;
	for (const auto& P : data) {
		if (fact) {
			print_variable(P.first, P.second);
		}
		else {
			print_variable(P.first, P.second.to_integer());
		}
	}
	cout << endl;
}

inline
bool execute_command(const command& c) {
	factoradic f1, f2, R;
	
	if (c.action == "var") {
		if (data.find(c.new_var) == data.end()) {
			data.insert( make_pair(c.new_var, factoradic(c.big_value)) );
		}
		else {
			data[c.new_var] = factoradic(c.big_value);
		}
	}
	else if (c.action == "op") {
		apply_op(data, c.var1, c.var2, f1, f2, R, c.op);
	}
	else if (c.action == "cmp") {
		apply_comp(data, c.var1, c.var2, f1, f2, R, c.op);
	}
	else if (c.action == "halve") {
		halve_value(data, c.var1, f1, R);
	}
	else if (c.action == "double") {
		double_value(data, c.var1, f1, R);
	}
	else if (c.action == "inc") {
		increment_value(data, c.var1, f1, R);
	}
	else if (c.action == "dec") {
		decrement_value(data, c.var1, f1, R);
	}
	else if (c.action == "ff") {
		factoradic F;
		F.from_factorial(c.small_value);
		
		if (data.find(c.new_var) == data.end()) {
			data.insert( make_pair(c.new_var, F) );
		}
		else {
			data[c.new_var] = F;
		}
	}
	else if (c.action == "def") {
		apply_op(data, c.var1, c.var2, f1, f2, R, c.op);
	
		if (data.find(c.new_var) == data.end()) {
			data.insert( make_pair(c.new_var, R) );
		}
		else {
			data[c.new_var] = R;
		}
		
		cout << "    "
			 << c.new_var << " := "
			 << c.var1 << c.op << c.var2 << " = "
			 << f1 << c.op << f2 << " = "
			 << R << " (" << R.to_integer() << ")"
			 << endl;
	}
	else if (c.action == "del") {
		memory::iterator it;
		if ((it = data.find(c.var1)) != data.end()) {
			data.erase(it);
		}
		else {
			cout << "    Error: variable '" << c.var1 << "' does not exist" << endl;
			print_time = false;
		}
	}
	else if (c.action == "even") {
		memory::iterator it;
		if ((it = data.find(c.var1)) != data.end()) {
			const factoradic& F = it->second;
			cout << "    Is '" << F << " (" << F.to_integer() << ") even? ";
			cout << (F.is_even() ? "Yes" : "No") << endl;
		}
		else {
			cout << "Error: variable '" << c.var1 << "' does not exist" << endl;
			print_time = false;
		}
	}
	else if (c.action == "ls") {
		list_all_variables(true);
		print_time = false;
	}
	else if (c.action == "ls-dec") {
		list_all_variables(false);
		print_time = false;
	}
	else if (c.action == "print") {
		print_variable(c.var1, true);
		print_time = false;
	}
	else if (c.action == "print-dec") {
		print_variable(c.var1, false);
		print_time = false;
	}
	else if (c.action == "help") {
		print_usage();
	}
	else if (c.action == "repeat") {
		bool exit_calc = false;
		
		for (size_t i = 0; i < c.small_value and not exit_calc; ++i) {
			exit_calc = execute_command(*c.sub_command);
		}
		
		return exit_calc;
	}
	else if (c.action == "exit") {
		print_time = false;
		return true;
	}
	
	return false;
}

int main(int argc, char *argv[]) {
	cout << "Welcome to the factoradic-base number calculator." << endl;
	cout << "Type 'help' to see the available options." << endl;
	cout << endl;
	
	// set cout properties
	cout.setf(ios::unitbuf); // unbuffered
	
	command main_command;
	cout << "> ";
	bool exit_calc = false;
	
	while (not exit_calc and cin >> main_command) {
		print_time = true;
		double begin = timing::now();
		
		exit_calc = execute_command(main_command);
		
		double end = timing::now();
		if (print_time) {
			cout << "    In " << timing::elapsed_time(begin, end) << " s" << endl;
		}
		
		if (not exit_calc) {
			cout << "> ";
		}
	}
	
	main_command.clear();
}

