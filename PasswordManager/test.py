import gnupg

gpg = gnupg.GPG(gnupghome="/home/pranav/Data/development/Simple-Projects/PasswordManager/UserHome")
f = open("unencrypted-file.txt", "rb")

status = gpg.encrypt_file(f, recipients=["pranav@pranavpore.com"], output="encrypted.txt.gpg", always_trust=True)
status.data

print ('ok: ', status.ok)
print ('status: ', status.status)
print ('stderr: ', status.stderr)