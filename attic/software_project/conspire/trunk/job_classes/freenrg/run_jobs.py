
from Queue import *
from threading import Thread,Lock

import os
import sys

# the first argument is the number of threads to use
try:
    nthreads = int(sys.argv[1])
except:
    nthreads = 1

# now import all of the parameters to run the job
from conspire_defined_parameters import *

# this has imported the lambda values to run, in lambda_values
# and also the number of blocks of simulation to run in nblocks

# this function runs each job
def runJob():
    while not queue.empty():
        try:
            job = queue.get()
            print "Running \"%s\"..." % job
            os.system(job)
            print "Job \"%s\" has finished" % job 
            queue.task_done()
        except:
            print "Job \"%s\" failed!" % job

# now loop over the iterations
for i in range(1,nblocks+1):
    print "Running block %d of %d..." % (i, nblocks)

    # now put the job for each lamdba value onto the queue
    queue = Queue()

    for j in range(1, len(lambda_values+1)):
        queue.put( "python run.py %d" % j )

    # start all of the threads and run each job
    for i in range(0,nthreads):
        thread = Thread(target=runJob)
        thread.setDaemon(True)
        thread.start()

    # now wait until all of the jobs have been processed
    queue.join()

print "Job finished"
