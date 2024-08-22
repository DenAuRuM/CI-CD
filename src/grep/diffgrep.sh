
echo ====== GREP 1 ======
#leaks -atExit -- ./s21_grep -e count s21_grep.c s21_grep.h >> test_leak.log
diff <(grep -e count s21_grep.c s21_grep.c ) <(./s21_grep -e count s21_grep.c  s21_grep.c )
echo ====== GREP 2 ======
diff <(grep -i test test_files/text.txt test_files/text4.txt) <(./s21_grep  -i test test_files/text.txt test_files/text4.txt)
#leaks -atExit -- ./s21_grep  -i test test_files/text.txt test_files/text4.txt >> test_leak.log
echo ====== GREP 3 ======
diff <(grep -n test test_files/text.txt test_files/text4.txt) <(./s21_grep  -n test test_files/text.txt test_files/text4.txt)
#leaks -atExit -- ./s21_grep  -n test test_files/text.txt test_files/text4.txt >> test_leak.log
echo ====== GREP 4 ======
diff <(grep -v test test_files/text.txt test_files/text4.txt) <(./s21_grep  -v test test_files/text.txt test_files/text4.txt)
#leaks -atExit -- ./s21_grep  -n test test_files/text.txt test_files/text4.txt >> test_leak.log
echo ====== GREP 5 ======
#leaks -atExit -- ./s21_grep  -c test test_files/text.txt test_files/text4.txt >> test_leak.log
diff <(grep -c test test_files/text.txt test_files/text4.txt) <(./s21_grep  -c test test_files/text.txt test_files/text4.txt)
echo ====== GREP 6 ======
#leaks -atExit -- ./s21_grep test test_files/text.txt test_files/text4.txt >> test_leak.log
diff <(grep test test_files/text.txt test_files/text4.txt) <(./s21_grep test test_files/text.txt test_files/text4.txt)
echo ====== GREP 7 ======
diff <(grep -i -e test test_files/text.txt test_files/text4.txt) <(./s21_grep -i -e test test_files/text.txt test_files/text4.txt)
