
from Sire.Sec import *
from Sire.Qt import *

lock = PasswordLock( Password("password") )

encrypted_data = lock.encrypt(QByteArray("Hello World. This is a really long piece of text."))

decrypted_data = lock.decrypt(encrypted_data)

print encrypted_data.data()
print decrypted_data.data()

