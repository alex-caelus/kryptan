#ifndef PWDLIST_H
#define PWDLIST_H

#include <string>
#include "PwdLabel.h"

namespace Kryptan{
	namespace Core{

		class PwdList
		{
			//this clas can only be created by PwdFile and itself
			friend class PwdFile;
		public:
			PwdList* FilterOnName(std::string filter);

		private:
			PwdList(void);
			~PwdList(void);
			PwdList(const PwdList& obj);
		};

	}
}

#endif