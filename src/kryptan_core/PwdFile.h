#ifndef PWDFILE_H
#define PWDFILE_H

#include <string>
#include "SecureString.h"
#include "PwdList.h"

namespace Kryptan{
	namespace Core{

		class PwdFile
		{
		public:
			PwdFile(std::string filename);
			~PwdFile(void);

			void Open(SecureString masterkey);
			void Save(SecureString masterkey);

			PwdList* GetPasswordList();

		private:
			PwdFile(const PwdFile& obj);
		};

	}
}

#endif