from Sire.Siren import *
from Sire.Qt import *

t = Tester()
c = t.getClass()
m = MD5Sum.fromText("Hello World")

s = QString()

xml = XMLStream(s, QIODevice.WriteOnly)

m.stream(xml)
c.stream(xml)
t.stream(xml)

xml = 0

print s

xml = XMLStream(s)

print m
print xml.loadNextObject()

print c
print xml.loadNextObject()

print t
print xml.loadNextObject()

