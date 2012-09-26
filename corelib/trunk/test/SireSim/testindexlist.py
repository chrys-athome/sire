from Sire.Sim import *

def checkRange(r):
    print "Checking range %s" % r
    for i in range(10,1001,10):
        s1 = len(r.indicies(i))
        s2 = r.count(i)

        if s1 != s2:
            print "%d FAILED %d vs. %d" % (i,s1,s2)

r = Range("all")
checkRange(r)

r = Range("none")
checkRange(r)

r = Range("1")
checkRange(r)

r = Range("1..10")
checkRange(r)

r = Range("10..(100)..")
checkRange(r)

r = IndexList("all")
checkRange(r)

r = IndexList("none")
checkRange(r)

r = IndexList("all, none")
checkRange(r)

r = IndexList("2..(2).., 4..(4)..", False)
checkRange(r)

r = IndexList("all", False)
checkRange(r)

r = IndexList("[ 5, 10, 20..(10).., 20..(2)..50, all ]", False)
checkRange(r)
