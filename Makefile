CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
INC_DIR=./include
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)
EXEC=MiniShell

GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz


all: $(SRC) $(EXEC)

%.o:%.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

$(EXEC): $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(OBJ)

$(GEXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(SRC)

doc:
	doxygen $(DOC_DIR)/doxygen.conf
