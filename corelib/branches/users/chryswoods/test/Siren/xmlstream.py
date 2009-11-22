from Sire.Siren import *
from Sire.Qt import *

t = Tester()
c = t.getClass()
m = MD5Sum.fromText("Hello World")

s = QString()

xml = XMLStream(s, QIODevice.WriteOnly)

c.stream(xml)
t.stream(xml)
m.stream(xml)

xml = 0

print s

xml = XMLStream(s)

print xml.loadNextObject()
print xml.loadNextObject()
print xml.loadNextObject()

