#include "command.hpp"

/// PRIVATE

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
	else if (action == "ff") {
		is >> new_var;
		is >> small_value;
	}
	else if (action == "make-permutation") {
		is >> var1;			// permutation index
		is >> small_value;	// num elems
		list_elems = vector<string>(small_value);
		for (string& elem : list_elems) {
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
	else if (action == "//") {
		while (is >> var1 and var1 != "//") { }
	}
	return is;
}

istream& command::read_full_command(istream& is) {
	is >> action;
	return read_partial_command(is);
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


