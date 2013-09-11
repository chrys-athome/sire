#!/usr/bin/python2

import socket
import thread
import os, re
import subprocess
import time
import sys

# autoflush output
sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)

print "Some text"
print "Some more text"
print "Even more text"

# SUBMISSION NODE
# PARENT_NODE=127.0.0.1 PARENT_NODE_PORT=10020 ISSUBMIT=TRUE WORKNAME=sleepwork CLASSNAME=pmemd FILENAME=/home/pgrad/long/linux/realtest.tar.gz ./leafhead3.py
# INTERMEDIATE/HEAD NODE
# PARENT_NODE=127.0.0.1 PARENT_NODE_PORT=10020 TIMESLOT_ID=1 MYPORT=10009 ./leafhead3.py
# INTERMEDIATE/HEAD NODE
# PARENT_NODE=gizmo PARENT_NODE_PORT=10009 TIMESLOT_ID=1 MYPORT=10010 ./leafhead3.py
# LEAF NODE
# ISLEAF=1 PARENT_NODE=gizmo PARENT_NODE_PORT=10010 TIMESLOT_ID=1 ./leafhead3.py
# QUERY NODE
# PARENT_NODE=127.0.0.1 PARENT_NODE_PORT=10020 ISQUERY=TRUE WKPTID=0 ./leafhead3.py
# DOWNLOAD COMPLETED FROM BROKER
# PARENT_NODE=127.0.0.1 PARENT_NODE_PORT=10020 ISDOWNLOAD=TRUE WKPTID=0 OUTFILENAME=/home/pgrad/long/linux/outwork.tar.gz ./leafhead3.py

def splithostchain(hostchain):
   l = hostchain.split('/', 1) 
   o = l[0].split(':')
   if (len(l) > 1):
      return o[0], int(o[1]), l[1]
   else:
      return o[0], int(o[1]), ''

def buildandsubmitscript(timeslot_number, duration, caps):
   print('RESERVATION ACCEPTED! - ' + str(timeslot_number) + ' ' + str(duration) + ' ' + caps)
   envstrings = 'PARENT_NODE=ssi-amrmmhd.epcc.ed.ac.uk PARENT_NODE_PORT=10000 ISLEAF=1 TIMESLOT_ID=' + str(timeslot_number)
   runstring = 'module load torque; echo "( cd /tmp; wget http://www.cs.bris.ac.uk/~long/res/leafhead3.py; chmod +x /tmp/leafhead3.py;' + envstrings + ' /usr/bin/python2 ./leafhead3.py; rm ./leafhead3.py )" | qsub -l walltime=00:20:00 '
# pipe to qsub | qsub
   subprocess.call('echo \'' + runstring + '\' | ssh long@icy.cs.bris.ac.uk \'ssh bl2335@bluecrystalp2.bris.ac.uk --\'', shell=True)

def senddownload(addr, portno, wkptid):
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   print("DOWNLOAD " + str(wkptid))
   sock.connect((addr, portno))
   sock.send('DOWNLOAD\n')
   sock.send('WorkPacketID: ' + str(wkptid) + '\n')
   sock.send('\n')
   return sock

def sendquery(addr, portno, wkptid):
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   print("QUERY " + str(wkptid))
   sock.connect((addr, portno))
   sock.send('QUERY\n')
   sock.send('WorkPacketID: ' + str(wkptid) + '\n')
   sock.send('\n')
   return sock

def sendreservation(addr, portno, tsid, hostname, dur, caps):
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   print("RESERVATION " + str(tsid) + " " + socket.gethostname())
   sock.connect((addr, portno))
   sock.send('RESERVATION\n')
   sock.send('HostChain: ' + hostname + '\n')
   sock.send('TimeSlot: ' + str(tsid) + '\n')
   sock.send('Duration: ' + str(dur) + '\n')
   sock.send('RqrdCapabilities: ' +  caps + '\n')
   sock.send('\n')
   return sock

#def sendquery(addr, portno, workname, tclass, gzname):
#   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#   print("QUERY " + workname + " " + tclass + " " + gzname)
#   sock.connect((addr, portno))
#   sock.send('QUERY\n')
#   sock.send('WorkName: ' + workname + '\n')
#   sock.send('Class: ' + tclass + '\n')
#   sock.send('JobCapabilities: single\n')
#   sock.send('FileName: ' + gzname + '\n')
#   sock.send('\n')
#   return sock

def sendsubmit(addr, portno, workname, tclass, gzname):
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   print("SUBMISSION " + workname + " " + tclass + " " + gzname)
   sock.connect((addr, portno))
   sock.send('SUBMISSION\n')
   sock.send('WorkName: ' + workname + '\n')
   sock.send('Class: ' + tclass + '\n')
   sock.send('JobCapabilities: single\n')
   sock.send('FileName: ' + gzname + '\n')
   sock.send('ContentLength: ' + str(os.stat(gzname).st_size) + '\n')
   sock.send('\n')
   return sock

def sendfree(addr, portno, tsid, hostname, cpus, gpus, retries):
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   print("FREE " + str(tsid) + " " + socket.gethostname())
   sock.connect((addr, portno))
   sock.send('FREE\n')
   sock.send('HostChain: ' + hostname + '\n')
   sock.send('TimeSlot: ' + str(tsid) + '\n')
   sock.send('Try: ' + str(retries) + '\n')
   sock.send('CPUs: ' + str(cpus) + '\n')
   sock.send('GPUs: ' + str(gpus) + '\n')
   sock.send('\n')
   return sock

def sendresults(addr, portno, workname, tsid, wkptid, hostname, gzname, datablock, filemaniptime, workingtime):
   sock = sendresultsheader(addr, portno, workname, tsid, wkptid, hostname, gzname, datablock, filemaniptime, workingtime, os.stat(gzname).st_size)
   uploadstream(datablock, '', sock, gzname, addr)
   return sock

def sendresultsheader(addr, portno, workname, tsid, wkptid, hostname, gzname, datablock, filemaniptime, workingtime, gzsize):
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   print("RESULTS " + str(tsid) + " " + socket.gethostname())
   sock.connect((addr, portno))
   sock.send('RESULTS\n')
   sock.send('HostChain: ' + hostname + '\n')
   sock.send('TimeSlot: ' + str(tsid) + '\n')
   sock.send('WorkName: ' + workname + '\n')
   sock.send('WorkPacketID: ' + str(wkptid) + '\n')
   sock.send('OverheadTime: ' + str(filemaniptime) + '\n')
   sock.send('WorkingTime: ' + str(workingtime) + '\n')
   sock.send('FileName: ' + gzname + '\n')
   sock.send('ContentLength: ' + str(gzsize) + '\n')
   sock.send('\n')
   return sock

def uploadstream(datablock, datain, sock, filename, addr):
   tarfile = open(filename, 'rb')
   totlen = 0
   dataamt = 0
   while 1:
      data = tarfile.read(datablock)
      if (len(data) == 0): break
      try:
         dataamt = sock.sendall(data)
      except:
         print('An error occurred sending data to ', addr)
         # Needs better handling!
         break
      if (((totlen + len(data)) / 1048576) > (totlen / 1048576)): print(str(totlen))
      totlen = totlen + len(data)
   tarfile.close()
   return sock

def downloadstream(datablock, datain, sock, filename, addr):
   dlen = 0
   vfile = open(filename, 'wb')
   data = datain
   try:
      vfile.write(data)
      dlen = dlen + len(data)
      print(str(dlen)+'\r')
   except:
      print('Failed to open file for writing')
      pass
   while 1:
      try:
         data = sock.recv(datablock)
      except:
         print('Failed to receive download bulk')
         break
      if (len(data) == 0): break
      try:
         vfile.write(data)
         if (((dlen + len(data)) / 1048576) > (dlen / 1048576)): print(str(dlen)+'\r')
         dlen = dlen + len(data)
      except:
         printf('Fail to write second')
         break
   vfile.close()
   return dlen

def streambetween(insock, outsock, datablock):
   tdlen = 0
   while 1:
      try:
         data = insock.recv(datablock)
      except:
         break
      if (len(data) == 0): break
      try:
         tdlen = tdlen + outsock.send(data)
      except:
         break
   return tdlen

def doworkpacket(tarfilename, timeout, tslot_id):
   # MAYBE ADD WORKPACKET ID TO NAMING CONVENTIONS?
   print("DOING WORK!")
   time1 = time.time()
   os.spawnvp(os.P_WAIT, 'mkdir', ['mkdir', '-p', 'tslot_' + str(tslot_id)])
   os.spawnvp(os.P_WAIT, 'tar', ['tar', '-xzf', tarfilename, '-C', 'tslot_' + str(tslot_id)])
   # Should check the return value of tar and make this error resilient - recover from broken files - need to send message length too!
   os.spawnvp(os.P_WAIT, 'rm', ['rm', '-f', tarfilename])
   os.spawnvp(os.P_WAIT, 'chmod', ['chmod', '+x', 'tslot_' + str(tslot_id) + '/go.sh'])
   # timeout is GNU coreutils - this is the command that runs everything!
   time2 = time.time()
   if (timeout > 0.):
# Timeout is currently BROKEN!
#      os.spawnvp(os.P_WAIT, 'timeout', ['timeout', '-k', str(timeout), str(timeout), 'bash', '-c', '(cd tslot_' + str(tslot_id) + ' && ./go.sh)'])
      os.spawnvp(os.P_WAIT, 'bash', ['bash', '-c', '(cd tslot_' + str(tslot_id) + ' && ./go.sh)'])
   else:
      os.spawnvp(os.P_WAIT, 'bash', ['bash', '-c', '(cd tslot_' + str(tslot_id) + ' && ./go.sh)'])
   time3 = time.time()
   os.spawnvp(os.P_WAIT, 'tar', ['tar', '-C', 'tslot_' + str(tslot_id), '-czf', 'out' + tarfilename, '.'])
   os.spawnvp(os.P_WAIT, 'rm', ['rm', '-fr', 'tslot_' + str(tslot_id)])
   time4 = time.time()
   print("SHOULD REQUEST SOME MORE WORK!")
   return (time4 - time1) - (time3 - time2), (time3 - time2)
   #sendfree(os.environ['HEAD_NODE'], int(os.environ['HEAD_NODE_PORT']), int(os.environ['TIMESLOT_ID']), socket.gethostname())
   #         giveresults(os.environ['HEAD_NODE'], int(os.environ['HEAD_NODE_PORT']), int(os.environ['TIMESLOT_ID']), int(p[2].rstrip('\n')), 'out' + p[4].rstrip('\n'), datablock)
   #         os.spawnvp(os.P_WAIT, 'rm', ['rm', '-f', 'out' + p[4].rstrip('\n')])

def clienthandler(datablock, threadid, clientsock, addr, portno):
   datain = ''
   dowork = 0
   retval = 0
   while 1:
      try:
         newdata = clientsock.recv(datablock)
      except:
         time.sleep(1)
         clientsock.close()
         print('connection closed with ', addr)
         return 0
      if len(newdata) == 0:
         print('connection closed by ', addr)
         return 0
      datain = datain + newdata
      if datain.count('\n\n') > 0:
         st = datain.split('\n\n', 1)
         datain = st[1]
         p = st[0].split('\n')
         if p[0].rstrip('\n') == 'FINALRESULT':
            workpckt_id = -999
            contentlen = 0
            for k in p:
               if re.match('WorkPacketID: ', k):
                  workpckt_id = int((k.split(' ', 1))[1])
               if re.match('ContentLength: ', k):
                  contentlen = int((k.split(' ', 1))[1])
            print "EXPECTED RESULTS PACKET SIZE == %s bytes" % contentlen
            downloadedcontentlength = downloadstream(datablock, datain, clientsock, os.environ['OUTFILENAME'], addr)
            if not (downloadedcontentlength == contentlen):
               print('DOWNLOAD FAILED: ' + str(len(datain)) + ' ' + str(contentlen) + ' != ' + str(downloadedcontentlength))
         if p[0].rstrip('\n') == 'RECEIPT':
            workpckt_id = -999
            for k in p:
               if re.match('WorkPacketID: ', k):
                  workpckt_id = int((k.split(' ', 1))[1])
            print('WORKPACKET ID on BROKER: ' + str(workpckt_id))
         if p[0].rstrip('\n') == 'STATUS':
            workpckt_id = -999
            status = ''
            for k in p:
               if re.match('WorkPacketID: ', k):
                  workpckt_id = int((k.split(' ', 1))[1])
               if re.match('Status: ', k):
                  status = (k.split(' ', 1))[1]
            print('WORKPACKET ID ' + str(workpckt_id) + ' on BROKER is ' + status)
         if p[0].rstrip('\n') == 'RESERVATION':
            hostchain = '127.0.0.1'
            timeslot_number = -1
            caps = 'single'
            duration = 0.
            for k in p:
               if re.match('HostChain: ', k):
                  hostchain = (k.split(' ', 1))[1]
               if re.match('TimeSlot: ', k):
                  timeslot_number = int((k.split(' ', 1))[1])
               if re.match('Duration: ', k):
                  duration = float((k.split(' ', 1))[1])
               if re.match('RqrdCapabilities: ', k):
                  caps = (k.split(' ', 1))[1]
            if (hostchain.__len__() > 0):
               kfirsthostname, kfirsthostport, khostchaintail = splithostchain(hostchain)
               tsock = sendreservation(kfirsthostname, kfirsthostport, timeslot_number, khostchaintail, duration, caps)
               time.sleep(1)
               tsock.close()
            else:
               buildandsubmitscript(timeslot_number, duration, caps)
         if p[0].rstrip('\n') == 'FREE':
            hostchain = '127.0.0.1'
            timeslot_number = -1
            CPUs = 1
            GPUs = 0
            try_number = 0
            for k in p:
               if re.match('HostChain: ', k):
                  hostchain = (k.split(' ', 1))[1]
               if re.match('TimeSlot: ', k):
                  timeslot_number = int((k.split(' ', 1))[1])
               if re.match('Try: ', k):
                  try_number = int((k.split(' ', 1))[1])
               if re.match('CPUs: ', k):
                  CPUs = int((k.split(' ', 1))[1])
               if re.match('GPUs: ', k):
                  GPUs = int((k.split(' ', 1))[1])
            thissock = sendfree(os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']), timeslot_number, socket.gethostname() + ':' + str(portno) + '/' + hostchain, CPUs, GPUs, try_number)
            streambetween(thissock, clientsock, datablock)
            time.sleep(1)
            thissock.close()
            #clienthandler(datablock, threadid, thissock, os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']))
         if p[0].rstrip('\n') == 'WORK':
            hostchain = '127.0.0.1'
            timeslot_number = -999
            workpckt_id = -999
            workpckt_timeout = 0
            tarfilename = ''
            workname = ''
            contentlen = 0
            for k in p:
               if re.match('HostChain: ', k):
                  hostchain = (k.split(' ', 1))[1]
               if re.match('TimeSlot: ', k):
                  timeslot_number = int((k.split(' ', 1))[1])
               if re.match('WorkName: ', k):
                  workname = ((k.split(' ', 1))[1])
               if re.match('WorkPacketID: ', k):
                  workpckt_id = int((k.split(' ', 1))[1])
               if re.match('WorkTimeOut: ', k):
                  workpckt_timeout = float((k.split(' ', 1))[1])
               if re.match('FileName: ', k):
                  tarfilename = ((k.split(' ', 1))[1])
               if re.match('ContentLength: ', k):
                  contentlen = int((k.split(' ', 1))[1])
            downloadedcontentlength = downloadstream(datablock, datain, clientsock, tarfilename, addr)
            if not (downloadedcontentlength == contentlen):
               print('DOWNLOAD FAILED: ' + str(len(datain)) + ' ' + str(contentlen) + ' != ' + str(downloadedcontentlength))
            else:
               dowork = 1
            print('WORK ' + str(timeslot_number) + ' ' + hostchain)
         if p[0].rstrip('\n') == 'WAIT':
            timeout = 0.
            for k in p:
               if re.match('TimeOut: ', k):
                  timeout = float((k.split(' ', 1))[1])
            retval = 2
         if p[0].rstrip('\n') == 'TERMINATE':
            retval = 1
#            (p[1].lsplit(' ', 1))[1]
#            t_tsid = int(p[1].rstrip('\n'))
#            tsid_to_node[t_tsid] = p[2].rstrip('\n')
#            thisock = sendfree(os.environ['PARENT_NODE'] + '/' + , int(os.environ['PARENT_NODE_PORT']), t_tsid, p[2].rstrip('\n'), 1, 1)
#            clienthandler(tsid_to_node, datablock, 9999, thisock, addr)
         if p[0].rstrip('\n') == 'RESULTS':
            hostchain = '127.0.0.1'
            timeslot_number = -999
            workpckt_id = -999
            overhead_time = 0.
            working_time = 0.
            tarfilename = ''
            workname = ''
            contentlen = 0
            for k in p:
               if re.match('HostChain: ', k):
                  hostchain = (k.split(' ', 1))[1]
               if re.match('TimeSlot: ', k):
                  timeslot_number = int((k.split(' ', 1))[1])
               if re.match('WorkPacketID: ', k):
                  workpckt_id = int((k.split(' ', 1))[1])
               if re.match('WorkName: ', k):
                  workname = ((k.split(' ', 1))[1])
               if re.match('OverheadTime: ', k):
                  overhead_time = float((k.split(' ', 1))[1])
               if re.match('WorkingTime: ', k):
                  working_time = float((k.split(' ', 1))[1])
               if re.match('FileName: ', k):
                  tarfilename = ((k.split(' ', 1))[1])
               if re.match('ContentLength: ', k):
                  contentlen = int((k.split(' ', 1))[1])
            thissock = sendresultsheader(os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']), workname, timeslot_number, workpckt_id, hostchain, tarfilename, datablock, overhead_time, working_time, contentlen)
            thissock.send(datain)
            streambetween(clientsock, thissock, datablock)
            time.sleep(1)
            thissock.close()
         time.sleep(1)
         clientsock.close()
         print('connection closed with ', addr)
         if (dowork == 1):
            filemaniptime, workingtime = doworkpacket(tarfilename, workpckt_timeout, timeslot_number)
            resultsock = sendresults(addr, portno, workname, timeslot_number, workpckt_id, hostchain, 'out' + tarfilename, datablock, filemaniptime, workingtime)
            time.sleep(1)
            resultsock.close()
         if (retval == 2):
            time.sleep(timeout)
         return retval

def serversocket(portno, datablock):
   client_retries = 0
   threadid = 0
   if (os.getenv('ISDOWNLOAD') != None):
      newsock = senddownload(os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']), os.environ['WKPTID'])
      clienthandler(datablock, 0, newsock, os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']))
      return
   elif (os.getenv('ISQUERY') != None):
      newsock = sendquery(os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']), os.environ['WKPTID'])
      clienthandler(datablock, 0, newsock, os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']))
      return
   elif (os.getenv('ISSUBMIT') != None):
      newsock = sendsubmit(os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']), os.environ['WORKNAME'], os.environ['CLASSNAME'], os.environ['FILENAME'])
      uploadstream(datablock, '', newsock, os.environ['FILENAME'], os.environ['PARENT_NODE'])
      clienthandler(datablock, 0, newsock, os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']))
      time.sleep(1)
      newsock.close()
      return
   elif (os.getenv('ISLEAF') != None):
      quitflag = 0
      while not quitflag:
         newsock = sendfree(os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']), int(os.environ['TIMESLOT_ID']), socket.gethostname() + ':LEAF', 1, 1, client_retries + 1)
         quitflag = clienthandler(datablock, 0, newsock, os.environ['PARENT_NODE'], int(os.environ['PARENT_NODE_PORT']))
         if quitflag == 2:
            client_retries = client_retries + 1
         else:
            client_retries = 0
         if quitflag == 2: quitflag = 0
         if (client_retries > 10):
            quitflag = 1
         time.sleep(5)
   else:
      sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#      sock.bind((socket.gethostname(), portno))
      sock.bind(("0.0.0.0", portno))
      sock.listen(5)
      try:
         while 1:
            threadid = threadid + 1
            clientsock, addr = sock.accept()
            print('connection made with ', addr)
            thread.start_new_thread(clienthandler, (datablock, threadid, clientsock, addr, portno))
      except:
         print('Performing clean up...')
         sock.close()

if (os.getenv('ISLEAF') == None) and (os.getenv('ISDOWNLOAD') == None) and (os.getenv('ISSUBMIT') == None) and (os.getenv('ISQUERY') == None):
   serversocket(int(os.environ['MYPORT']), 65536)
else:
   serversocket(0, 65536)
