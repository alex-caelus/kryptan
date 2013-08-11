#include "Program.h"

#include "common.h"
#include "../kryptan_core/Exceptions.h"
#include "Prompts.h"
#include "MessageBoxes.h"
#include "Utilities.h"

using namespace Kryptan;
using namespace Core;
using namespace std;

Program::Program(bool useAntiKeylogging)
{
    //initialize screen
    initscr();
    cbreak();
    //noecho();
    keypad(stdscr, TRUE);
    refresh();
}

Program::~Program() 
{ 
    endwin();
}

int Program::run()
{
    try
    {
        file = GetFileObject();
        bool exit;
        do
        {
            exit = getch() == 'q';
        }
        while(!exit);
        return 0;
    }
    catch(KryptanQuit &q)
    {
        //do nothing, just quit
        return q.ExitCode();
    }
}

PwdFile* Program::GetFileObject()
{
    file = new PwdFile("secret.pwd");
    if(file->Exists())
    {
        OpenFile(file);
    }
    else
    {
        CreateFile(file);
    }
    return file;
}

void Program::OpenFile(Core::PwdFile* file)
{
    bool done = false;
    int nrOfTries = 0;
    do
    {
        masterkey = PromptPass("Open file", "Please input master key.").Prompt();

        if(masterkey.length() > 0)
        {
            try
            {
                nrOfTries++;
                file->OpenAndParse(masterkey);
                done = true;
            }
            catch(KryptanDecryptWrongKeyException)
            {
                if(nrOfTries < 3)
                {
                    InfoBox("Error", "Wrong masterkey, try again!").Show();
                }
                else
                {
                    InfoBox("Error", "Max number of attempts exceeded, exiting").Show();
                    throw KryptanQuit(1000);
                }
            }
            catch(KryptanFileNotReadableException &e)
            {
                InfoBox("Error", "Error occurred while reading " + (file->GetFilename() + ".\nThe error was:\n") + e.what()).Show();
                throw KryptanQuit(1001);
            }
            catch(KryptanDecryptException &e)
            {
                InfoBox("Error", "Error occurred while decrypting " + (file->GetFilename() + ", reason:\n") + e.what()).Show();
                throw KryptanQuit(1002);
            }
            catch(KryptanFileContentException &e)
            {
                InfoBox("Error", "The contents of the file is corrupt.");
                throw KryptanQuit(1003);
            }
            catch(std::exception &e)
            {
                const char* P = e.what();
                InfoBox("Error", "Unexpected error occurred while\nreading/parsing the password file.\nThe error was:\n" + std::string(e.what())).Show();
                throw KryptanQuit(-1);
            }
        }
    }
    while(!done);
}

void Program::CreateFile(Core::PwdFile* file)
{
    bool create = ConfirmBox("No password file found", "A new password file will now be created.").Confirm();
    if(!create)
    {
        InfoBox("No password file found", "Kryptan will now exit").Show();
        throw KryptanQuit(2000);
    }
    else
    {
        bool done = false;
        do
        {
            masterkey = PromptPass("Create file", "Please input new master key.").Prompt();
            SecureString confirm = PromptPass("Create file", "Please confirm new master key.").Prompt();
            if(!masterkey.equals(confirm))
            {
                InfoBox("Error", "The keys did not match! Please try again").Show();
            }
            else
            {
                try{
                    file->CreateNew();
                    file->Save(masterkey);
                    done = true;
                }
                catch(KryptanFileNotWritableException &e)
                {
                    InfoBox("Error", string("An error occurred while writing new password file.\nThe error was:\n") + e.what()).Show();
                    InfoBox("Error", "Please make sure you have sufficient permission\nto write to the program directory.\nKryptan will now exit").Show();
                    throw KryptanQuit(2001);
                }
            }
        }
        while(!done);
    }
}