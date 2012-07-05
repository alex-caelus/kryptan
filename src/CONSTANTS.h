#ifndef CONSTANTS_H
#define	CONSTANTS_H

#define VERSION_NUMBER "2.0"
#define PASSWORD_FILE_VERSION_NUMBER "2.0"
#define PASSWORD_FILE "secret.pwd"
#define MAX_FAILED_ATTEMPTS 3
#define MAXLISTHEIGHT 8
#define RUBBEREFFEKT 3
#define ANTI_KEYLOGGING_ARGUMENT "--use-anti-keylogging"
#define TREESTART "__SUB-TREE__"
#define TREEEND "__END-SUB-TREE__"
#define PASSWORDSTART "__PASSWORD-ENTRY__"
#define PASSWORDEND "__END-PASSWORD-ENTRY__"
#define EMPTYLINE "__EMPTY__"
#define NEW_FILE_TMP_CONTENT "This is a new encrypted file with no internal file-structure yet."
#define NEWLINE "\n"

#define PASSWORD_CHARACTERS_ALNUM "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
#define PASSWORD_CHARACTERS_SPECIALS "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
#define PASSWORD_CHARACTERS_ALL PASSWORD_CHARACTERS_ALNUM PASSWORD_CHARACTERS_SPECIALS

#define VERSION "Version " VERSION_NUMBER
#define CREATEDBY "Program created by: Alexander Nilsson. All rights reserved."

#define ROOT_TREE_NAME "All Passwords"
#define NO_PASSWORDS "No passwords in the password file!"
#define CAPTION_NEW_PWD_FILE "New Password File"
#define CREATE_NEW_PWD_FILE_Y_N "Create new password file (y/n)? "
#define NEW_MASTER_PWD "New MASTER key: "
#define CONFIRM_MASTER_PWD "Confirm MASTER: "
#define OLD_MASTER_PWD "Confirm old MASTER key: "
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
#define REMOVE_PWD "You are about to remove the following password:"
#define USE_SPECIALS_Y_N "Use special characters? (y/n)"
#define CAPTION_GENERATE_PWD "Generate Password"
#define GENERATE_PWD_LENGTH "New password length?"
#define GEN_PWD_RESULT "New password is:"
#define GEN_PWD_SAVE_Y_N "Save to password list (y/n) ?"
#define CAPTION_EDIT_MASTER "New MASTER Key"
#define CAPTION_HELP "Help"
#define CREATE_NEW_CATEGORY "Create a new category here."
#define ADD_NEW_PASSWORD_HERE "Put password here."
#define NEW_CATEGORY_QUESTION "New category name:"
#define NEW_CATEGORY_CAPTION "New Category"
#define PWD_WIZARD_CAPTION "Password wizard"
#define CHANGE_DESCRIPTION_QUESTION "Do you want to change the description? (y/n) "
#define GET_DESCRIPTION_QUESTION "Description: "
#define CHANGE_USERNAME_QUESTION "Do you want to change the username? (y/n) "
#define GET_USERNAME_QUESTION "Username: "
#define CHANGE_PASSWORD_QUESTION "Do you want to change the password? (y/n) "
#define MOVE_PASSWORD "Do you want to move this password to another category? (y/n) "
#define CAPTION_REMOVE_TREE "Remove category"
#define REMOVE_TREE_CONFIRM_1 "You are about to remove the following category:"
#define CONFIRM_QUESTION "Are you sure? (y/n) "
#define REMOVE_THIS_CATEGORY "Remove this category."

#define HELPTEXT  {                                                     \
    "Kryptan " VERSION,                                                  \
    "This program is created with crypto++ 5.6.1 and using:",           \
    "\"Password-Based Encryptor with DES-EDE2 and HMAC/SHA-1\"",        \
    "Contact Alexander Nilsson for further questions at:",              \
    "alex.caelus@gmail.com OR www.caelus.org/proj/kryptan",              \
	0																	\
}                                                                       \


enum MENU_CHOICES {
    MENU_NULL,
    MENU_SHOW_PWD,
    MENU_ADD_PWD,
    MENU_GENERATE_PWD,
    MENU_EDIT_PWD,
    MENU_REMOVE_PWD,
    MENU_REMOVE_TREE,
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
#define MENUSTRING_REMOVE_TREE		"Remove a category."
#define MENUSTRING_CHANGE_MASTER	"Change the MASTER key."
#define MENUSTRING_SHOW_HELP		"Show help text."
#define MENUSTRING_QUIT			"Quit."
#define MENUSTRING_GO_TO_MAIN		"Go to main menu."
#define MENUSTRING_GO_UP_TREE	"Go up one level."
#define MENU_SEPPARATOR			"----------------"


#define WARNING_ERASE_CONTENT "Warning: This will erase the contents of the current file, are you sure you want to continue (y/n)? "

#define INFO_CAPTION "INFO"
#define WARNING_CAPTION "WARNING"
#define ERROR_CAPTION "ERROR"
#define ERROR_WRITE_FILE "Could not write to password file!"
#define ERROR_CREATE_FILE "Could not create password file!"
#define ERROR_READ_FILE "Password file not found or is not readable!"
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
#define ERROR_TREE_NOT_EMPTY_ON_REMOVE "The category must be empty, before it can be removed!"
#define ERROR_FILE_VERSION "Password file version mismatch"
#define ERROR_FILE_VERSION_HELP "Visit http://www.caelus.org/proj/kryptan for help"
#define ERROR_ENCRYPTION_CAPTION "ENCRYPTION ERROR"
#define ERROR_DECRYPTION_CAPTION "DECRYPTION ERROR"


/* UTILITY FUNCTION */
// generic solution
template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

#endif