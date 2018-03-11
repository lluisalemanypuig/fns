
# VARIABLES

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
CORE_DIR = $(SRC_DIR)/core
UTILS_DIR = $(SRC_DIR)/utils
RULES_DIR = $(PROJ_DIR)/build-rules
DEB_BIN_DIR = $(PROJ_DIR)/bin-debug
REL_BIN_DIR = $(PROJ_DIR)/bin-release

FLAGS = -std=c++0x -fopenmp -Wunused
DEB_FLAGS = -O0 $(FLAGS)
REL_FLAGS = -O2 $(FLAGS)

LIBS = -lgmp -fopenmp -lpthread

# DEPENDENCIES

TIME_DPS = $(UTILS_DIR)/time.hpp
DEFS_DPS = $(CORE_DIR)/defs.hpp
GMPUTILS_DPS = $(CORE_DIR)/gmp_utils.hpp
INTEGER_DPS = $(CORE_DIR)/integer.hpp $(GMPUTILS_DPS)
FACTORADIC_DPS = $(CORE_DIR)/factoradic.hpp $(CORE_DIR)/factoradic_template.cpp $(INTEGER_DPS)
OPERATIONS_DPS = $(CORE_DIR)/operations.hpp $(FACTORADIC_DPS) $(INTEGER_DPS)

CALC_DPS = $(INTEGER_DPS) $(FACTORADIC_DPS) $(OPERATIONS_DPS) $(DEFS_DPS) $(TIME_DPS)
PROC_PERM_DPS = $(FACTORADIC_DPS)

# RULES

.PHONY: debdirectory reldirectory
debug: debdirectory $(DEB_BIN_DIR)/calculator $(DEB_BIN_DIR)/process_permutations
release: reldirectory $(REL_BIN_DIR)/calculator $(REL_BIN_DIR)/process_permutations

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
