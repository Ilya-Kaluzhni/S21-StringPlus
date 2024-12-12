FLAGS = -Wall -Werror -Wextra -std=c11
ifeq ($(shell uname), Linux)
	CHFLAGS = -lcheck -lsubunit -lrt -lm
endif
ifeq ($(shell uname), Darwin)
	CHFLAGS = -lcheck -lm
endif
CCOV = -fprofile-arcs -ftest-coverage
GCOVF = --coverage
LIB_O=$(patsubst %.c, %.o, $(wildcard lib/s21_*.c))
TESTS=$(patsubst tests/%, %, $(patsubst %.check, %, $(wildcard tests/s21_*.check)))

all: s21_string.a

lib/%.o: lib/%.c
	gcc $(FLAGS) -c $< -o $@

s21_string.a: $(LIB_O)
	ar rcs s21_string.a $(LIB_O)

test: s21_string.a check_files $(TESTS)
	make start_test

$(TESTS): %: tests/%.c
	gcc $(FLAGS) $< $(CHFLAGS) -L. s21_string.a -o $@

start_test:
	for file in $(TESTS); do \
        ./$$file; \
    done

check_files:
	for file in $(TESTS); do \
        checkmk clean_mode=1 tests/$$file.check > tests/$$file.c; \
    done

gcov_report: coverage_flag test
	lcov -o report.info -c -d .
	genhtml -o result report.info
	open result/index.html
	make open_html

coverage_flag:
	$(eval FLAGS += --coverage)

open_html:
	open result/index.html

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf s21_test*
	rm -rf *.gcov
	rm -rf *.gcno
	rm -rf lib/*.o
	rm -rf lib/*.gcda
	rm -rf lib/*.gcno
	rm -rf tests/*.c
	rm -rf result/
	rm -rf *.info

rebuild:
	make clean
	make
