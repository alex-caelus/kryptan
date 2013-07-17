#ifndef PWDLABEL_H
#define PWDLABEL_H

#include <string>
#include "SecureString.h"
namespace Kryptan{
	namespace Core{

		class PwdLabelKey{
			friend PwdLabel;
		private:
			PwdLabelKey();
		};

		class PwdLabel
		{
			friend class Pwd;
		public:
			SecureString GetName();
		private:
			PwdLabel(PwdLabelKey const);
			~PwdLabel();
			PwdLabel(const PwdLabel&);

			std::string mName;

            static PwdLabel* CreateLabel(std::string labelname);
            static void DestroyLabel(PwdLabel* label);
		};

	}
}

#endif