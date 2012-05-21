/* 
 * File:   PasswordList.h
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 11:30
 */

#ifndef PASSWORDLIST_H
#define	PASSWORDLIST_H

#include <deque>
#include <sstream>
#include <vector>

#include "StringContainer.h"

using std::deque;
using std::vector;
using std::stringstream;

class PasswordList {
public:
    PasswordList(StringContainer* container);
    virtual ~PasswordList();
    
    bool parse();
    bool save();
    bool isDecrypted(){return _decrypted;};
    vector<string> getList();
    string getPassword(int i);
    string getDescription(int i);
    string getUsername(int i);
    bool setUsername(int i, string uname);
    bool setPassword(int i, string pwd);
    bool setDescription(int i, string desc);
    bool hasUsername(int i);
    bool deletePassword(int i);
    string getLastError(){return _strerror.str();};
    
    bool newPassword(string, string, string);
    
    int getSize(){
        return _list.size();
    }
private:
    
    class Password{
        public:
            Password(string description, string uname, string pwd){
                this->pwd = pwd;
                this->username = uname;
                this->description = description;
            }
            
            string getPwd(){
                return pwd;
            };
            
            string getDesc(){
                return description;
            };
            
            string getUname(){
                return username;
            };
            
            void setUname(string uname){
                username = uname;
            }
            
            void setPwd(string npwd){
                pwd = npwd;
            };
            
            void setDesc(string ndesc){
                description = ndesc;
            };
        private:
            string pwd;
            string username;
            string description;
    };
    
    stringstream _strerror;
    bool _decrypted;
    deque<Password> _list;
    StringContainer* _container;
    PasswordList(const PasswordList& orig);
};

#endif	/* PASSWORDLIST_H */

