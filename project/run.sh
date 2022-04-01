#/bin/bash

if [-d output];then
  rm -fv output/*.root
  rm -fv output/*.txt
else
  mkdir output
fi

cd ../build
rm -rfv *.root
wait
nice make -j4
wait
./exampleB5 exampleB5.in > >(tee out.txt) 2> >(tee err.txt)
wait
cd -
mv ../build/*.root output/
mv ../build/*.txt output/

