###############################################################################
#                         User-Aware Flying AP Project
#                             FAP Controller + AP
###############################################################################
#                        Comunicacoes Moveis 2017/2018
#                             FEUP | MIEEC / MIEIC
###############################################################################

CC		= gcc
CFLAGS	= -Wall #-Wextra

BIN		= bin
SRC		= src
LIB		= lib
TEST	= test

TEST_EXECUTABLE	= Test_FapController


.PHONY: all
all: $(BIN)/$(TEST_EXECUTABLE)


.PHONY: run_test
run_test: all
	./$(BIN)/$(TEST_EXECUTABLE)


$(BIN)/$(TEST_EXECUTABLE): $(TEST)/* $(SRC)/* $(LIB)/*
	$(CC) $(CFLAGS) -I$(SRC) -I$(LIB) $(TEST)/*.c $(SRC)/*.c $(LIB)/*.c -o $@


.PHONY: clean
clean:
	rm -rf $(BIN)/*
