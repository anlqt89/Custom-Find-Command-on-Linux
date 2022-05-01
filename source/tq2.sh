#!/bin/sh
echo "..................................."
echo "command: findf testdir -n dir1"
./opt.exe testdir -n dir1
echo " "

echo ".................................."
echo "command: findf testdir -n test2"
./opt.exe testdir -n test2
echo " "

rm testdir/oneMinFile.txt
rm -r testdir/oneMinDir
touch testdir/oneMinFile.txt
mkdir testdir/oneMinDir
echo "..................................."
echo "command findf -w testdir -m -2"
./opt.exe -w testdir -m -2
echo " "

echo "..................................."
echo "command: findf -w testdir -n oneMinDir -m -2"
./opt.exe -w testdir -n oneMinDir -m -2
echo " "

echo ".................................."
echo "command: find -i 60686666"
./opt.exe -i 60686666  



