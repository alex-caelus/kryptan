#ifndef PROMTS_H
#define PROMTS_H

#include "DialogBase.h"
#include "../kryptan_core/core.h"

namespace Kryptan
{
    
    class PromtAbortException { };

    class PromptPass : DialogBase
    {
    public:
        PromptPass(std::string title, std::string message, bool modal = true);

        Core::SecureString Prompt();
    private:
        std::string message;
    };
    
    class PromptString : DialogBase
    {
    public:
        PromptString(std::string title, std::string message, bool modal = true);

        Core::SecureString Prompt();

    private:
        std::string message;
    };

    class PromtOrGeneratePass : DialogBase
    {
    public:
        PromtOrGeneratePass();

        Core::SecureString Prompt();

		static Core::SecureString GeneratePassword(int length, bool useSpecialChars);
    };
};


#endif