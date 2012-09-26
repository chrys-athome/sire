
from Sire.Base import *
from Sire.Qt import *

p = Properties()

p.setProperty("author", VariantProperty(QVariant("Christopher")))

print p["author"].toString()
