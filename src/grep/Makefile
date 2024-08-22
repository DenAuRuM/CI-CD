cc = gcc -Werror -Wall -Wextra -std=c11

all: s21_grep

s21_grep:
	$(cc) s21_grep.c -o s21_grep

clean:
	rm -rf *.o

rebuild:
	clean all

format:
	calng-format -i -style=Google *.c *.h
