#!/bin/bash

echo "Run all examples"

for file in $(/bin/ls example/*.c); do
  echo "---------------------------"
  echo "${file}"
  echo "---------------------------"

  clang -I./ -L./ $file -lspecc -o ${file}.out && env LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH} ${file}.out
  rm -f ${file}.out
done
