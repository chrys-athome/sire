#!/bin/bash

export NUMBER_AMBER_CPUS=1
export NUMBER_AMBER_GPUS=0
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
if [ "$CONSPIRE_CLUSTERID" = "emerald" ]; then
   module add intel
   module add cuda
   module add amber
   export NUMBER_AMBER_GPUS=3
fi
python2 run_md.py $NUMBER_AMBER_CPUS $NUMBER_AMBER_GPUS
