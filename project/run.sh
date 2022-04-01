#/bin/bash

if [ -d output ];then
  rm -fv output/*.root
  rm -fv output/*.txt
else
  mkdir output
fi

cd ../build
wait
nice make -j4
wait
./exampleB5 exampleB5.in > >(tee out.txt) 2> >(tee err.txt)
wait
cd -
mv ../build/*.root output/
mv ../build/out.txt output/
mv ../build/err.txt output/

