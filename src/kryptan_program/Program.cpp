#include "Program.h"

#include "common.h"
#include "../kryptan_core/Exceptions.h"
#include "Prompts.h"
#include "MessageBoxes.h"
#include "Utilities.h"
#include "MainMenu.h"
#include "PwdMenu.h"

using namespace Kryptan;
using namespace Core;
using namespace std;

Program::Program(bool useAntiKeylogging)
{
    //initialize screen
    initscr();
    start_color();
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
        MainMenu main(file);
        MenuActions action;
        do
        {
            action = main.Display();
            switch (action)
            {
            case Kryptan::NEW_PWD:
                NewPwd();
                break;
            case Kryptan::OPEN_PWD:
                OpenPwd(main.GetSelectedPwd());
                break;
            case Kryptan::CHANGE_MASTER:
                ChangeMasterkey();
                break;
            case Kryptan::ABOUT:
                About();
                break;
            case Kryptan::QUIT:
            case Kryptan::NO_ACTION:
            default:
                break;
            }
        }
        while(action != QUIT);
        return 0;
    }
    catch(KryptanQuit &q)
    {
        //do nothing, just quit
        return q.ExitCode();
    }
    catch(std::exception &ex)
    {
        InfoBox e("Error", (string("An exception occured, the error was: \n\"") + ex.what()) + "\"\nKryptan will now exit!");
        e.Show();
        return 1;
    }
    catch(...)
    {
        InfoBox e("Error", "An unknown exception occured!\nKryptan will now exit!");
        e.Show();
        return 1;
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
    try{
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
                catch(KryptanFileContentException)
                {
                    InfoBox("Error", "The contents of the file is corrupt.").Show();
                    throw KryptanQuit(1003);
                }
                catch(std::exception &e)
                {
                    InfoBox("Error", "Unexpected error occurred while\nreading/parsing the password file.\nThe error was:\n" + std::string(e.what())).Show();
                    throw KryptanQuit(-1);
                }
            }
        }
        while(!done);
    }
    catch(PromtAbortException)
    {
        InfoBox("Aborted", "Aborted by user, Kryptan will now exit").Show();
        throw KryptanQuit(0);//zero, all is fine, the user wanted to quit
    }
}

void Program::CreateFile(Core::PwdFile* file)
{
    try
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
                    catch(std::exception &e)
                    {
                        InfoBox("Error", "Unexpected error occurred while\ncreating the new password file.\nThe error was:\n" + std::string(e.what())).Show();
                        throw KryptanQuit(-1);
                    }
                }
            }
            while(!done);
        }
    }
    catch(PromtAbortException)
    {
        InfoBox("Aborted", "Aborted by user, Kryptan will now exit").Show();
        throw KryptanQuit(0);//zero, all is fine, the user wanted to quit
    }
}


void Program::NewPwd()
{
    try{
        PromtOrGeneratePass p;
        SecureString newpass = p.Prompt();
        if(newpass.length() <= 0)
        {
            InfoBox i("New Password", "Password must not be empty!", false);
            i.Show();
            throw PromtAbortException();
        }

        PromptString p2("Description", "Please input a short description.\nYou may abort password creation with [Esc]", false);
        SecureString description = p2.Prompt();

        SecureString username;
        try{
            PromptString p2("Description", "Please input a username, this is optional.", false);
            username = p2.Prompt();
        }catch(PromtAbortException){};

        OpenPwd( file->GetPasswordList()->CreatePwd(description, username, newpass), true );
    }
    catch(PromtAbortException){};
}

void Program::OpenPwd(Core::Pwd* pwd, bool editmode)
{
    PwdMenu p(file->GetPasswordList(), pwd);
    p.Display(editmode);
    file->Save(masterkey);
}

void Program::ChangeMasterkey()
{
    try{
        bool done = false;
        do
        {
            SecureString newmasterkey = PromptPass("New Masterkey", "Please input new master key.\nAbort with [Esc]", false).Prompt();
            SecureString confirm = PromptPass("New Masterkey", "Please confirm new master key.\nAbort with [Esc]", false).Prompt();
            if(!newmasterkey.equals(confirm))
            {
                InfoBox("Error", "The keys did not match! Please try again", false).Show();
            }
            else
            {
                try{
                    file->Save(newmasterkey);
                    done = true;
                    masterkey = newmasterkey;
                }
                catch(KryptanFileNotWritableException &e)
                {
                    InfoBox("Error", string("An error occurred while writing new password file.\nThe error was:\n") + e.what(), false).Show();
                    InfoBox("Error", "Please make sure you have sufficient permission\nto write to the program directory.\nKryptan will now exit", false).Show();
                    throw KryptanQuit(2001);
                }
                catch(std::exception &e)
                {
                    InfoBox("Error", "Unexpected error occurred while\ncreating the new password file.\nThe error was:\n" + std::string(e.what()), false).Show();
                    throw KryptanQuit(-1);
                }
            }
        }
        while(!done);
    }
    catch(PromtAbortException)
    {
    }
}

void Program::About()
{
    string Message = "This program is created by Alexander Nilsson.\n\n";
    Message += "Please visit http://www.caelus.org for support and further information.";

    InfoBox("About", Message).Show();
}