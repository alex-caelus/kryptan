#ifndef PROGRAM_H
#define PROGRAM_H

#include "../kryptan_core/core.h"

namespace Kryptan
{
	
	class Program
	{

	public:
		Program(bool useAntiKeylogging);
		~Program();

		int run();

	private:
		Core::PwdFile* file;
		Core::SecureString masterkey;

		Core::PwdFile* GetFileObject();
		void OpenFile(Core::PwdFile* file);
		void CreateFile(Core::PwdFile* file);
	};

}

#endif