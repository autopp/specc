#!/bin/bash

echo "Run all examples"

for file in $(/bin/ls example/*.c); do
  echo "---------------------------"
  echo "${file}"
  echo "---------------------------"

  gcc -I./ -L./ $file -lspecc -lm --std=gnu99 -o ${file}.out 2>&1 && env LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH} ${file}.out 2>&1
  rm -f ${file}.out
done
