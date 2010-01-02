# Parameter files
parfile1 ../../../parameter/amber99.ff
parfile2 ../../../parameter/solvents.ff
parfile3 ../../../parameter/gaff.ff
parfile4 ethane2methanol.par
# PDB Files
solute1 ethane2methanol.pdb
solvent1 boxT4P.pdb
#set the output files
streamwarning warning
streamfatal fatal
streamheader off
streaminfo off
streamdetail off
streamaccept off
cutoff 10.00
feather 0.5
boundary solvent
nptsim on
pressure 1.0
prefsampling 1 200.0


