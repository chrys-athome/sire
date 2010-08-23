
from Sire.Base import *
from Sire.Qt import *

p = Properties()

p.insert("author", VariantProperty(QVariant("Christopher")))

print p["author"].what()
