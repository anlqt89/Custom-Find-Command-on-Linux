#!/bin/sh
rm -r testdir/dir1/twomins
rm testdir/dir1/dir2/twomins.txt
mkdir testdir/dir1/twomins
touch testdir/dir1/dir2/twomins.txt
echo ".................................."
echo "BEOFRE DELETE"
echo "****************"
./opt.exe -w  testdir
echo "command: findf testdir/dir1 -m -2 -d"
./opt.exe testdir/dir1 -m -2 -d
echo "AFTER DELETE"
./opt.exe testdir
echo " "

