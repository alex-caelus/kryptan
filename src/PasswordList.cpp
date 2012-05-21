/* 
 * File:   PasswordList.cpp
 * Author: Alexander-i7
 * 
 * Created on den 8 februari 2012, 11:30
 */

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>

#include "CONSTANTS.h"
#include "PasswordList.h"

using std::getline;
using std::endl;



PasswordList::PasswordList(StringContainer* container) {
    _container = container;
}

PasswordList::PasswordList(const PasswordList& orig) {
}

PasswordList::~PasswordList() {
}

bool PasswordList::parse(){
    char* data = _container->getContents();
    if(data == NULL){
        _strerror.str("");
        _strerror << _container->getLastError();
        return false;
    }
    stringstream content(data);
    
    string firstLine;
    getline(content, firstLine);
    if(firstLine != FILESTART){
        _strerror.str("");
        _strerror << ERROR_FILE_STRUCTURE;
        return false;
    }
    
    while(!content.eof()){
        string delimiter;
        string description;
        string uname;
        string pwd;
        getline(content, delimiter);
        if(delimiter != PASSWORDSTART){
            if(delimiter == FILEEND){
                break;
            } else {
                _strerror.str("");
                _strerror << ERROR_FILE_STRUCTURE_CORRUPT;
                _strerror << _list.size();
                return false;
            }
        }
        getline(content, description);
        getline(content, uname);
        getline(content, pwd);
        if(description == FILEEND || pwd == FILEEND || uname == FILEEND || 
                description.empty() || pwd.empty() || uname.empty() || content.eof()){
            _strerror.str("");
            _strerror << ERROR_FILE_STRUCTURE_EOF;
            _strerror << _list.size();
            return false;
        } if(uname == EMPTYLINE){
            uname = "";
        }
        _list.push_back(Password(description, uname, pwd));
    }
    return true;
}

bool PasswordList::save(){
    /* Structure output */
    stringstream content;
    content << FILESTART << endl;
    for(deque<Password>::iterator it = _list.begin(); it != _list.end(); it++){
        content << PASSWORDSTART << endl;
        //first description
        content << it->getDesc() << endl;
        //second username (if it exists)
        if(it->getUname().empty())
            content << EMPTYLINE << endl;
        else
            content << it->getUname() << endl;
        //And pasword last
        content << it->getPwd()  << endl;
    }
    content << FILEEND << endl;
    
    /* Prepare data to container */
    string output = content.str();
    const char* buffer = output.c_str();
    unsigned int buflen = output.length();
    /* save to container */
    if(!_container->setContents(buffer, buflen)){
        _strerror.str("");
        _strerror << _container->getLastError();
        return false;
    }
    return true;
}

vector<string> PasswordList::getList(){
    stringstream strlist;
    vector<string> vlist;
    //strlist << "\nPassword list:\n";
    for(deque<Password>::iterator it = _list.begin(); it != _list.end(); it++){
        int i = it - _list.begin() + 1;
        strlist << i << ". " << it->getDesc() << '\n';
    }
    if(_list.empty()){
        strlist << NO_PASSWORDS;
    }
    while(!strlist.eof()){
        char buff[100];
        strlist.getline(buff, 100);
        string b(buff);
        if(b.length() > 1)
            vlist.push_back(b);
    }
    return vlist;
}

string PasswordList::getPassword(int i){
    return _list.at(i-1).getPwd();
}

string PasswordList::getDescription(int i){
    return _list.at(i-1).getDesc();
}

string PasswordList::getUsername(int i){
    return _list.at(i-1).getUname();
}

bool PasswordList::setUsername(int i, string uname){
    _list.at(i-1).setUname(uname);
    return save();
}

bool PasswordList::setPassword(int i, string pwd){
    _list.at(i-1).setPwd(pwd);
    return save();
}

bool PasswordList::setDescription(int i, string desc){
    _list.at(i-1).setDesc(desc);
    return save();
}

bool PasswordList::hasUsername(int i){
    return !_list.at(i-1).getUname().empty();
}

bool PasswordList::deletePassword(int i){
    deque<Password>::iterator it = _list.begin();
    it += i-1;
    _list.erase(it);
    return save();
}

/**
 * This makes use of insertion sort
 * @param desc
 * @param uname
 * @param pwd
 * @return 
 */
bool PasswordList::newPassword(string desc, string uname, string pwd){
    Password newPass(desc, uname, pwd);
    deque<Password>::iterator it = _list.begin();
    for(; it != _list.end(); it++){
        if( desc < (*it).getDesc()){
            _list.insert(it, newPass);
            return save();
        }
    }
    _list.push_back(newPass);
    //if we have reached this point then the list is sorted and we append the new element;
    return save();
}