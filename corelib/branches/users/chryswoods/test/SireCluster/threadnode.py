
from Sire.Cluster import *

for i in range(0,5):
    Cluster.addThread()

print "Getting local node..."
node = Cluster.getLocalNode()
print node

print "Submitting job..."
promises = node.submit( [ WorkTest(10,0), WorkTest(10,0), WorkTest(5,0),
                          WorkTest(1,11), WorkTest(1,6), WorkTest(1,11) ] )

print "Waiting for job..."
promises.wait()

print "Job complete!"

for i in range(0,promises.count()):
    print promises[i].result()
    print promises[i].result().isError()

print "End of script"
