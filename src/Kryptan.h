/* 
 * File:   Cryptospwdus.h
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 09:51
 */

#ifndef CRYPTOSPWDUS_H
#define	CRYPTOSPWDUS_H

#include "EncryptedFile.h"
#include "PasswordList.h"
#include "UI.h"

class Kryptan {
public:
    static void run(bool useAntiKeylogging);
private:
    static bool hasRun;
    static PasswordList* list;
    static UI* ui;
    static EncryptedFile* pwdfile;
    
    static void setUp();
    static void showPwd();
    static void createNewFile();

    static void addPwd();
    static void editPwd();
    static void removePwd();
    static void generatePwd();
    static void changeMaster();
    static void showHelp();
    
    Kryptan();
    Kryptan(const Kryptan& orig);
    virtual ~Kryptan();
};

#endif	/* CRYPTOSPWDUS_H */

