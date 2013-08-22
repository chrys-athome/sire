
from Sire.Base import *

import shutil
import sys
import os
import tempfile

siredir = getInstallDir()

print("\n#################################################################")
print(  "# This script will create a sire.run install file for the copy")
print(  "# of Sire running in directory %s" % siredir)
print(  "#################################################################\n")

sys.stdout.write("Please provide a name for the installer (sire.run) : ",)
sys.stdout.flush()

sire_run = sys.stdin.readline()

try:
    sire_run = sire_run.lstrip().rstrip()
except:
    sire_run = ''

if len(sire_run) == 0:
    sire_run = "sire.run"

print( "\nCreating Sire installer \"%s\"..." % (sire_run) )

makeself = "%s/build/makeself.sh" % getShareDir()
install_sire = "%s/build/install_sire.sh" % getShareDir()

# create a directory that will contain the files to be packages
with tempfile.TemporaryDirectory() as tempdir:
    print("Copying %s to %s/sire.app" % (getInstallDir(),tempdir))
    shutil.copytree(getInstallDir(), "%s/sire.app" % (tempdir), symlinks=True)
    print("Copying %s to %s/install_sire.sh" % (install_sire, tempdir))
    shutil.copyfile(install_sire, "%s/install_sire.sh" % tempdir)

    print("Using 'makeself' to create the self-extracting installer...")
    os.system("%s --current %s %s \"Sire Molecular Simulation Framework\" ./install_sire.sh" \
                   % (makeself, tempdir, sire_run))

print( "\nAll done :-). Just type %s to install Sire." % sire_run )
