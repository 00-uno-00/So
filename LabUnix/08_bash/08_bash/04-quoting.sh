#!/bin/bash
VAR=schi
VAR1=23
echo $VAR $VAR1
echo $0 $1

echo "Doppio apice"
echo "il valore è $VAR"

echo "Singolo apice inverso"
echo `ls -l | grep schi`
echo `ls -l | grep $VAR`

echo "Singolo apice"
echo '$VAR'
