#pragma once

/// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

/// Custom includes
#include "core/integer.hpp"

class command {
	private:
		map<string, size_t> stats;
		
	public:
		string action;
		string new_var;
		string var1, var2;
		string op;
		
		integer big_value;
		size_t small_value;
		
		command *sub_command;
	
	public:
		command();
		~command();
		
		void clear();
		
		/// OPERATORS
		
		inline friend
		istream& operator>> (istream& is, command& c) {
			is >> c.action;
			if (c.action == "var") {
				is >> c.new_var;
				is >> c.big_value;
			}
			else if (c.action == "op") {
				is >> c.var1 >> c.op >> c.var2;
			}
			else if (c.action == "cmp") {
				is >> c.var1 >> c.op >> c.var2;
			}
			else if (c.action == "halve") {
				is >> c.var1;
			}
			else if (c.action == "double") {
				is >> c.var1;
			}
			else if (c.action == "inc") {
				is >> c.var1;
			}
			else if (c.action == "dec") {
				is >> c.var1;
			}
			else if (c.action == "ff") {
				is >> c.new_var;
				is >> c.small_value;
			}
			else if (c.action == "def") {
				is >> c.new_var >> c.var1 >> c.op >> c.var2;
			}
			else if (c.action == "del") {
				is >> c.var1;
			}
			else if (c.action == "even") {
				is >> c.var1;
			}
			else if (c.action == "shrink") { }
			else if (c.action == "ls") { }
			else if (c.action == "ls-dec") { }
			else if (c.action == "print" or c.action == "print-dec") {
				is >> c.var1;
			}
			else if (c.action == "help") { }
			else if (c.action == "repeat") {
				is >> c.small_value;
				if (c.sub_command != nullptr) {
					c.clear();
				}
				
				c.sub_command = new command();
				is >> (*(c.sub_command));
			}
			return is;
		}
};

