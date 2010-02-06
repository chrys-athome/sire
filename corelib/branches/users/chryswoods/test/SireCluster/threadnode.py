
from Sire.Cluster import *

for i in range(0,50):
    Cluster.addThread()

print "Getting local node..."
nodes = Cluster.getLocalNodes(100)
print nodes

promises = []

for i in range(0,25):
    promises.append( WorkTest(1,11) )

for i in range(0,25):
    promises.append( WorkTest(10,0) )

print "Submitting job..."
promises = nodes.submit(promises)

print "Waiting for job..."
promises.wait()

print "Job complete!"

for i in range(0,promises.count()):
    print promises[i].result()
    print promises[i].result().isError()

print "End of script"
