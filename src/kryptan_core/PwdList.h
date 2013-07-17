#ifndef PWDLIST_H
#define PWDLIST_H

#include <string>
#include <vector>
#include "Pwd.h"

namespace Kryptan {
    namespace Core {

        class PwdList {
            //this clas can only be created by PwdFile
            friend class PwdFile;
        public:

			typedef std::vector<Pwd> Pwdvector;

            Pwdvector All();
            Pwdvector Filter(std::string pattern, std::vector<std::string> labels = std::vector<std::string>());
			
			int CreatePassword(SecureString desciption, SecureString password);
			int CreatePassword(SecureString desciption, SecureString username, SecureString password);
			int CreatePassword(int id, SecureString desciption, SecureString username, SecureString password);
			Pwd* GetPwd(int);
			void DeletePwd(Pwd* pwd);

        private:
            PwdList(void);
            ~PwdList(void);
            PwdList(const PwdList& obj);
        };

    }
}

#endif