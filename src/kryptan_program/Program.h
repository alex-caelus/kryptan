#ifndef PROGRAM_H
#define PROGRAM_H

#include "../kryptan_core/core.h"
#include "common.h"

namespace Kryptan
{
    
	class Program : PwdDataModificationObserver
    {

    public:
        Program(bool useAntiKeylogging);
        ~Program();

        int run();

		void PwdDataModified() override;

    private:
        Core::PwdFile* file;
        Core::SecureString masterkey;

        Core::PwdFile* GetFileObject();
        void OpenFile(Core::PwdFile* file);
        void CreateFile(Core::PwdFile* file);
        void NewPwd();
        void OpenPwd(Core::Pwd* pwd, bool editmode=false);
        void ChangeMasterkey();
        void About();

    };

}

#endif