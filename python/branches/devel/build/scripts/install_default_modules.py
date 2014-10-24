
import sys
import os

installed_something = False

try:
    import sqlite3
except:
    print("ERROR: Cannot import sqlite3. Something went wrong with compilation!")
    sys.exit(-1)

try:
    import zlib
except:
    print("ERROR: Cannot import zlib. Something went wrong with compilation!")
    sys.exit(-1)

try:
    import ssl
except:
    print("ERROR: Cannot import ssl. Something went wrong with compilation!")
    sys.exit(-1)

try:
    import easy_install
except:
    print("ERROR: Cannot import easy_install. Something went wrong with compilation!")
    sys.exit(-1)

try:
    import pip
except:
    print("ERROR: Cannot import pip. Something went wrong with compilation!")
    sys.exit(-1)

os.environ["PYTHONPATH"] = ""

try:
    import nose
except:
    print("nose not installed. Installing using pip...")
    pip.main(["install", "nose"])
    installed_something = True

try:
    import IPython
except:
    print("ipython not installed. Installing using pip...")
    pip.main(["install", "ipython"])
    installed_something = True

if installed_something:
    print("\nNeed to scan imported libraries to update @rpath...")

