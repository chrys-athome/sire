
from Sire.Cluster import *

uids = Cluster.UIDs()

print "The number of nodes in your cluster is %d." % len(uids)

print "The UIDs are;"

for uid in uids:
    print uid

print "Reserving a node from the cluster...",
nodes = Cluster.getNode()
print " ...done!"

print nodes

if nodes.isEmpty():
    print "Strange - there are no nodes available - adding this thread!"
    this_thread = nodes.borrowThisThread()

node = nodes.getNode()

if (node.isNull()):
    print "Strange - I couldn't get a node!"
    assert( not node.isNull() )

if not node.isLocal():
    print "I'm running on a non-local node!"

promise = node.startJob( WorkTest(0, 10) )

promise.wait()

result = promise.result()

promise = node.startJob( WorkTest(0,3) )

result = promise.result()

#get up to 10 nodes
nodes2 = Cluster.getNodes(10)

print nodes2

if nodes2.isEmpty():
    this_thread = 0
    this_thread = nodes2.borrowThisThread()

promises = []

print "Starting lots of jobs..."
for i in range(0, nodes2.count()):
    node = nodes2.getNode()
    promises.append( node.startJob(WorkTest(0,10)) )

print "Waiting for them to finish..."
for promise in promises:
    result = promise.result()

print "All done!"
