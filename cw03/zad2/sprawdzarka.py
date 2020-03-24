import csv
import sys
import subprocess
import numpy as np
import os 
from timeit import default_timer as timer


os.chdir("/mnt/c/Users/andrz/CLionProjects/cw03/zad2/")
start = timer()
p = subprocess.run(sys.argv[1:])
end = timer()

if(p.returncode != 0):
    print("Something went wrong")
    exit(0)
else:
    print(end-start)
operations = sys.argv[2]

lines = []

with open(operations) as op:
    line = op.readline()
    while line:
        lines.append(line.split())
        line = op.readline()

matrixA = []
matrixB = []
result = []
correctResult = []

for line in lines:
    with open(line[0]) as csvfile:
        matrixA = np.array(list(csv.reader(csvfile)), dtype='int')
    with open(line[1]) as csvfile:
        matrixB = np.array(list(csv.reader(csvfile)), dtype='int')
    with open(line[2]) as csvfile:
        result = np.array(list(csv.reader(csvfile)), dtype='int')
    
    correctResult = matrixA@matrixB
    if(np.array_equal(correctResult, result)):
    	print(line)
    	print("Correct")
    else:
    	print(line)
    	print("Failed")

p = subprocess.run(["rm","-r", "wyniki"])
p = subprocess.run(["mkdir","wyniki"])
    
