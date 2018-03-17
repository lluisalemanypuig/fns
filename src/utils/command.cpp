#include "command.hpp"

/// PRIVATE

/// PUBLIC

command::command() {
	sub_command = nullptr;
	big_value.init_si(0);
}

command::~command() {
	clear();
}

void command::clear() {
	if (sub_command != nullptr) {
		sub_command->clear();
		delete sub_command;
		sub_command = nullptr;
	}
}


