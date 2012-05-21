/* 
 * File:   PasswordFile.h
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 12:39
 */

#ifndef PASSWORDFILE_H
#define	PASSWORDFILE_H

#include <string>

using std::string;

class StringContainer {
public:
    virtual char* getContents() = 0;
    virtual bool setContents(const char*, unsigned int) = 0;
    string getLastError(){return _strerror;};
protected:
    string _strerror;
};

#endif	/* PASSWORDFILE_H */

