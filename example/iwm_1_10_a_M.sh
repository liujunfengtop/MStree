#!/bin/sh


rm tree -f
rm out_M -f
./ms 3 10000 -t 0.01 -T -I 3 1 1 1 -m 1 2 4 -m 2 1 4 -em 1 1 2 0 -em 1 2 1 0 -ej 1 2 1 -ej 2 3 1 | tail -n +4 | grep -v // > tree

