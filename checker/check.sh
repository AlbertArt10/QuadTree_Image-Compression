#!/bin/bash
#//////////////////////////////////////////////////////
#	Created by Nan Mihai on 22.04.2023
#	Copyright (c) 2023 Nan Mihai. All rights reserved.
#	Checker Tema 2 - Structuri de date și algoritmi
#	Arbori cuaternari de compresie
#	Facultatea de Automatica si Calculatoare
#	Anul Universitar 2022-2023, Seria CD
#/////////////////////////////////////////////////////
rm tests/output/*
result1=0
result2=0
result3=0
result=0
bonus=0
depunctari=0
ZERO=0
VALGRIND="valgrind --leak-check=full --track-origins=yes -q --log-file=rezultat_valgrind.txt"
make build > /dev/null 2>&1
var=10
BEST=80
FACTORS=(0 1000 0 3000 100 700 0 10 100 1000 50 300 0 2000 2500 5000 500 3000 0 2000)
TESTS=(0 0 1 1 2 2 3 3 4 4 5 5 6 6 7 7 8 8 9 9)

#Cerința 1 - Determinarea statisticilor
echo "Cerința 1 - Compresia fisierelor"
for i in {0..19}
do
	fileIn="tests/input/test"${TESTS[$i]}".ppm"
	fileOut="tests/output/test.out"
	refOut="tests/ref/test"$i"_c1.txt"
	./quadtree -c1 ${FACTORS[$i]} $fileIn $fileOut > /dev/null 2>&1
	diff $fileOut $refOut > /dev/null
	EXIT_CODE=$?
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		if (( i < 10 )); then
			echo "Statistici_Test  $i ................................................. PASS"
		else
			echo "Statistici_Test $i ................................................. PASS"
		fi
		result=$(awk "BEGIN {print $result+1; exit}")
		result1=$(awk "BEGIN {print $result1+1; exit}")
		if (( i < 10)); then
			$VALGRIND ./quadtree -c1 ${FACTORS[$i]} $fileIn $fileOut > /dev/null 2>&1
			if [[ -z $(cat rezultat_valgrind.txt) ]]; then
				printf "VALGRIND ........................................................... PASS\n"
				bonus=$(echo "$bonus + 0.5" | bc -l)
			else
				printf "VALGRIND ........................................................... FAIL\n"				
			fi
		fi
	else
		if (( i < 10 )); then
			echo "Statistici_Test  $i ................................................. FAIL"
		else
			echo "Statistici_Test $i ................................................. FAIL"
		fi
	fi
done
#Cerința 2 - Compresia fișierelor
echo "Cerința 2 - Compresia fișierelor"
for i in {0..19}
do
	fileIn="tests/input/test"${TESTS[$i]}".ppm"
	fileOut="tests/output/test.out"
	refOut="tests/ref/test"$i"_c2.out"
	./quadtree -c2 ${FACTORS[$i]} $fileIn $fileOut > /dev/null 2>&1
	diff $fileOut $refOut > /dev/null
	EXIT_CODE=$?
	if [ $EXIT_CODE -eq $ZERO ]
	then
		if (( i < 10 )); then
			echo "Compresia_Test  $i .................................................. PASS"
		else
			echo "Compresia_Test $i .................................................. PASS"
		fi
		result=$(echo "$result + 1.5" | bc -l)
		result2=$(echo "$result2 + 1.5" | bc -l)
		if (( i < 10 )); then
			$VALGRIND ./quadtree -c2 ${FACTORS[$i]} $fileIn $fileOut > /dev/null 2>&1
			if [[ -z $(cat rezultat_valgrind.txt) ]]; then
				printf "VALGRIND ........................................................... PASS\n"
				bonus=$(echo "$bonus + 0.5" | bc -l)
			else
				printf "VALGRIND ........................................................... FAIL\n"				
			fi
		fi
	else
		if (( i < 10 )); then
			echo "Compresia_Test  $i .................................................. FAIL"
		else
			echo "Compresia_Test $i .................................................. FAIL"
		fi
	fi
done

#Cerința 3 - Decompresia fișierelor
echo "Cerința 3 - Decompresia fișierelor"
for i in {0..19}
do
	fileIn="tests/input/test"$i"_c2.out"
	fileOut="tests/output/test.ppm"
	refOut="tests/ref/test"$i"_c3.ppm"
	./quadtree -d $fileIn $fileOut > /dev/null 2>&1
	diff $fileOut $refOut > /dev/null
	EXIT_CODE=$?
	if [ $EXIT_CODE -eq $ZERO ]
	then
		if (( i < 10 )); then
			echo "Decompresia_Test  $i ................................................ PASS"
		else
			echo "Decompresia_Test $i ................................................ PASS"
		fi
		result=$(echo "$result + 1.5" | bc -l)
		result3=$(echo "$result3 + 1.5" | bc -l)
		if (( i < 10 )); then
			$VALGRIND ./quadtree -d $fileIn $fileOut > /dev/null 2>&1
			if [[ -z $(cat rezultat_valgrind.txt) ]]; then
				printf "VALGRIND ........................................................... PASS\n"
				bonus=$(echo "$bonus + 1.0" | bc -l)
			else
				printf "VALGRIND ........................................................... FAIL\n"				
			fi
		fi
	else
		if (( i < 10 )); then
			echo "Decompresia_Test  $i ................................................ FAIL"
		else
			echo "Decompresia_Test $i ................................................ FAIL"
		fi
	fi
done
echo "Cerinta 1 : "$result1
echo "Cerinta 2 : "$result2
echo "Cerinta 3 : "$result3
echo "Total     : "$result
echo "Bonus     : "$bonus
total=${result%.*}
if (( total == BEST )); then
	echo "Felicitari! Ai punctajul maxim: "$BEST"p! :)"
else
	echo "Ai acumulat "$result"p din maxim 80p! :("
fi
make clean > /dev/null 2>&1
