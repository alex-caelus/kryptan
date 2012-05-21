/* 
 * File:   EncryptedFile.h
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 13:08
 */

#ifndef ENCRYPTEDFILE_H
#define	ENCRYPTEDFILE_H

#include "CONSTANTS.h"
#include "StringContainer.h"

#include <fstream>


using std::fstream;
using std::string;
using std::string;

class EncryptedFile : public StringContainer{
public:
    EncryptedFile(string filename);
    virtual ~EncryptedFile();
    
    bool createFile(string key);
    bool setKey(string key);
    bool changeKey();
    bool ready();
    bool hasKey(){return _key != NULL;};
    bool isCorrupt(){return _corrupt;};
    
    char* getContents();
    bool setContents(const char*, unsigned int);
    
    bool EncryptString(const char* csPassPhrase, const char* csInStr, unsigned int inLen, char* &szOutstr, unsigned int &unOutputLength);
    bool DecryptString(const char* csPassPhrase, const char* csInStr, unsigned int inLen, char* &szOutstr, unsigned int &unOutputLength);
private:
    string _filename;
    fstream _file;
    char* _key;
    char* _decryptedContents;
    bool _corrupt;
    
    bool openW();
    bool openR();
    bool close();
    
    EncryptedFile(const EncryptedFile& orig);
};

#endif	/* ENCRYPTEDFILE_H */