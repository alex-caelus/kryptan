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

        Caelus::Utilities::SecureString Prompt();
    private:
        std::string message;
    };
    
    class PromptString : DialogBase
    {
    public:
        PromptString(std::string title, std::string message, bool modal = true);

        Caelus::Utilities::SecureString Prompt();

    private:
        std::string message;
    };

    class PromtOrGeneratePass : DialogBase
    {
    public:
        PromtOrGeneratePass();

        Caelus::Utilities::SecureString Prompt();

		static Caelus::Utilities::SecureString GeneratePassword(int length, bool useSpecialChars);
    };
};


#endif