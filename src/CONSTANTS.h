#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define VERSION_NUMBER "1.1.1"
#define PASSWORD_FILE "secret.pwd"
#define MAX_FAILED_ATTEMPTS 3
#define MAXLISTHEIGHT 8
#define RUBBEREFFEKT 3
#define ANTI_KEYLOGGING_ARGUMENT "--use-anti-keylogging"
#define FILESTART "__SECRET-PASSWORD-FILE__"
#define FILEEND "__END-FILE__"
#define PASSWORDSTART "__PASSWORD-ENTRY__"
#define EMPTYLINE "__EMPTY__"
#define NEW_FILE_TMP_CONTENT "This is a new encrypted file with no internal file-structure yet."

#define PASSWORD_CHARACTERS_ALNUM "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
#define PASSWORD_CHARACTERS_SPECIALS "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
#define PASSWORD_CHARACTERS_ALL PASSWORD_CHARACTERS_ALNUM PASSWORD_CHARACTERS_SPECIALS

#define VERSION "Version " VERSION_NUMBER
#define CREATEDBY "Program created by: Alexander Nilsson. All rights reserved."

#define NO_PASSWORDS "No passwords in the password file!"
#define CAPTION_NEW_PWD_FILE "New Password File"
#define CREATE_NEW_PWD_FILE_Y_N "Create new password file (y/n)? "
#define NEW_MASTER_PWD "New MASTER key: "
#define CONFIRM_MASTER_PWD "Confirm MASTER: "
#define CAPTION_DECRYPT "Decrypting File"
#define PROMT_PWD "Password: "
#define MSG_DECRYPTING "Decrypting..."
#define CAPTION_FILE_STRUCTURE "File Structure"
#define CREATE_FILESTRUCTURE_Y_N "Create filestructure (y/n)? "
#define CREATE_FILESTRUCTURE_CONFIRM_Y_N "Warning: This will erase the content of the file (if any), are you sure you want to continue (y/n)? "
#define WELCOME_MSG "Password file successfully read!"
#define CAPTION_WELCOME "Welcome"
#define CAPTION_SHOW_PWD "Show password"
#define CAPTION_ADD_PWD "New Password"
#define ADD_PWD_DESC "Short description: "
#define ADD_PWD_USERNAME "Username (can be empty): "
#define ADD_PWD_PWD "New password: "
#define ADD_PWD_CONFIRM "Confirm: "
#define CAPTION_EDIT_PWD "Edit password"
#define CAPTION_EDIT_SHORT "Edit "
#define CAPTION_REMOVE_PWD "Remove Password"
#define CAPTION_REMOVE_PWD_SHORT "Remove "
#define DELETE_PWD_Y_N "Are you sure you want to delete this password (y/n) ? "
#define USE_SPECIALS_Y_N "Use special characters? (y/n)"
#define CAPTION_GENERATE_PWD "Generate Password"
#define MIN_LENGTH_Y_N "Minimum length?"
#define MAX_LENGTH_Y_N "Maximum length?"
#define GEN_PWD_RESULT "New password is:"
#define GEN_PWD_SAVE_Y_N "Save to password list (y/n) ?"
#define CAPTION_EDIT_MASTER "New MASTER Key"
#define CAPTION_HELP "Help"

#define HELPTEXT  {                                                     \
    "Kryptan " VERSION,                                                  \
    "This program is created with crypto++ 5.6.1 and using:",           \
    "\"Password-Based Encryptor with DES-EDE2 and HMAC/SHA-1\"",        \
    "Contact Alexander Nilsson for further questions at:",              \
    "alex.caelus@gmail.com OR www.caelus.org/proj/kryptan"              \
}                                                                       \


enum MENU_CHOICES {
    MENU_NULL,
    MENU_SHOW_PWD,
    MENU_ADD_PWD,
    MENU_GENERATE_PWD,
    MENU_EDIT_PWD,
    MENU_REMOVE_PWD,
    MENU_CHANGE_MASTER,
    MENU_SHOW_HELP,
    MENU_QUIT,
    MENU_GO_TO_MAIN
};
#define MAKE_YOUR_SELECTION "Make your selection:  \b"
#define MENUTITLE "Main Menu"
#define MENUSTRING_SHOW_PWD		"Get password."
#define MENUSTRING_ADD_PWD		"Add a custom password."
#define MENUSTRING_GENERATE_PWD		"Generate a new random password."
#define MENUSTRING_EDIT_PWD		"Change an existing password."
#define MENUSTRING_REMOVE_PWD		"Remove a password."
#define MENUSTRING_CHANGE_MASTER	"Change the MASTER key."
#define MENUSTRING_SHOW_HELP		"Show help text."
#define MENUSTRING_QUIT			"Quit."
#define MENUSTRING_GO_TO_MAIN		"Go to main menu."
#define MENU_SEPPARATOR			"----------------"


#define WARNING_ERASE_CONTENT "Warning: This will erase the contents of the current file, are you sure you want to continue (y/n)? "

#define ERROR_CAPTION "ERROR"
#define NO_ERROR "No error reported!"
#define ERROR_WRITE_FILE "Could not write to file!"
#define ERROR_READ_FILE1 "File \""
#define ERROR_READFILE2 "\" not found or is not readable!"
#define ERROR_MASTER_KEY "Invalid key!"
#define ERROR_MAC_VALIDATION "MAC validation failed!"
#define ERROR_KEY_CONFIRM "Keys do not match, please try again!"
#define ERROR_PWD_CONFIRM "Passwords do not match, please try again!"
#define ERROR_NO_PWD_FILE "No usable password file, application exits!"
#define ERROR_PWD_FILE_CORRUPT "Password file corruption detected, the file may have been accidentally modified or tampered with!"
#define ERROR_MAX_FAILED_DECRYPTS "Max number of failed attempts!"
#define ERROR_SINGELTON "There can only be one instance of this class running a a time!"
#define ERROR_FILE_STRUCTURE "Structure of passwordfile is incorrect or missing!"
#define ERROR_FILE_STRUCTURE_CORRUPT "Password file corrupt! Erroneous password delimiter. Passwords recovered: "
#define ERROR_FILE_STRUCTURE_EOF "Password file corrupt! Unexpected end of file. Passwords recovered: "

#endif