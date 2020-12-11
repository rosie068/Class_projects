#written by Rosemary He
#this script filters out poornames in a directory
#it takes in 2 optional arguments: the dir to search and a -r recursive option
#if no argument is given, then . is used

#!/bin/bash

IFS=$'\n'

recursive=false
dir=""
#if more than 2 operand, exit with failure status
if [[ "$#" -ge 3 ]]; then
    echo "Too many arguments given" >/dev/stderr
    exit 1
#if 2 operands, check if first is -r, otherwise it is wrong input
elif [[ "$#" -eq 2 ]]; then
    if [ $1 == "-r" ]; then
	dir=$2
	recursive=true
    else
	echo "Please pass r first and only r" >/dev/stderr
	exit 1
    fi
#if 1 operand, if it is -r then set to current dir, otherwise it is the dir
elif [[ "$#" -eq 1 ]]; then
    if [ $1 == "-r" ]; then
	dir="."
	recursive=true
    else
	dir=$1
    fi
#if no operand, set it to current dir
else
    dir="."
fi

#check if valid dir name
if [ -d "$dir" ]; then
    #check if the dir starts with -, if so exit with failure status
    if [ ${dir:0:1} == "-" ]; then
        echo "Input starts with -" >/dev/stderr
        exit 1
    fi
else
    echo "Input is not a directory" >/dev/stderr
    exit 1
fi

#find bad files names from the directory and only this dir
read_bad () {
    list=$(find $1 -maxdepth 1)
    
    #check if success examining the directory
    if [ "$?" -ne 0 ]; then
	echo "Error when examining directory" >/dev/stderr
    fi

    #check if the names are violating guideline
    for name in $list; do
	f_name=$(basename "$name")
	
	if [[ "$name" != "." ]] && [[ "$name" != ".." ]]; then
	    if [[ "$f_name" =~ [^A-Za-z._-] ]]; then
		if [ -d "$name" ]; then
		    echo "${name}/"
		else
		    echo $name
		fi
	    elif [[ "$f_name" == .* ]] || [[ "$f_name" == -* ]]; then
		if [ -d "$name" ]; then
		    echo "${name}/"
		else
                    echo $name
		fi
	    elif [[ ${#f_name} -gt 14 ]]; then
		if [ -d "$name" ]; then
	            echo "${name}/"
		else
                    echo $name
		fi 
	    fi
	fi
    done
}

#finds all bad name, including duplicates in this dir and only this dir
all_bad () {
    bad=$(read_bad $1)
    dups=$(find $1 -maxdepth 1 | sort | uniq -iD)
    
    #check if success examining the directory
    if [ "$?" -ne 0 ]; then
        echo "Error when examining directory" >/dev/stderr
    fi

    echo "${bad[@]}"
    for tmp in $dups; do
	if [[ " ${bad[*]} " != *$tmp* ]]; then
            echo $tmp
	fi
    done
}

all_bad_r () {
    list=$(find $1)

    #check if success examining the directory 
    if [ "$?" -ne 0 ]; then
        echo "Error when examining directory" >/dev/stderr
    fi

    for files in $list; do
        if [ -d "$files" ]; then
            all_bad $files
        fi
   done
}

if [ "$recursive" = false ]; then
    all_bad $dir | sort | uniq
else
    all_bad_r $dir | sort | uniq
fi

