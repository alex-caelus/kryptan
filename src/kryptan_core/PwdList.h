#ifndef PWDLIST_H
#define PWDLIST_H

#include <string>
#include <list>
#include "Pwd.h"
#include "SecureString.h"

namespace Kryptan {
    namespace Core {

        class PwdList {
            //this clas can only be created by PwdFile
            friend class PwdFileWorker;
        public:

            typedef std::vector<Pwd*> PwdVector;

            PwdVector All();
            PwdVector Filter(const SecureString& pattern);
            PwdVector Filter(const PwdLabelVector& labels);
            PwdVector Filter(const SecureString& pattern, const PwdLabelVector& labels);

            Pwd* CreatePwd(const SecureString& desciption, const SecureString& password);
            Pwd* CreatePwd(const SecureString& desciption, const SecureString& username, const SecureString& password);
            void DeletePwd(Pwd* pwd);
            
            PwdLabelVector AllLabels();
            PwdLabelVector FilterLabels(SecureString pattern);
            int CountPwds();
            int CountPwds(const SecureString& label);
            
            bool AddPwdToLabel(Pwd* pwd, SecureString label);
            bool RemovePwdFromLabel(Pwd* pwd, SecureString label);

        private:
            PwdList(void);
            ~PwdList(void);
            PwdList(const PwdList& obj);

            std::list<Pwd*> pwds;
            std::list<SecureString> existingLabels;

        };
    }
}

#endif