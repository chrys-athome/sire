
from Sire.Squire import *
from Sire.Maths import *
from Sire.Units import *

S = S_GTO(0.5, 1)
P = P_GTO(0.5, 1)
C = PointCharge( Vector(0,0,0), 1*mod_electron )
D = PointCharge( Vector(0,0,1.4), 1*mod_electron )

x = Vector(0,0,0)
y = Vector(0.0,0.0,1.4)

def printInt(sxsx, sxpx, pxsx, pxpx,
             sysy, sypy, pysy, pypy,
             sxsy, sxpy, pxsy, pxpy,
             sysx, sypx, pysx, pypx):

    print "%f %f %f %f %f %f %f %f" % (sxsx, sxpx[0], sxpx[1], sxpx[2],
                                       sxsy, sxpy[0], sxpy[1], sxpy[2])
    print "%f %f %f %f %f %f %f %f" % (pxsx[0], pxpx(0,0), pxpx(0,1), pxpx(0,2),
                                       pxsy[0], pxpy(0,0), pxpy(0,1), pxpy(0,2))
    print "%f %f %f %f %f %f %f %f" % (pxsx[1], pxpx(1,0), pxpx(1,1), pxpx(1,2),
                                       pxsy[1], pxpy(1,0), pxpy(1,1), pxpy(1,2))
    print "%f %f %f %f %f %f %f %f" % (pxsx[2], pxpx(2,0), pxpx(2,1), pxpx(2,2),
                                       pxsy[2], pxpy(2,0), pxpy(2,1), pxpy(2,2))
    print "%f %f %f %f %f %f %f %f" % (sysx, sypx[0], sypx[1], sypx[2],
                                       sysy, sypy[0], sypy[1], sypy[2])
    print "%f %f %f %f %f %f %f %f" % (pysx[0], pypx(0,0), pypx(0,1), pypx(0,2),
                                       pysy[0], pypy(0,0), pypy(0,1), pypy(0,2))
    print "%f %f %f %f %f %f %f %f" % (pysx[1], pypx(1,0), pypx(1,1), pypx(1,2),
                                       pysy[1], pypy(1,0), pypy(1,1), pypy(1,2))
    print "%f %f %f %f %f %f %f %f" % (pysx[2], pypx(2,0), pypx(2,1), pypx(2,2),
                                       pysy[2], pypy(2,0), pypy(2,1), pypy(2,2))


print "OVERLAP"
printInt( overlap_integral( SS_GTO(x,S,x,S) ), overlap_integral( SP_GTO(x,S,x,P) ),
          overlap_integral( SP_GTO(x,P,x,S) ), overlap_integral( PP_GTO(x,P,x,P) ),
          overlap_integral( SS_GTO(y,S,y,S) ), overlap_integral( SP_GTO(y,S,y,P) ),
          overlap_integral( SP_GTO(y,P,y,S) ), overlap_integral( PP_GTO(y,P,y,P) ),
          overlap_integral( SS_GTO(x,S,y,S) ), overlap_integral( SP_GTO(x,S,y,P) ),
          overlap_integral( SP_GTO(x,P,y,S) ), overlap_integral( PP_GTO(x,P,y,P) ),
          overlap_integral( SS_GTO(y,S,x,S) ), overlap_integral( SP_GTO(y,S,x,P) ),
          overlap_integral( SP_GTO(y,P,x,S) ), overlap_integral( PP_GTO(y,P,x,P) ) )

print "\nKINETIC"
printInt( kinetic_integral( SS_GTO(x,S,x,S) ), kinetic_integral( SP_GTO(x,S,x,P) ),
          kinetic_integral( SP_GTO(x,P,x,S) ), kinetic_integral( PP_GTO(x,P,x,P) ),
          kinetic_integral( SS_GTO(y,S,y,S) ), kinetic_integral( SP_GTO(y,S,y,P) ),
          kinetic_integral( SP_GTO(y,P,y,S) ), kinetic_integral( PP_GTO(y,P,y,P) ),
          kinetic_integral( SS_GTO(x,S,y,S) ), kinetic_integral( SP_GTO(x,S,y,P) ),
          kinetic_integral( SP_GTO(x,P,y,S) ), kinetic_integral( PP_GTO(x,P,y,P) ),
          kinetic_integral( SS_GTO(y,S,x,S) ), kinetic_integral( SP_GTO(y,S,x,P) ),
          kinetic_integral( SP_GTO(y,P,x,S) ), kinetic_integral( PP_GTO(y,P,x,P) ) )

def pot_int( GTO ):
    return potential_integral(C, GTO) + potential_integral(D, GTO)

print "\nPOTENTIAL"
printInt( pot_int( SS_GTO(x,S,x,S) ), pot_int( SP_GTO(x,S,x,P) ),
          pot_int( SP_GTO(x,P,x,S) ), pot_int( PP_GTO(x,P,x,P) ),
          pot_int( SS_GTO(y,S,y,S) ), pot_int( SP_GTO(y,S,y,P) ),
          pot_int( SP_GTO(y,P,y,S) ), pot_int( PP_GTO(y,P,y,P) ),
          pot_int( SS_GTO(x,S,y,S) ), pot_int( SP_GTO(x,S,y,P) ),
          pot_int( SP_GTO(x,P,y,S) ), pot_int( PP_GTO(x,P,y,P) ),
          pot_int( SS_GTO(y,S,x,S) ), pot_int( SP_GTO(y,S,x,P) ),
          pot_int( SP_GTO(y,P,x,S) ), pot_int( PP_GTO(y,P,x,P) ) )

