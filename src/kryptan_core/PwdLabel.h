#ifndef PWDLABEL_H
#define PWDLABEL_H

#include <string>
#include "SecureString.h"
namespace Kryptan{
	namespace Core{

		class PwdLabel
		{
		public:

		private:
			PwdLabel(void);
			~PwdLabel(void);
			PwdLabel(const PwdLabel& obj);
		};

	}
}

#endif