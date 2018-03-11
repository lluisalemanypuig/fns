
# VARIABLES

PROJ_DIR = ..
SRC_DIR = $(PROJ_DIR)/src
CORE_DIR = $(SRC_DIR)/core
UTILS_DIR = $(SRC_DIR)/utils
BIN_DIR = $(PROJ_DIR)/bin

OBJECTS = 	$(BIN_DIR)/factoradic.o		\
			$(BIN_DIR)/integer.o		\
			$(BIN_DIR)/gmp_utils.o		\
			$(BIN_DIR)/time.o

CALC_OBJECTS =  $(BIN_DIR)/calculator.o	\
				$(BIN_DIR)/operations.o \
				$(OBJECTS)

PROC_PERM_OBJECTS = $(BIN_DIR)/permutations.o \
					$(OBJECTS)

FLAGS = -O0 -std=c++0x -fopenmp -Wunused
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

all: $(BIN_DIR)/calculator $(BIN_DIR)/process_permutations

######################
# PROCESS PERMUTATIONS

$(BIN_DIR)/process_permutations: $(PROC_PERM_OBJECTS)
	g++ $(FLAGS) -o $(BIN_DIR)/process_permutations $(PROC_PERM_OBJECTS) $(LIBS)

$(BIN_DIR)/permutations.o: $(SRC_DIR)/permutations.cpp $(PROC_PERM_DPS)
	g++ $(FLAGS) -c $< -o $@

############
# CALCULATOR

$(BIN_DIR)/calculator: $(CALC_OBJECTS)
	g++ $(FLAGS) -o $(BIN_DIR)/calculator $(CALC_OBJECTS) $(LIBS)

$(BIN_DIR)/calculator.o: $(SRC_DIR)/calculator.cpp $(CALC_DPS)
	g++ $(FLAGS) -c $< -o $@

$(BIN_DIR)/operations.o: $(CORE_DIR)/operations.cpp $(FACTORADIC_DPS)
	g++ $(FLAGS) -c $< -o $@

$(BIN_DIR)/factoradic.o: $(CORE_DIR)/factoradic.cpp $(FACTORADIC_DPS)
	g++ $(FLAGS) -c $< -o $@

$(BIN_DIR)/integer.o: $(CORE_DIR)/integer.cpp $(INTEGER_DPS)
	g++ $(FLAGS) -c $< -o $@

$(BIN_DIR)/gmp_utils.o: $(CORE_DIR)/gmp_utils.cpp $(GMPUTILS_DPS)
	g++ $(FLAGS) -c $< -o $@

$(BIN_DIR)/time.o: $(UTILS_DIR)/time.cpp $(TIME_DPS)
	g++ $(FLAGS) -c $< -o $@

##########

clean:
	rm -f $(OBJECTS)
	rm -f $(CALC_OBJECTS)
	rm -f $(PROC_PERM_OBJECTS)

distclean:
	make -f $(SRC_DIR)/Makefile clean
	rm -f $(BIN_DIR)/factoradic
	rm -f $(BIN_DIR)/process_permutations
