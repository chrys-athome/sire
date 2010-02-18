
from Sire.Sec import *
from Sire.Siren import *
from Sire.Qt import *

(public, private) = PrivateKey.generate( Key.LockedToThread )

lock = SignatureLock( public, private, MACType.SHA2 )

signed_data = lock.encryptString("Hello World!")
print signed_data.length()

verified = lock.decryptString(signed_data)

print verified.length()
print verified
