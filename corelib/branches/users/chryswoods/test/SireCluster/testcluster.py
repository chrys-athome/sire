
from Sire.Cluster import *

uids = Cluster.UIDs()

print "The number of nodes in your cluster is %d." % len(uids)

print "The UIDs are;"

for uid in uids:
    print uid

nodes = Cluster.getNode()

print nodes

if nodes.isEmpty():
    print "Strange - there are no nodes available - adding this thread!"
    this_thread = nodes.borrowThisThread()

node = nodes.getNode()

if (node.isNull()):
    print "Strange - I couldn't get a node!"
    assert( not node.isNull() )

promise = node.startJob( WorkTest(1, 10) )

promise.wait()

result = promise.result()
