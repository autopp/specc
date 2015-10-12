#!/bin/bash

if [[ -z $(which valgrind 2> /dev/null) ]]; then
  echo "missing valgrind"
  exit
fi

files="success before_after before_after_nested multiple_before_after"

echo "Run examples with valgrind"

for file in $files; do
  filename="example/${file}.c"
  echo -n "${filename}: "

  gcc -I./ -L./ $filename -lspecc -lm --std=gnu99 -o ${filename}.out 2>&1 && env LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH} valgrind -q --leak-check=full --log-fd=3 ${filename}.out 3>&2 > /dev/null 2>&1

  if [[ $? -eq 0 ]]; then
    echo OK
  else
    echo NG
  fi

  rm -f ${filename}.out
done
