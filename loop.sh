#!/bin/zsh
w=$1
h=$2
n=$3
p=$4
s=1

if [ "$5" != "" ]; then
    s=$5
fi

for ((i=${s}; i < ${n} + ${s}; i++));
  do
    ./gen.exe -w "${w}" -h "${h}" -s "${i}" -p "${p}"
  done
