
from Sire.Cluster import *

Cluster.addThread()

print "Getting local node..."
node = Cluster.getLocalNode()
print node

print "Submitting job..."
promise = node.submit( WorkTest(10,7) )

print "Waiting for job..."
while not promise.wait(10000):
   print "Still waiting..."

print "Job complete!"

print promise.result()
print promise.result().isError()
promise.result().throwError()

