# this program takes input from command line or file, shuffles the list and outputs the first n lines

#!/usr/local/bin/python3
import argparse
import random
import string
import sys

def check_range(irange, input_method):
    #check optional inputs are valid
    if (irange != None) and (len(input_method) != 0):
        raise argparse.ArgumentError("Error: -i is mutually exclusive with FILE positional argument")
        sys.exit(1)
    elif (irange) != None:
        in_range = str(irange).split("-")
        low = int(in_range[0])
        high = int(in_range[1])
        #check for valid input
        if len(in_range) != 2:
            raise argparse.ArgumentError("Please input in the format low-high")
            sys.exit(1)
        elif (low < 0) or (low > high):
            raise argparse.ArgumentError("Input range: 0 <= low <= high")
            sys.exit(1)
        else:
            ret_list = list(range(low, high+1))
    else:
        #check non-optional input is valid
        if len(input_method)>1:
            raise argparse.ArgumentError("Error: more than one non-optional argument given")
            sys.exit(1)
        elif len(input_method)==0 or input_method[0]=='-':
            lines = sys.stdin.read()
        else:
            filename = input_method[0]
            file = open(filename, 'r')
            lines = file.read()
            file.close()
        ret_list = lines.split("\n")

    #remove empty strings
    while("" in ret_list):
    	ret_list.remove("")
        
    return ret_list

def shuffle_list(old_list, headcount, repeat):
    #shuffle list
    if (headcount == None) and (repeat == False):
        randList = random.sample(old_list,len(old_list))
        for line in randList:
            print(line)
    elif (headcount == None) and (repeat == True):
        forever=True
        while(forever):
            randList = random.sample(old_list,len(old_list))
            for line in randList:
                print(line)
    elif (headcount != None) and (repeat == False):
        if headcount < 0:
            raise argparse.ArgumentError("Error: head count must be greater than 0")
            sys.exit(1)
        else:
            randList = random.sample(old_list,len(old_list))
            for i in range(headcount):
                print(randList[i])
    else:
        if headcount < 0:
            raise argparse.ArgumentError("Error: head count must be greater than 0")
            sys.exit(1)
        else:
            for i in range(headcount):
                print(random.choice(old_list))

if __name__=="__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i","--input-range",action="store",dest="range",default=None,help="shuffle lines from lo to hi, inclusive")
    parser.add_argument("-n","--head-count",type=int,action="store",dest="headcount",default=None,help="output at most count lines")
    parser.add_argument("-r","--repeat",action="store_true",dest="repeat",default=False,help="allow select with replacement. if no head-count given, will repeat indefinetly")
    parser.add_argument("input",nargs='*',help="source of input. if nothing or - is given uses stdin, else use as file name")
    args = parser.parse_args()

    old_list = check_range(args.range, args.input)
    shuffle_list(old_list, args.headcount, args.repeat)
    