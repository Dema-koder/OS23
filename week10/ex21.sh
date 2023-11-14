#!bin/bas
ls -l /etc | grep ^d | wc -l
file ex1.c
gcc ex1.c -o ex1
file ex1.c
./ex1
file ex1.c 
echo "The file ex1.c with the result "Hello, world!" and the file ex1.c with the output "Пpивет, мир!" differ in that the latter contains Unicode text while the former has ASCII text characters. The first file is identified as ASCII because it only contains English characters, and the second file is identified as Unicode because it contains Russian characters. Unicode is an international character encoding standard that assigns a unique number to each character in various scripts and languages, making nearly all symbols accessible on a variety of platforms, applications, and devices."
