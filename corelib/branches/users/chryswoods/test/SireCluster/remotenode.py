
from Sire.Cluster import *

node = Cluster.getNode()

print node

promise = node.submit( WorkTest(5,0,-1) )

while not promise.wait(500):
    print "Master node is still waiting..."

promises = []

for i in range(0,2):
    promises.append( WorkTest(1,11) )

promises = node.submit(promises)

promises.wait()


