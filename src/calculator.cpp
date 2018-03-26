/// C includes
#include <string.h>

/// C++ includes
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

/// Custom includes
#include "core/factoradic.hpp"
#include "core/integer.hpp"
#include "utils/make_perms.hpp"
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
	cout << "      > def varname s1 X s2: creates a variable with name 'varname'" << endl;
	cout << "            with contents the result of operating the contents of s1" << endl;
	cout << "            and s2 with operator X." << endl;
	cout << "            s1 and s2 must be either a variable name or a number in base 10" << endl;
	cout << "      > cmp s1 C s2: compares the contents of s1 and s2 with C." << endl;
	cout << "            The comparisons supported are >,>=,==,<=,<" << endl;
	cout << "            s1 and s2 must be either a variable name or a number in base 10" << endl;
	cout << endl;
	cout << "      > inc v: apply ad-hoc algorithm to increment by 1 the contents of v." << endl;
	cout << "            v must be a variable name" << endl;
	cout << "      > dec v: apply ad-hoc algorithm to decrement by 1 the contents of v." << endl;
	cout << "            v must be a variable name" << endl;
	cout << "      > double v: apply ad-hoc algorithm to double the contents of v." << endl;
	cout << "            v must be a variable name" << endl;
	cout << "      > halve v: apply ad-hoc algorithm to halve the contents of v." << endl;
	cout << "            v must be a variable name" << endl;
	cout << "      > square v: apply ad-hoc algorithm to square of the contents of v." << endl;
	cout << "            v must be a variable name" << endl;
	cout << "      > even v: checks if the contents of v is even." << endl;
	cout << "            v must be a variable name" << endl;
	cout << endl;
	cout << "      > ff v n: compute the factorial of n! in factorial base." << endl;
	cout << "            n must be a number in base 10" << endl;
	cout << endl;
	cout << "      > repeat n OPTION: repeats the given COMMAND n times." << endl;
	cout << "            n must be a number in base 10" << endl;
	cout << "      > { COMMAND_1 COMMAND_2 ... COMMAND_N }: allows defining a block of instructions." << endl;
	cout << "            This can be useful to repeat a list of options in block" << endl;
	cout << "      > // ... //: this is a comment. The contents of '...' may be arbitrarily long." << endl;
	cout << endl;
	cout << "      > shrink: removes all leading zeros from all variables in order to save space" << endl;
	cout << "      > shrink-var: removes all leading zeros from all variables in order to save space" << endl;
	cout << "      > del-var v: delete variable with name 'v'" << endl;
	cout << "      > del: removes all all variables" << endl;
	cout << endl;
	cout << "      > ls: lists all variables and their content (in factorial base)" << endl;
	cout << "      > ls-dec: lists all variables and their content (in base 10)" << endl;
	cout << "      > print v: prints the contets of variable v (in factorial base)" << endl;
	cout << "      > print-dec v: prints the contents of v (in base 10)" << endl;
	cout << endl;
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
bool print_prompt;
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
	
	caddress it;
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
	factoradic R;
	
	if (c.action == "var") {
		if (data.find(c.new_var) == data.end()) {
			data.insert( make_pair(c.new_var, factoradic(c.big_value)) );
		}
		else {
			data[c.new_var] = factoradic(c.big_value);
		}
	}
	else if (c.action == "op") {
		apply_op(data, c.var1, c.var2, c.op, R);
	}
	else if (c.action == "cmp") {
		apply_comp(data, c.var1, c.var2, c.op);
	}
	else if (c.action == "inc") {
		increment_value(data, c.var1);
	}
	else if (c.action == "dec") {
		decrement_value(data, c.var1);
	}
	else if (c.action == "double") {
		double_value(data, c.var1);
	}
	else if (c.action == "halve") {
		halve_value(data, c.var1);
	}
	else if (c.action == "square") {
		square_value(data, c.var1);
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
	else if (c.action == "make-permutation") {
		
		// permutation index
		factoradic Fx;
		integer Ix;
		
		bool is_var_x = read_var(data, c.var1, Fx, Ix);
		
		vector<string> kth_perm;
		if (is_var_x) {
			make_perms::kth_permutation(Fx, c.list_elems, kth_perm);
		}
		else {
			make_perms::kth_permutation(Ix, c.list_elems, kth_perm);
		}
		
		for (const string& e : kth_perm) {
			cout << e << " ";
		}
		cout << endl;
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
	else if (c.action == "def") {
		apply_op(data, c.var1, c.var2, c.op, R, false);
		
		if (data.find(c.new_var) == data.end()) {
			data.insert( make_pair(c.new_var, R) );
		}
		else {
			data[c.new_var] = R;
		}
	}
	else if (c.action == "del-var") {
		memory::iterator it;
		if ((it = data.find(c.var1)) != data.end()) {
			data.erase(it);
		}
		else {
			cout << "    Error: variable '" << c.var1 << "' does not exist" << endl;
			print_time = false;
		}
	}
	else if (c.action == "del") {
		data.clear();
	}
	else if (c.action == "shrink-var") {
		memory::iterator it;
		if ((it = data.find(c.var1)) != data.end()) {
			it->second.shrink();
		}
		else {
			cout << "    Error: variable '" << c.var1 << "' does not exist" << endl;
			print_time = false;
		}
	}
	else if (c.action == "shrink") {
		for (address it = data.begin(); it != data.end(); ++it) {
			it->second.shrink();
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
		print_time = false;
	}
	else if (c.action == "repeat") {
		bool exit_calc = false;
		
		factoradic f;
		integer I;
		bool is_var = read_var(data, c.var1, f, I);
		
		if (is_var) {
			f.to_integer(I);
		}
		
		for (integer i = 0; i < I and not exit_calc; ++i) {
			exit_calc = execute_command(*c.sub_command.begin());
		}
		
		return exit_calc;
	}
	else if (c.action == "{") {
		bool exit_calc = false;
		
		for (auto it = c.sub_command.begin(); it != c.sub_command.end() and not exit_calc; ++it) {
			
			exit_calc = execute_command(*it);
		}
		
		return exit_calc;
	}
	else if (c.action == "//") {
		// comment -> do nothing
		print_time = false;
		print_prompt = false;
	}
	else if (c.action == "exit") {
		print_time = false;
		return true;
	}
	else {
		cout << endl;
		cout << "    Error: option '" << c.action << "' not recognised" << endl;
		cout << endl;
		print_time = false;
	}
	
	return false;
}

void interactive() {
	cout << "Welcome to the factoradic-base number calculator." << endl;
	cout << "Type 'help' to see the available options." << endl;
	cout << endl;
	
	command main_command;
	cout << "> ";
	bool exit_calc = false;
	
	while (not exit_calc and cin >> main_command) {
		print_time = true;
		print_prompt = true;
		double begin = timing::now();
		
		exit_calc = execute_command(main_command);
		
		double end = timing::now();
		if (print_time) {
			cout << "    In " << timing::elapsed_time(begin, end) << " s" << endl;
		}
		
		if (not exit_calc and print_prompt) {
			cout << "> ";
		}
	}
	
	main_command.clear();
}

void execute_program(const string& program_file) {
	ifstream fin;
	fin.open(program_file.c_str());
	if (not fin.is_open()) {
		cerr << "Error: could not program file '" << program_file << "'" << endl;
		return;
	}
	
	double begin = timing::now();
	
	command main_command;
	bool exit_calc = false;
	
	while (not exit_calc and fin >> main_command) {
		exit_calc = execute_command(main_command);
	}
	
	double end = timing::now();
	
	main_command.clear();
	fin.close();
	
	cout << "    In " << timing::elapsed_time(begin, end) << " s" << endl;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Error: you must specify a mode of execution:" << endl;
		cerr << "    -i: interactive" << endl;
		cerr << "    -l: load a program. Must specify also the name of the" << endl;
		cerr << "        containing the instructions" << endl;
		return 1;
	}
	
	if (strcmp(argv[1], "-i") == 0) {
		interactive();
	}
	else if (strcmp(argv[1], "-l") == 0) {
		
		if (argc == 2) {
			cerr << "    Error: the name of the file containing the instructions" << endl;
			cerr << "           must be specified" << endl;
			return 1;
		}
		
		string program_file = string(argv[2]);
		execute_program(program_file);
	}
}

