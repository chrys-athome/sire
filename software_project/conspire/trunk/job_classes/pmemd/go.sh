#!/bin/bash

if [ "$CONSPIRE_CLUSTERID" = "pretend" ]; then
   if [ -z "$AMBERHOME" ]; then
      export AMBERHOME=$HOME/amber12
   fi
fi
if [ "$CONSPIRE_CLUSTERID" = "bluecrystalp1" ]; then
   echo "Bluecrystal phase 1 does not have amber installed!"
fi
if [ "$CONSPIRE_CLUSTERID" = "bluecrystalp2" ]; then
   module add apps/amber11-serial
fi
python2 run_md.py . 0

