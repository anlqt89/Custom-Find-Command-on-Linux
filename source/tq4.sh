#!/bin/sh
echo "..................................."
echo "command: findf testdir -n test1 -a cat"
./opt.exe testdir -n test1 -a cat
echo " "

echo ".................................."
echo "BEFORE DELETE"
./opt.exe testdir/dir1/dir2
echo "***************"
echo "command: findf testdir/dir1/dir2 -n test6 -a rm"
./opt.exe testdir/dir1/dir2 -n test6 -a rm
echo "AFTER DELETE"
./opt.exe testdir/dir1/dir2
echo " "


echo ".................................."
echo "BEFORE MOVE FILE"
./opt.exe testdir
echo "***************"
echo "command: findf testdir/dir3 -n test7 -a mv testdir/dir5/dir6/test7_moved"
./opt.exe testdir/dir3 -n test7 -a mv testdir/dir5/dir6
echo "AFTER MOVE FILE"
./opt.exe testdir
echo " "


echo ".................................."
echo "BEFORE MOVE DIRECTORY"
./opt.exe testdir
echo "***************"
echo "command: findf testdir -n dir3 -a mv testdir/dir5"
./opt.exe testdir -n dir3 -a mv testdir/dir5
echo "AFTER MOVE DIRECTORY"
./opt.exe testdir
echo " "
