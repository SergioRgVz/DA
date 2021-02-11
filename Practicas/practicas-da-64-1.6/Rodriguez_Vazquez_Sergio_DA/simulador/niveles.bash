#!/bin/bash
niveles=(1224 2225 2054 2056 2556 2556 2452 2429 3256 3292)
for j in "${niveles[@]}"
do
    ../simulador/simulador -level $j
done