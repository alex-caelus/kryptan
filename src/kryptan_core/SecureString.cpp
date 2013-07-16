#include "SecureString.h"

#include <string.h>
#include <algorithm>

#define DEFAULT_ALLOCATED 80

using namespace Kryptan::Core;

SecureString::SecureString(void){
	init();
}

//SecureString::SecureString(ssnr size){
//	init();
//	allocate(size);
//}

SecureString::SecureString(ssarr str, ssnr maxlen, bool deleteStr){
	init();
	assign(str, maxlen, deleteStr);
}

SecureString::SecureString(c_ssarr str, ssnr maxlen){
	init();
	assign(str, maxlen);
}

SecureString::SecureString(const SecureString& src){
	init();
	assign(src);
}

SecureString::~SecureString(void)
{
	//Destroy all unsecured data
	UnsecuredStringFinished();

	//Zero out all data
	memset(_data, 0, allocated());
	memset(_key, 0, allocated());
	_length = 0;
	_allocated = 0;

	//deallocate arrays
	delete[] _data;
	delete[] _key;
}

void SecureString::init(){
	_data = new ssbyte[sizeof(ssnr)];
	_key  = new ssbyte[sizeof(ssnr)];
	//fill key with zeros, this keeps the length() and allocated() from failing before any call to allocate(x)
	*((ssnr*)_data) = 0; 
	*((ssnr*)_key) = 0;
	_length = 0;
	_allocated = 0;
	_plaintextcopy = NULL;
	_mutableplaintextcopy = false;
	resetLinefeedPosition();
}

void SecureString::allocate(ssnr size){
	//increase size by one to include last '\0'
	size += 1;
	//the new array must at least be able to hold a key the size of ssnr
	if(size <= sizeof(ssnr)){
		size = sizeof(ssnr) + 1; //include last '\0'
	}
	//check if new size is larger than current string length
	if(size <= length()){
		size = length() + 1; //include last '\0'
	}

	//create the new arrays
	ssarr newdata = new ssbyte[size];
	ssarr newkey  = new ssbyte[size];

	//store the length of the string
	ssnr strlen = length();
	
	//fill the key array with random data
	//and the data array as a mirror (xor results in zero)
	for(ssnr i=0; i<size; i++){
		newkey[i] = (ssbyte)rand();
		newdata[i] = newkey[i];
	}

	//copy existing data over to the new array
	if(allocated()){
		ssnr nrOfOldAllocatedBytes = allocated();
		//After this the old key and data is zeroed out (length() and allocate() will not work)
		for(ssnr i=0; i < nrOfOldAllocatedBytes; i++){
			newdata[i] = newkey[i] ^ (_key[i] ^ _data[i]);
			//fill old data and key with zeroes
			_data[i] = 0;
			_key[i] = 0;
		}
	}
	_length = strlen ^ ((ssnr)*newkey);
	_allocated = (size-1) ^ ((ssnr)*newkey);

	//deallocate the old arrays
	delete[] _data;
	delete[] _key;
	_data = newdata;
	_key  = newkey;
}

void SecureString::append(ssarr str, ssnr maxlen, bool deleteStr){
	//set len to strlen(str) or maxlen, wichever is lowest (except if maxlen is 0 then set len to strlen(0))
	ssnr len = (maxlen == 0) ? strlen(str) : std::min((ssnr)strlen(str), maxlen);
	ssnr oldlen = length();
	//calculate the new total length
	ssnr totlen = oldlen + len;
	//Check if there is room for the new string
	if(totlen > allocated()){
		allocate(totlen*2); //make more room than neccessary, just in case there will be more appends later
	}
	for(ssnr i=0; i<len; i++){
		_data[oldlen+i] = _key[oldlen+i] ^ str[i];
	}
	_length =  ((ssnr)*_key) ^ totlen;
	if(deleteStr){
		memset(str, 0, len);
		delete[] str;
	}
	resetLinefeedPosition();
}

void SecureString::append(c_ssarr str, ssnr maxlen){
    append((ssarr)str, maxlen, false);
}

void SecureString::append(const SecureString& str){
	ssnr len = str.length();
	ssnr oldlen = this->length();
	ssnr totlen = oldlen + len;
	if(totlen > this->allocated()){
		this->allocate(totlen*2); //make more room than neccessary, just in case there will be more appends later
	}
	for(ssnr i=0; i<len; i++){
		this->_data[i+oldlen] = this->_key[oldlen+i] ^ (str._data[i] ^ str._key[i]);
	}
	_length =  ((ssnr)*_key) ^ totlen;
	resetLinefeedPosition();
}

void SecureString::assign(ssarr str, ssnr maxlen, bool deleteStr){
	//set len to strlen(str) or maxlen, wichever is lowest (except if maxlen is 0 then set len to strlen(0))
	ssnr len = (maxlen == 0) ? strlen(str) : std::min((ssnr)strlen(str), maxlen);

	//remove old data
	if(length() > 0){
		ssnr oldlen = length();
		memcpy(_data, _key, oldlen);
	} 
	
	//allocate enough space
	if(len > allocated()){
		allocate(len*2); //make more room than neccessary, just in case there will be more appends later
	}
	for(ssnr i=0; i<len; i++){
		_data[i] = _key[i] ^ str[i];
	}
	_data[len] = _key[len];
	_length =  ((ssnr)*_key) ^ len;
	if(deleteStr){
		memset(str, 0, len);
		delete[] str;
	}
}

void SecureString::assign(c_ssarr str, ssnr maxlen){
    assign((ssarr)str, maxlen, false);
}

void SecureString::assign(const SecureString& str){
	//remove old data
	if(length() > 0){
		ssnr oldlen = length();
		memcpy(_data, _key, oldlen);
	} 

	ssnr len = str.length();
	if(len > this->allocated()){
		this->allocate(len*2); //make more room than neccessary, just in case there will be more appends later
	}
	for(ssnr i=0; i<len; i++){
		this->_data[i] = this->_key[i] ^ (str._key[i] ^ str._data[i]);
	}
	_length =  ((ssnr)*_key) ^ len;
}

SecureString::c_ssarr SecureString::getUnsecureString(){
	//there can only be one unsecure plaintext copy at a time
	if(_plaintextcopy != NULL)
		return NULL;
	ssnr size = length();
	_plaintextcopy = new ssbyte[size+1];
	_plaintextcopy[size] = '\0';
	for(ssnr i=0; i < size; i++){
		_plaintextcopy[i] = _key[i] ^ _data[i];
	}
	_mutableplaintextcopy = false;
	return _plaintextcopy;
}

SecureString::ssarr SecureString::getUnsecureStringM(){
	ssarr ret = (ssarr)getUnsecureString();
	_mutableplaintextcopy = true;
	return ret;
}

SecureString::c_ssarr SecureString::getUnsecureNextline(){
	//there can only be one unsecure plaintext copy at a time
	if(_plaintextcopy != NULL)
		return NULL;

	int startPos = (((ssnr)*_key) ^ _nexlinefeedposition);
	int tLen = ((ssnr)*_key) ^ _length;
	int sLen = 0;
	int CRLF = 0;

	//Find next linefeed
	for(int i=startPos; i < tLen; i++){
		if(at(i) == '\n'){
			sLen = i - startPos;
			break;
		} else if(at(i) == '\r'){
			sLen = i - startPos;
			if(at(i+1) == '\n'){
				CRLF = 1;
			}
			break;
		}
	}

	//create new buffert
	ssarr line = new ssbyte[sLen+1];

	//copy text over to the unsecured buffer
	for(int i=0; i<sLen; i++){
		line[i] = _key[startPos+i] ^ _data[startPos+i];
	}
	line[sLen] = '\0';

	_nexlinefeedposition = ((ssnr)*_key) ^ (startPos+sLen+1+CRLF);

	//line is the plaintextcopy
	_plaintextcopy = line;
	_mutableplaintextcopy = false;
	return _plaintextcopy;
}

void SecureString::UnsecuredStringFinished(){
	if(_plaintextcopy == NULL)
		return;
	if(_mutableplaintextcopy){
		assign(_plaintextcopy);
	} else {
		memset(_plaintextcopy, 0, strlen(_plaintextcopy));
		delete[] _plaintextcopy;
	}
	_plaintextcopy = NULL;
}

bool SecureString::equals(SecureString& s2){
	if(s2.length() != this->length()){
		return false;
	}
	int len = this->length();
	for(int i=0; i<len; i++){
		if(s2.at(i) != this->at(i)){
			return false;
		}
	}
	return true;
}

bool SecureString::equals(char* s2){
	if(strlen(s2) != this->length()){
		return false;
	}
	int len = this->length();
	for(int i=0; i<len; i++){
		if(s2[i] != this->at(i)){
			return false;
		}
	}
	return true;
}