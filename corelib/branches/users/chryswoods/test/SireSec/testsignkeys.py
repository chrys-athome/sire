
from Sire.Sec import *
from Sire.Siren import *
from Sire.Qt import *

(encrypt_public, encrypt_private) = PrivateKey.generate( Key.LockedToThread )
(sign_public, sign_private) = PrivateKey.generate()

lock = SignedPubPriLock( encrypt_public, sign_private,
                         encrypt_private, sign_public, 
                         Cipher.Blowfish, MACType.SHA2 )

encrypted_data = lock.encryptString("Hello World!")
print encrypted_data.length()

print lock.decryptString(encrypted_data)

lock2 = SignedPubPriLock( encrypt_public, encrypt_private,
                          encrypt_private, encrypt_public )

try:
    print lock2.decryptString(encrypted_data)
except:
    print "Signature verification failed (expected)"

lock2 = SignedPubPriLock( sign_public, sign_private,
                          sign_private, sign_public )

try:
    print lock2.decryptString(encrypted_data)
except:
    print "Decryption failed (expected)"

lock.encryptFile("test/SireSec/testsignkeys.py", "test/SireSec/testsignkeys_encrypted")
lock.decryptFile("test/SireSec/testsignkeys_encrypted", "test/SireSec/testsignkeys.py")
