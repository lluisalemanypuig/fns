/*********************************************************************
 * Factorial Number System
 * Copyright (C) 2018  Lluís Alemany Puig
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

#include "command.hpp"

/// PRIVATE

inline bool is_comment(const string& str) {
	if (str.length() <= 1) {
		return false;
	}
	if (str == "//") {
		return true;
	}
	if (str.substr(0,2) == "//") {
		return true;
	}
	if (str.substr(str.length() - 2, 2) == "//") {
		return true;
	}
	return false;
}

istream& command::read_partial_command(istream& is) {
	if (action == "var") {
		is >> new_var;
		is >> big_value;
	}
	else if (action == "op") {
		is >> var1 >> op >> var2;
	}
	else if (action == "cmp") {
		is >> var1 >> op >> var2;
	}
	else if (action == "inc") {
		is >> var1;
	}
	else if (action == "dec") {
		is >> var1;
	}
	else if (action == "double") {
		is >> var1;
	}
	else if (action == "halve") {
		is >> var1;
	}
	else if (action == "square") {
		is >> var1;
	}
	else if (action == "factorial") {
		is >> new_var;
		is >> small_value;
	}
	else if (action == "make-permutation") {
		is >> var1;			// permutation index
		is >> small_value;	// num elems
		sorted_list = vector<string>(small_value);
		for (string& elem : sorted_list) {
			is >> elem;
		}
	}
	else if (action == "permutation-index") {
		is >> small_value;	// num elems
		permutation = vector<string>(small_value);
		for (string& elem : permutation) {
			is >> elem;
		}
		sorted_list = vector<string>(small_value);
		for (string& elem : sorted_list) {
			is >> elem;
		}
	}
	else if (action == "def") {
		is >> new_var >> var1 >> op >> var2;
	}
	else if (action == "even") {
		is >> var1;
	}
	else if (action == "shrink-var") {
		is >> var1;
	}
	else if (action == "shrink") { }
	else if (action == "del-var") {
		is >> var1;
	}
	else if (action == "del") { }
	else if (action == "ls") { }
	else if (action == "ls-dec") { }
	else if (action == "print" or action == "print-dec") {
		is >> var1;
	}
	else if (action == "endlines") { }
	else if (action == "trailing") { }
	else if (action == "exit") { }
	else if (action == "help") { }
	else if (action == "repeat") {
		is >> var1;
		
		if (sub_command.size() > 0) {
			clear();
		}
		
		command new_command;
		is >> new_command;
		sub_command.insert(sub_command.begin(), new_command);
	}
	else if (action == "{") {
		
		// read list of commands
		string ac;
		is >> ac;
		command new_command(ac);
		new_command.read_partial_command(is);
		sub_command.insert(sub_command.end(), new_command);
		
		while (is >> ac and ac != "}") {
			command new_command(ac);
			new_command.read_partial_command(is);
			sub_command.insert(sub_command.end(), new_command);
		}
	}
	else if (is_comment(action)) {
		action = "//";
		while (is >> var1 and not is_comment(var1))
		{ }
	}
	else {
		cerr << "Error: action '" << action << "' not recognised" << endl;
	}
	return is;
}

istream& command::read_full_command(istream& is) {
	if (is >> action) {
		return read_partial_command(is);
	}
	return is;
}

/// PUBLIC

command::command() {
	big_value.init_si(0);
}

command::command(const string& a) {
	big_value.init_si(0);
	action = a;
}

command::~command() {
	clear();
}

void command::clear() {
	sub_command.clear();
}


