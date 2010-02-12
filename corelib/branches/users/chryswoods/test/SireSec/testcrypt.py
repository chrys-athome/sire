
from Sire.Sec import *
from Sire.Qt import *

lock = PasswordLock( Password("long and complicated password with lots of text") )

encrypted_data = lock.encrypt(QByteArray("Hello World. This is a really long piece of text."))
decrypted_data = lock.decrypt(encrypted_data)

print decrypted_data.data()

encrypted_data = lock.encryptString("Hello World!")

print lock.decryptString(encrypted_data)

