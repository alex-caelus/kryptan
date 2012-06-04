/* 
 * File:   EncryptedFile.cpp
 * Author: Alexander-i7
 * 
 * Created on den 8 februari 2012, 13:08
 */

#include <sstream>
#include <string.h>

#include "EncryptedFile.h"

using std::stringstream;

EncryptedFile::EncryptedFile(string filename) {
    _filename = filename;
    _decryptedContents = NULL;
    _key = NULL;
    _strerror = NO_ERROR;
    _corrupt = false;
}

EncryptedFile::EncryptedFile(const EncryptedFile& orig) {
}

EncryptedFile::~EncryptedFile() {
    if (_decryptedContents) {
        delete _decryptedContents;
    }
    if (_key) {
        delete _key;
    }
}

bool EncryptedFile::createFile(string key) {
    _key = new char[key.length() + 1];
    strncpy(_key, key.c_str(), key.length() + 1);
    string tmpbuff = NEW_FILE_TMP_CONTENT;
    bool status = setContents(tmpbuff.c_str(), tmpbuff.length());
    return status;
}

bool EncryptedFile::openW() {
    close();
    _file.open(_filename.c_str(), fstream::out | fstream::trunc | fstream::binary);
    if (!_file.is_open()) {
        _strerror = ERROR_WRITE_FILE;
        return false;
    }
    _file.flush();
    return true;
}

bool EncryptedFile::openR() {
    close();
    /* Does the file exist? */
    _file.open(_filename.c_str(), fstream::in | fstream::binary);
    if (!_file) {
        _strerror = ERROR_READ_FILE1 + _filename + ERROR_READFILE2;
        return false;
    }
    return true;
}

bool EncryptedFile::ready() {
    if (!openR())
        return false;
    close();
    return true;
}

bool EncryptedFile::setKey(string key) {
    _key = new char[key.length() + 1];
    strncpy(_key, key.c_str(), key.length() + 1);
    char* contents = getContents();
    if (contents == NULL) {
        delete _key;
        _key = NULL;
        return false;
    }
    return true;
}

bool EncryptedFile::close() {
    if (_file && _file.is_open())
        _file.close();
    return true;
}

bool EncryptedFile::changeKey() {
    return false;
}

char* EncryptedFile::getContents() {
    /* If the file has already been decrypted, there is no reason to do it again. */
    if (_decryptedContents) {
        return _decryptedContents;
    }
    if (!openR()) {
        return NULL;
    }
    unsigned int length, decryptedLength;
    char * buffer;

    // get length of file:
    _file.seekg(0, fstream::end);
    length = (unsigned int) _file.tellg();
    _file.seekg(0, fstream::beg);

    // allocate memory:
    buffer = new char [length];

    // read data as a block:
    _file.read(buffer, length);

    DecryptString(_key, buffer, length, _decryptedContents, decryptedLength);

    delete buffer;
    close();

    return _decryptedContents;
}

bool EncryptedFile::setContents(const char* content, unsigned int length) {
    /* if there are bufferd contents from the file, remove it*/
    if (_decryptedContents) {
        delete _decryptedContents;
        _decryptedContents = NULL;
    }
    /* open file for writing */
    if (!openW()) {
        return false;
    }
    /* encrypt content */
    char* buffer;
    unsigned int encryptedLength;
    if (EncryptString(_key, content, length, buffer, encryptedLength)) {
        /* write to file */
        _file.write(buffer, encryptedLength);
    }

    /* clean up */
    delete buffer;
    close();

    return true;
}

#include <cryptopp/default.h>
#include <cryptopp/hex.h>
#include <stdlib.h>

using namespace CryptoPP; // Cryptlib source uses a namespace

bool EncryptedFile::EncryptString(const char* csPassPhrase, const char* csInStr, unsigned int inLen, char* &szOutstr, unsigned int &unOutputLength) {
    try {
        unOutputLength = inLen;
        szOutstr = new char[unOutputLength + 1];
		memcpy(szOutstr, csInStr, unOutputLength);
        szOutstr[unOutputLength] = 0;
    } catch (Exception const& e) {
        _strerror = e.GetWhat();
        return false;
    }

    return true;
}

bool EncryptedFile::DecryptString(const char* csPassPhrase, const char* csInStr, unsigned int inLen, char* &szOutstr, unsigned int &unOutputLength) {
    DefaultDecryptorWithMAC *p;
    try {
        HexDecoder decryptor(p = new DefaultDecryptorWithMAC(csPassPhrase));
        decryptor.Put((byte *) csInStr, inLen);
        decryptor.MessageEnd();

        unOutputLength = (unsigned int) decryptor.MaxRetrievable();
        szOutstr = new char[unOutputLength + 1];
        decryptor.Get((byte *) szOutstr, unOutputLength);
        szOutstr[unOutputLength] = 0;

    } catch (DefaultDecryptor::KeyBadErr const& e) {
        _strerror = ERROR_MASTER_KEY;
        return false;
    } catch (DefaultDecryptorWithMAC::MACBadErr const& e) {
        _strerror = ERROR_MAC_VALIDATION;
        _corrupt = true;
        return false;
    } catch (Exception const& e) {
        _strerror = e.GetWhat();
        _corrupt = true;
        return false;
    }

    return true;
}