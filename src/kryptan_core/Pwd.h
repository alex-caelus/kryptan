/* 
 * File:   Pwd.h
 * Author: alexander
 *
 * Created on July 17, 2013, 5:48 PM
 */

#ifndef PWD_H
#define	PWD_H

#include "PwdLabel.h"

namespace Kryptan {
    namespace Core {

        class Pwd {
            //this class can only be instansiated by the PwdList class
            friend class PwdList;
        public:
			SecureString GetDescription();
			SecureString GetUsername();
			SecureString GetPassword();
        private:
            Pwd();
            Pwd(const Pwd& orig);
            virtual ~Pwd();
        };
    }
}

#endif	/* PWD_H */

