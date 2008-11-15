
from Sire.Cluster import *

uids = Cluster.UIDs()

print "The number of nodes in your cluster is %d." % len(uids)

print "The UIDs are;"

for uid in uids:
    print uid

nodes = Cluster.getNode()

print nodes

if nodes.isEmpty():
    print "Strange - there are no nodes available!"
    assert( not nodes.isEmpty() )

node = nodes.getNode()

if (node.isNull()):
    print "Strange - I couldn't get a node!"
    assert( not node.isNull() )

node.startJob( WorkTest(1, 10) )

node.wait()

result = node.result()

node.release()

