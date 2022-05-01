#!/bin/sh
mkdir testdir
cd testdir
touch test1
echo "This is test 1" >> test1
touch test2
echo "This is test 2" >>test2
mkdir dir1
mkdir dir3
cd dir1
touch test3
echo "This is test 3" >> test3
touch test4
echo "This is test 4" >> test4
mkdir dir2
cd dir2
touch test5
echo "This is test 5" >> test5
touch test6
echo "This is test 6" >> test6
cd ..
cd ..
cd dir3
touch test7
echo "This is test7" >>test7
touch test8
echo "This is test8">>test8
mkdir dir4
cd ..
mkdir dir5
cd dir5
mkdir dir6

