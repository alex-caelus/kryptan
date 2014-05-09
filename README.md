Kryptan
=======

What is Kryptan?

A secure password storage application, for both Windows and Linux. 
It uses a text-based interface and works great on linux over SSH, as 
well as a standalone Windows desktop application.

* All passwords are stored using industrial grade encryption (256-bit Serpent-cipher).
* Retrieval of passwords needs a decryption key (a.k.a. Master-Key).
* The encryption key is generated from your Master-Key using industry standard PBKDF2 with a 2 second iteration time (in short: the security of your password file increases with the processing speed of your CPU).
* Local standalone application (No reliance on 3rd pary servers you don't trust).
* Manual and easy syncronization with smarthpone applications, using direct and ecnrypted TCP connections (again, no reliance on 3rd party servers).
* Multi platform, works on Windows (desktop) and linux (command line).
* Open Source, if you don't trust me, check the code.

Where to get it?
----------------

The Github project is located [here](https://github.com/alex-caelus/kryptan/releases).

How do I use it?
----------------

Simply start the program by douple clicking on the executable file (kryptan.exe on windows).

Not that this is a keyboard only application, but it should be realatively straight forward to use, simply follow the on screen instructions.

The password file that is generated is located in the same directory as the applcation itself ("current working directory") and is called `secret.pwd`.

`secret-pwd` is the file that contains all your passwords and this file should always be backed up on a secure location, incase something bad happens (like a hard drive crash).

Where do i report bugs/feature requests?
----------------------------------------

[Here](https://github.com/alex-caelus/kryptan/issues), please check wheather or not a simmilar issue/request already exists before you posts.

How do I contribute?
--------------------

Visit the GitHub project and make a fork of it, make any changes you want and then make a pull request and I'll look into it :).

Change Log
----------

### 2014-04-27 - VERSION 3.1.2
New encryption engine, using serpent encryption and much safer encryption key derivation.
It is now possible to syncronize your password file to Android devices running Kryptan for Android!

### 2013-08-25 - VERSION 3.0
Completely rewritten both core and userinterface in order to make it more structured and more usable.
Nested categories have now been replaced by a flat list of labels, where a password can be associated by several labels.
New filter function where the list of passwords that matches what you type is updated in realtime.

### 2012-07-05 - VERSION 2.1
Updated: Stronger encryption! The new version replaces the old 3DES (DES-EDE2) with the much more secure AES 256-bit encryptionmethod!
Fixed: "Generate password" now outputs a key with correct keylength!
Fixed: Compiler warning messages are now handled properly!

### 2012-06-04 - VERSION 2.0
Major update! Completely rewritten code base!
Added capability to organize passwords into categories (or folders).
Updated passwordfile structure, that is incompatible with the old. Please move your passwords manualy to the new version.

### 2012-05-09 - VERSION 1.1.1
Removed the 1.1 fix, instead updated the pdcurses lib to a newer version that included its own (better) fix.
Keyboardlayout and latin-1 character encoding should now work correctly.

### 2012-05-01 - VERSION 1.1
Fixed keyboard layout on swedish windows machines.
Fixed characters on swedish windows machines.

### 2012-03-02 - VERSION 1.0
Original release, using Password-Based Encryptor with DES-EDE2 and HMAC/SHA-1 with crypto++ 5.6.1

