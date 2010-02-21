
from Sire.Sec import *
from Sire.Siren import *
from Sire.Qt import *

(public, private) = PrivateKey.generate( Key.LockedToThread )
(public2, private2) = PrivateKey.generate( Key.LockedToThread )

lock = SignatureLock( public, private, MACType.SHA2 )
lock2 = SignatureLock( public2, private2 )

signed_data = lock.encryptString("Hello World!")

verified = lock.decrypt(signed_data)
print verified.constData()

try:
    verified = lock2.decrypt(signed_data)
    print verified.constData()
except:
    print "Verification failed (expected)"

lock.encryptFile("test/SireSec/testsig.py", "test/SireSec/testsig_signature")
lock.decryptFile("test/SireSec/testsig_signature", "test/SireSec/testsig.py")
