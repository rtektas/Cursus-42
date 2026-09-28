#!/bin/bash

resfile=results.txt
perfile=perms5.txt

> "$resfile"

cnt=0

while read -r line; do
	# Create a temporary block file to hold the result block
	tempfile=$(mktemp)

	echo "Input: $line" > "$tempfile"
	./a.out $line >> "$tempfile"
	# echo "1 2 3 4 5" >> "$tempfile"  # Optional: comment this if a.out already prints the final sorted line
	#echo >> "$tempfile"  # Ensures a final newline is there
	# Count total lines so far in this block
	total=$(wc -l < "$tempfile")

	# Append total and block separator
	echo "---" >> "$tempfile"
	echo "total $cnt : $((total - 2))" >> "$tempfile"
	echo "---" >> "$tempfile"
	((cnt++))

	# Append the block to the final results file
	cat "$tempfile" >> "$resfile"
	rm "$tempfile"
done < "$perfile"

#grep '^total' results.txt | awk '$4 == 9'

##!/bin/bash
#
#resfile=results.txt
#perfile=permutations.txt
##perms3.txt
#
#> $resfile
#
#while read line; do
#	echo "Input: $line" >> $resfile
#	./a.out $line >> $resfile
#	echo "---" >> $resfile
#done < $perfile

##!/bin/bash
#
## Create or empty the output file
#> results.txt
#
## Generate all permutations and loop through them
#for perm in $(echo $(seq 1 4) | tr ' ' '\n' | \
#              awk '{a[NR]=$0} END{for(i=1;i<=NR;i++)for(j=1;j<=NR;j++)if(j!=i)for(k=1;k<=NR;k++)if(k!=i&&k!=j)for(l=1;l<=NR;l++)if(l!=i&&l!=j&&l!=k)print a[i],a[j],a[k],a[l]}'); do
#    echo "Input: $perm" >> results.txt
#    ./push_swap $perm >> results.txt
#    echo "---" >> results.txt
#done
#
#Generate all 4! = 24 permutations of 1–4.
#
#Run ./push_swap on each one.
#
#Append the output (and the input used) to results.txt.
