
# VARIABLES

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
CORE_DIR = $(SRC_DIR)/core
UTILS_DIR = $(SRC_DIR)/utils
RULES_DIR = $(PROJ_DIR)/build-rules
DEB_BIN_DIR = $(PROJ_DIR)/bin-debug
REL_BIN_DIR = $(PROJ_DIR)/bin-release

GEN_FLAGS = -std=c++0x -fopenmp -Wunused -Wall -Wconversion

INCLUDE = $(SRC_DIR)

LIBS = -lgmp -fopenmp -lpthread

# DEPENDENCIES

TIME_DPS = $(UTILS_DIR)/time.hpp
DEFS_DPS = $(UTILS_DIR)/defs.hpp
GMPUTILS_DPS = $(CORE_DIR)/gmp_utils.hpp
INTEGER_DPS = $(CORE_DIR)/integer.hpp $(GMPUTILS_DPS)
COMMAND_DPS = $(UTILS_DIR)/command.hpp $(INTEGER_DPS)
FACTORADIC_DPS = $(CORE_DIR)/factoradic.hpp $(CORE_DIR)/factoradic_template.cpp $(INTEGER_DPS)
OPERATIONS_DPS = $(UTILS_DIR)/operations.hpp $(FACTORADIC_DPS) $(INTEGER_DPS)
MAKE_PERMS_DPS = $(UTILS_DIR)/make_perms.hpp $(UTILS_DIR)/make_perms.cpp \
				 $(FACTORADIC_DPS) $(INTEGER_DPS)

CALC_DPS = $(INTEGER_DPS) $(FACTORADIC_DPS) $(OPERATIONS_DPS) $(DEFS_DPS) \
		   $(TIME_DPS) $(COMMAND_DPS) $(MAKE_PERMS_DPS)
		   
PROC_PERM_DPS = $(FACTORADIC_DPS) $(MAKE_PERMS_DPS)

# RULES

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/calculator $(DEB_BIN_DIR)/process_permutations
release: releasedirectory $(REL_BIN_DIR)/calculator $(REL_BIN_DIR)/process_permutations

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
