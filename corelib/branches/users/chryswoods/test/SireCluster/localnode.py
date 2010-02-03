
from Sire.Cluster import *

node = Node()

promise = node.submit( WorkTest(10,5) )

promise.wait()

print promise.result()
print promise.result().isError()
promise.result().throwError()


