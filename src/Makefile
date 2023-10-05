CFLAGS = -Wall -Werror -Wextra -std=c11
LFLAGS = -lcheck
CC = gcc

.PHONY: clean all rebuild

TESTS = tests.c

SOURCES = helpers.c	\
	$(wildcard ./conversion/*.c) 	\
	$(wildcard ./another_funcs/*.c) 		\
	$(wildcard ./arithmetic/*.c) 	\
	$(wildcard ./comparison/*.c) \

OBJECTS := $(SOURCES:.c=.o)
TEST_OBJECTS := $(TESTS:.c=.o)

ifeq ($(shell uname), Linux)
	LFLAGS = -lpthread -lcheck -lsubunit -lm
endif

%.o: %.c s21_decimal.h
	$(CC) $(CFLAGS) -c $< -o $@

all: libs21_decimal.a

test: libs21_decimal.a $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LFLAGS) -L. -ls21_decimal -o test

libs21_decimal.a: s21_decimal.a
	mv $^ $@

s21_decimal.a: $(OBJECTS) s21_decimal.h
	ar rcs s21_decimal.a $^

gcov_report: clean add_gcov_flags test
	./test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report

add_gcov_flags:
	$(eval CFLAGS += --coverage)
	$(eval LFLAGS += -lgcov)

clean:
	rm -rf *.o ./*/*.o test gcov_report */*.gcda */*.gcno *.gcno *.gcda coverage.info *.a a.out

rebuild: clean all