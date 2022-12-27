import gnupg

gpg = gnupg.GPG(gnupghome="/home/pranav/Data/development/Simple-Projects/PasswordManager/UserHome")
email = input("Enter your email: ")
passphrase = input("Enter a new password for the key: ")

input_data = gpg.gen_key_input(name_email=email, passphrase=passphrase)
key = gpg.gen_key(input_data)
print(key)

ascii_armored_public_keys = gpg.export_keys(key.fingerprint)
ascii_armored_private_keys = gpg.export_keys(key.fingerprint, True, passphrase=passphrase)

open("UserHome/public.asc", "w").write(ascii_armored_public_keys)
open("UserHome/private.asc", "w").write(ascii_armored_private_keys)
