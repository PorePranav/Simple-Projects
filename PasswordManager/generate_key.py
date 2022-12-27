import gnupg

gpg = gnupg.GPG(gnupghome="/home/pranav/Data/development/Simple-Projects/PasswordManager/UserHome")
input_data = gpg.gen_key_input(name_email='test@test.com', passphrase='passphrase')
key = gpg.gen_key(input_data)
print(key)

ascii_armored_public_keys = gpg.export_keys(key.fingerprint)
ascii_armored_private_keys = gpg.export_keys(key.fingerprint, True, passphrase="passphrase")

print(ascii_armored_private_keys)
print(ascii_armored_public_keys)

open("mykeyfile.asc", "w").write(ascii_armored_public_keys)
open("mykeyfileprivate.asc", "w").write(ascii_armored_private_keys)
