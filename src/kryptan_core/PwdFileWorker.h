#ifndef PWDFILEWORKER_H
#define PWDFILEWORKER_H

#include <string>
#include "SecureString.h"
#include "PwdList.h"
#include "Exceptions.h"

#define RootTagStart "<passwords>"
#define RootTagEnd "</passwords>"
#define PwdTagStart "<pwd>"
#define PwdTagEnd "</pwd>"
#define DescriptionTagStart "<description>"
#define DescriptionTagEnd "</description>"
#define UsernameTagStart "<username>"
#define UsernameTagEnd "</username>"
#define PasswordTagStart "<password>"
#define PasswordTagEnd "</password>"
#define LabelTagStart "<label>"
#define LabelTagEnd "</label>"

namespace Kryptan {
    namespace Core {

        class PwdFileWorker {
        public:
			static void ReadFile(std::string filename, int& length, char*& buffer);
			static void WriteFile(std::string filename, char* content, int length);
			static bool FileExists(std::string filename);
			static PwdList* ParseFileContents(SecureString content);
			static char* GetNextTagStart(const char* pos);
			static int GetTagLength(const char* tagStart);
			static SecureString EscapeTags(const SecureString& str);
			static SecureString UnescapeTags(const char* str, int length);
			static char* Encrypt(SecureString content, int& encryptedLength, SecureString masterkey);
			static SecureString Decrypt(char* encryptedBuffer, int encryptedBufferLength, SecureString masterkey);
			static void DeletePwdList(PwdList* list);
        private:
        };

    }
}

#endif