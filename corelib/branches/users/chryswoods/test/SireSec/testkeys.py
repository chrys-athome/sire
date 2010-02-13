
from Sire.Sec import *

(public, private) = PrivateKey.generate()

(public2, private2) = PrivateKey.generate()

lock = PubPriLock( public, private, Cipher.Blowfish )

encrypted_data = lock.encryptString("Hello World!")
print encrypted_data.length()

print lock.decryptString(encrypted_data)

lock = PubPriLock( public2, private2, Cipher.Blowfish )

try:
    print lock.decryptString(encrypted_data)
except:
    print "Could not decrypt the data! (expected!)"

encrypted_data = lock.encryptString("Hello World!")

print lock.decryptString(encrypted_data)

lock = PubPriLock( public, private2 )

print lock.decryptString(encrypted_data)

encrypted_data = lock.encryptString("Hello World!")

print encrypted_data.length()

try:
    print lock.decryptString(encrypted_data)
except:
    print "Could not decrypt the data! (expected!)"
