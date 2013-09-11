
import sys

pdb = sys.argv[1]

lines = open(pdb, "r").readlines()

for line in lines:
    words = line.split()

    try:
        #CRYST1  13.695  22.326  20.178  90.00  90.00  90.00 P1            1
        if words[0] == "CRYST1":
            a = float(words[1])
            b = float(words[2])
            c = float(words[3])
            alpha = float(words[4])
            beta = float(words[5])
            gamma = float(words[6])

            print "CRYST1 %8.3f %8.3f %8.3f %6.2f %6.2f %6.2f    1" % (a,b,c,alpha,beta,gamma)            
        else:
            print line,
    except:
        print line,
