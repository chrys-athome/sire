
import sys

carfile = sys.argv[1]

lines = open(carfile, "r").readlines()

i = 0

for line in lines[4:]:
    words = line.split()

    if words[0] == "end":
        break

    elif words[0] == "PBC":
        a = float(words[1])
        b = float(words[2])
        c = float(words[3])
        alpha = float(words[4])
        beta = float(words[5])
        gamma = float(words[6])

        #      CRYST1   13.695   22.326   20.178  90.00  90.00  90.00     1
        print "CRYST1 %8.3f %8.3f %8.3f %6.2f %6.2f %6.2f    1" % (a,b,c,alpha,beta,gamma)

    else:
        atom = words[0][0:4]
        x = float(words[1])
        y = float(words[2])
        z = float(words[3])

        i += 1

              #000000000111111111122222222223333333333444444444455555
              #123456789012345678901234567890123456789012345678901234
              #HETATM    2  H02 MEO     1       0.008  -0.015   1.089
        print("HETATM %4d %4s ZEO     1    %8.3f%8.3f%8.3f" % (i, atom, x, y, z))
