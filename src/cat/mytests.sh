#!/bin/bash

echo "\n\033[45m=======TEST 1=======\033[0m\n"
./s21_cat test_files/test_1_cat.txt --number > s21_cat.log

cat test_files/test_1_cat.txt --number > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 1 is passed, files s21_cat.log and s_cat.log with flag --number are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 1 is falied :(\033[0m"
fi

echo "\n\033[45m=======TEST 2=======\033[0m\n"
./s21_cat test_files/test_1_cat.txt --squeeze-blank > s21_cat.log

cat test_files/test_1_cat.txt --squeeze-blank > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 2 is passed, files s21_cat.log and s_cat.log with flag --squeeze-blank are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 2 is falied :(\033[0m"
fi

echo "\n\033[45m=======TEST 3=======\033[0m\n"
./s21_cat test_files/test_4_cat.txt --number-nonblank > s21_cat.log

cat test_files/test_4_cat.txt --number-nonblank > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 3 is passed, files s21_cat.log and s_cat.log with flag --number-nonblank are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 3 is falied :(\033[0m"
fi

echo "\n\033[45m=======TEST 4 =======\033[0m\n"
./s21_cat -vT test_files/test_4_cat.txt > s21_cat.log

cat -t test_files/test_4_cat.txt > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 4 is passed, files s21_cat.log and s_cat.log with flags -vT -t are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 4 is falied :(\033[0m"
fi

echo "\n\033[45m=======TEST 5 =======\033[0m\n"
./s21_cat -E test_files/test_4_cat.txt > s21_cat.log

cat -E test_files/test_4_cat.txt > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 5 is passed, files s21_cat.log and s_cat.log with flag -E are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 5 is falied :(\033[0m"
fi

echo "\n\033[45m=======TEST 6 =======\033[0m\n"
./s21_cat -T test_files/test_1_cat.txt > s21_cat.log

cat -T test_files/test_1_cat.txt > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 6 is passed, files s21_cat.log and s_cat.log with flag -T are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 6 is falied :(\033[0m"
fi

echo "\n\033[45m=======TEST 7 =======\033[0m\n"
./s21_cat -benstuv test_files/test_1_cat.txt > s21_cat.log

cat -benstuv test_files/test_1_cat.txt > s_cat.log

if diff -q s21_cat.log s_cat.log >/dev/null;
then
    echo "\033[1;32mTest 7 is passed, files s21_cat.log and s_cat.log with flags -benstuv are identical!\033[0m"
else
    echo "\033[1;31mOoops! Test 7 is falied :(\033[0m"
fi