#include "Program.h"

#include "common.h"
#include "../kryptan_core/Exceptions.h"
#include "Prompts.h"
#include "MessageBoxes.h"
#include "QrBox.h"
#include "Utilities.h"
#include "MainMenu.h"
#include "PwdMenu.h"
#include "../kryptan_core/Server.h"

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
			case Kryptan::SYNC:
				Syncronize();
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

void Program::PwdDataModified()
{
	InfoBox box("Saving", "Please wait...", false);
	box.Show(false);
	file->Save(masterkey);
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
					InfoBox box("Decrypting", "Please wait...");
					box.Show(false);
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
				catch (KryptanDecryptMacBadException)
				{
					if (nrOfTries < 3)
					{
						InfoBox("Error", "Could not verify file integrity.\n\nPerhaps you've used the wrong masterkey?\n\nOtherwise the file has been corrupted.\n\nTry again!").Show();
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
                catch(KryptanFileVersionException)
                {
                    if(ConfirmBox("Error", "This file uses an old format!\nDo you want to convert the file to a newer format?").Confirm())
                    {
                        try{
                            file->OpenAndParse(masterkey, true);
                            done = true;
                        }
                        catch(std::exception &e)
                        {
                            InfoBox("Error", string("An error occured while converting the password file. The error was:\n") + e.what()).Show();
                            throw KryptanQuit(-1);
                        }
                    }
                    else
                    {
                        throw KryptanQuit(1004);
                    }
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
						PwdDataModified();
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
		bool success = false;
		Pwd* pwd;
		while (!success)
		{

			PromptString p2("Description", "Please input a short description.\nYou may abort password creation with [Esc]", false);
			SecureString description = p2.Prompt();

			try{
				pwd = file->GetPasswordList()->CreatePwd(description, newpass);
				success = true;
			}
			catch (KryptanDuplicatePwdException &e)
			{
				InfoBox i("Error", e.what(), false);
				i.Show();
			}
		}

        SecureString username;
        try{
            PromptString p2("Description", "Please input a username, this is optional.", false);
            username = p2.Prompt();
        }catch(PromtAbortException){};

		pwd->SetUsername(username);

		PwdDataModified();
        OpenPwd(pwd, true );
    }
    catch(PromtAbortException){};
}

void Program::OpenPwd(Core::Pwd* pwd, bool editmode)
{
    PwdMenu p(file, pwd, this);
    p.Display(editmode);
}

void Program::ChangeMasterkey()
{
    try{
        bool done = false;
        if(!PromptPass("New Masterkey", "Please input your OLD masterkey as confirmation.", false).Prompt().equals(masterkey))
        {
            InfoBox("Error", "The key was incorrect, please try again!", false).Show();
            return;
        }
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
					InfoBox box("Saving", "Please wait...", false);
					box.Show(false);
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
    Message += "Please visit http://www.caelus.org/proj/kryptan/ for support and further information.";

    InfoBox("About", Message).Show();
}

void Program::Syncronize()
{
	Server* server;
	InfoBox* box;
	try{
		//first let the user confirm the action confirm action
		if (!ConfirmBox("Confirm syncronization",
			"To syncronize your password file with a smartphone read this _before_\ncontinuing:\n\n"
			"1. Open and log in to Krypan on your smartphone.\n"
			"2. Click the syncronize menu option on your smartphone.\n"
			"3. Scan the QR code that will appear here when you proceed.\n"
			"4. Once a successfull connection is established the QR code will dissapear.\n"
			"5. Press confirm on your smartphone by pressing apply.\n"
			"6. You are done!", false).Confirm())
		{
			return;
		}

		box = new InfoBox("Generating key", "Please wait...", false);
		box->Show(false);

		//port
		int port = 4321;

		//generate one time transmission key
		SecureString tmpTransmissionKey = PromtOrGeneratePass::GeneratePassword(30, false);
		string content = file->SaveToString(tmpTransmissionKey, 100); //lower mash iterations to 100 for sync with low powered devices
		server = Server::CreateServer(port, content);

		//serve current content
		server->StartAsyncServe();

		//generate QR code
		delete box;
		box = 0;
		QRIpPortKeyBox qrbox("Waiting for conection...");
		qrbox.Show(port, tmpTransmissionKey);

//		box = new InfoBox("Syncronization", qrMessage, false);
//		box->Show(false);

		//set timer for getch
		timeout(100);//ms

		//status message
		string stsMsg = "";
		string lastMessage = "";
		string title = "Syncronization status";

		bool contd = true;
		while (contd){
			Server::Status status = server->GetStatus();

			switch (status)
			{
			case Server::WAITING_FOR_START:
			case Server::WAITING_FOR_CONNECTION:
				stsMsg = ""; //continue to show the QR code
				break;
			case Server::SENDING_CONTENT:
				stsMsg = "You can quit at any time by pressing the ESC button.\n\nWaiting for connection... OK!\n\nSending data...";
				break;
			case Server::WAITING_FOR_CONTENT:
				stsMsg = "You can quit at any time by pressing the ESC button.\n\nWaiting for connection... OK!\n\nSending data... OK!\n\nWaiting for other device to confirm changes...";
				break;
			case Server::FINISHED:
				stsMsg = "You can quit at any time by pressing the ESC button.\n\nWaiting for connection... OK!\n\nSending data... OK!\n\nWaiting for other device to confirm changes... OK!\n\nAll done!\n\nPress any key to continue...";
				file->ReplaceContent(tmpTransmissionKey, server->getRecievedContent());
				server->AbortAsyncServe();
				this->PwdDataModified();
				timeout(-1);
				contd = false;
				break;
			case Server::ABORTING:
				stsMsg = "Waiting for server to shut down";
				break;
			case Server::ABORTED:
				stsMsg = "Syncronization canceled!\n\nPress any key to continue...";
				timeout(-1);
				contd = false;
				break;
			case Server::SERVER_ERROR:
				stsMsg = "Error: " + server->GetErrorMessage() + "\n\nPress any key to continue.";
				timeout(-1);
				contd = false;
				break;
			}

			if (!stsMsg.empty() && stsMsg != lastMessage)
			{
				qrbox.Hide();
				delete box;
				box = new InfoBox(title, stsMsg, false);
				box->Show(false);
				lastMessage = stsMsg;
			}

			//check for input
			int c = getch();
			if (c == 27) //ESC
			{
				contd = false;
			}
		}

		timeout(-1);
		delete server;
		delete box;
	}
	catch (std::exception &ex)
	{
		timeout(-1);
		delete server;
		delete box;
		InfoBox e("Error", string("An exception occured, the error was:\n\n") + ex.what() + "\n\nPress any key to continue...", false);
		e.Show();
	}
}