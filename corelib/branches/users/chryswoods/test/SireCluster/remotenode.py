
from Sire.Cluster import *

node = Cluster.getNode()

print node

promise = node.submit( WorkTest(2,0,-1) )

while not promise.wait(500):
    print "Master node is still waiting..."
    print node

promises = []

for i in range(0,3):
    promises.append( WorkTest(1,3) )

promises = node.submit(promises)

while not promises.wait(500):
    print "Master node is still waiting..."
    print node


