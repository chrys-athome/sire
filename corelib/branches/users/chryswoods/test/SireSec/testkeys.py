
from Sire.Sec import *
from Sire.Siren import *
from Sire.Qt import *

(public, private) = PrivateKey.generate( Key.LockedToThread )

(public2, private2) = PrivateKey.generate()

print public
print public2

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

bytearray = QByteArray()

ds = DataStream(bytearray, QIODevice.WriteOnly)

public2.save(ds)

public3 = PublicKey()

print bytearray.length()

ds = DataStream(bytearray)

public3.load(ds)

lock2 = PubPriLock(public3)

encrypted_data = lock2.encryptString("Hello from streamed key")

print lock.decryptString(encrypted_data)
