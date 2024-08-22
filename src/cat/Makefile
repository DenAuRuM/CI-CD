cc = gcc -Werror -Wall -Wextra -std=c11

all: s21_cat

s21_cat:
	$(cc) s21_cat.c s21_table.c -o s21_cat

clean:
	rm -rf *.o

rebuild:
	clean all

format:
	calng-format -i -style=Google *.c *.h