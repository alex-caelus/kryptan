#include "PwdTree.h"
#include "Exceptions.h"

#include <ctype.h>
#include <fstream>
#include <string.h>

#ifndef _WIN32
//We make sprintf_s portable to the linux platform
#define sprintf_s snprintf
#endif

PwdTree::PwdTree(PwdTree* parent){
	this->parent = parent;
	this->name = NULL;
}

PwdTree::~PwdTree(){
	delete name;
	//do not delete parent
}

PwdTree* PwdTree::getParent(){
	return parent;
}

SecureString* PwdTree::getName(){
	return name;
}

void PwdTree::setName(SecureString* name){
	delete this->name;
	this->name = name;
}

void PwdTree::addPassword(Pwd* pass){
	passwords.push_back(pass);
}

void PwdTree::addSubTree(PwdTree* tree){
	subtrees.push_back(tree);
}

PwdTree* PwdTree::getTreeOnScreen(const char* caption, bool allowNewCategories, const char* textChooseThis){
	UiElementList list;
	SecureString* theCaption = new SecureString(caption);
	if(parent != NULL){
		theCaption->append(" - ");
		theCaption->append(*getName());
	}

	//list counter
	int i=1;
	int buffwidth = numDigits( 2 + subtrees.size() ) + 2;

	//list choices
	int UP_ONE_LEVEL = -1;
	int NEW_CATEGORY = -1;
	int PASSWORD_HERE;

	//add first row
	if(parent){
		int len = buffwidth+strlen(MENUSTRING_GO_UP_TREE) + 1;
		char* buff = new char[len];
		sprintf_s(buff, len, "%i. %s", i, MENUSTRING_GO_UP_TREE);
		list.push_back(UiElement(buff, true, A_BOLD, 2+numDigits(i))); //delete when done
		UP_ONE_LEVEL = i++;
	}

	//add option to create a new category
	if(allowNewCategories){
		int len = buffwidth+strlen(CREATE_NEW_CATEGORY) + 1;
		char* buff = new char[len];
		sprintf_s(buff, len, "%i. %s", i, CREATE_NEW_CATEGORY);
		list.push_back(UiElement(buff, true, A_BOLD, 2+numDigits(i))); //delete when done
		NEW_CATEGORY = i++;
	}

	//add option to choose this category
	{
		int len = buffwidth+strlen(textChooseThis) + 1;
		char* buff = new char[len];
		sprintf_s(buff, len, "%i. %s", i, textChooseThis);
		list.push_back(UiElement(buff, true, A_BOLD, 2+numDigits(i))); //delete when done
		PASSWORD_HERE = i++;
	}

	//Add divisor element
	list.push_back(UiElement(MENU_SEPPARATOR, A_NORMAL));


	{
		//add all sub categories to the list
		pwtreelist::iterator it = subtrees.begin();
		for(; it != subtrees.end(); it++, i++){
			int len = buffwidth+1;
			char* buff = new char[len];
			sprintf_s(buff, len, "%i. ", i);
			SecureString* line = new SecureString(buff);
			line->append(*(*it)->getName());
			list.push_back(UiElement(line, true, A_BOLD | A_UNDERLINE, 2+numDigits(i))); //delete it when done
		}
	}

	//display the list
	int choice = Ui::getInstance()->PromtList(list, UiElement(theCaption, true));

	//validate the choice
	if(choice > 0 && choice <= i){
		//parse the choice
		if(choice == UP_ONE_LEVEL){
			if(parent){
				//go up one step in the tree
				return parent->getTreeOnScreen(caption, allowNewCategories, textChooseThis);
			}
		} else if (choice == NEW_CATEGORY){
			PwdTree* new_t = new PwdTree(this);
			addSubTree(new_t);
			SecureString* name = Ui::getInstance()->PromtString(UiElement(NEW_CATEGORY_QUESTION), UiElement(NEW_CATEGORY_CAPTION));
			new_t->setName(name);
			return new_t;
		} else if(choice == PASSWORD_HERE){
			return this;
		} else{
			pwtreelist::iterator ch = subtrees.begin();
			int limit = choice-(i - subtrees.size());
			for(int i=0; i < limit; i++){
				ch++;
			}
			return (*ch)->getTreeOnScreen(caption, allowNewCategories, textChooseThis);
		}
	}

	return getTreeOnScreen(caption, allowNewCategories, textChooseThis); //if response is not valid, try again
}

Pwd* PwdTree::getPasswordOnScreen(const char* caption){
	UiElementList list;
	SecureString* theCaption = new SecureString(caption);
	if(parent != NULL){
		theCaption->append(" - ");
		theCaption->append(*getName());
	}

	//list counter
	int i=1;
	int buffwidth = numDigits( 1 + subtrees.size() + passwords.size()) + 2;
	int UP_ONE_LEVEL=-1, GO_TO_MAIN=-1;

	//add first row
	if(parent){
		int len = buffwidth+strlen(MENUSTRING_GO_UP_TREE) + 1;
		char* buff = new char[len];
		sprintf_s(buff, len, "%i. %s", i, MENUSTRING_GO_UP_TREE);
		list.push_back(UiElement(buff, true, A_BOLD, 2+numDigits(i))); //delete when done
		UP_ONE_LEVEL = i++;
	} else {
		int len = buffwidth+strlen(MENUSTRING_GO_TO_MAIN) + 1;
		char* buff = new char[len];
		sprintf_s(buff, len, "%i. %s", i, MENUSTRING_GO_TO_MAIN);
		list.push_back(UiElement(buff, true, A_BOLD, 2+numDigits(i))); //delete when done
		GO_TO_MAIN = i++;
	}

	//Add divisor element
	list.push_back(UiElement(MENU_SEPPARATOR, A_NORMAL));
	
	{
		//add all sub categories to the list
		pwtreelist::iterator it = subtrees.begin();
		for(; it != subtrees.end(); it++, i++){
			int len = buffwidth + 1;
			char* buff = new char[len];
			sprintf_s(buff, len, "%i. ", i);
			SecureString* line = new SecureString(buff);
			line->append(*(*it)->getName());
			list.push_back(UiElement(line, true, A_BOLD | A_UNDERLINE, 2+numDigits(i))); //delete it when done
		}
	}

	{
		//add all passwords to the list
		pwlist::iterator it = passwords.begin();
		for(; it != passwords.end(); it++, i++){
			int len = buffwidth + 1;
			char* buff = new char[len];
			sprintf_s(buff, len, "%i. ", i);
			SecureString* line = new SecureString(buff);
			line->append(*(*it)->getDescription());
			list.push_back(UiElement(line, true, A_BOLD, 2+numDigits(i)));
		}
	}

	//display the list
	int choice = Ui::getInstance()->PromtList(list, UiElement(theCaption, true));

	//validate the choice
	if(choice > 0 && choice <= i){
		//parse the choice
		if(choice == UP_ONE_LEVEL){
				return parent->getPasswordOnScreen(caption);
		} else if(choice == GO_TO_MAIN) {
			return NULL;
		} 
		choice -= (i - passwords.size() - subtrees.size() );
		if((unsigned int)choice < subtrees.size()){
			pwtreelist::iterator ch = subtrees.begin();
			for(int i=0; i < choice; i++){
				ch++;
			}
			return (*ch)->getPasswordOnScreen(caption);
		} else {
			pwlist::iterator ch = passwords.begin();
			int limit = choice - subtrees.size();
			for(int i=0; i < limit; i++){
				ch++;
			}
			return *ch;
		}
	}

	return getPasswordOnScreen(caption);  //if response is not valid, try again
}

bool PwdTree::removePassword(Pwd* pass){
	for(pwlist::iterator it = passwords.begin(); it != passwords.end(); it++){
		if((*it) == pass){
			passwords.erase(it);
			return true;
		}
	}

	for(pwtreelist::iterator it = subtrees.begin(); it != subtrees.end(); it++){
		if((*it)->removePassword(pass)){
			return true;
		}
	}

	return false;
}

bool PwdTree::removeSubTree(PwdTree* tree){
	if(tree->empty()){
		for(pwtreelist::iterator it = subtrees.begin(); it != subtrees.end(); it++){
			if((*it) == tree){
				subtrees.erase(it);
				return true;
			} else if((*it)->removeSubTree(tree)){
				return true;
			}
		}
	}

	return false;
}

bool compareStrings(const char* firstBuff, const char* secondBuff){
	int firstLength = strlen(firstBuff);
	int secondLength = strlen(secondBuff);

	int i=0;
	while ( i < firstLength && i < secondLength )
	{
		if (tolower(firstBuff[i])<tolower(secondBuff[i])) 
			return true;
		else if (tolower(firstBuff[i])>tolower(secondBuff[i])) 
			return false;
		++i;
	}
	if (firstLength < secondLength) 
		return true;
	else 
		return false;
}

bool compareSubtrees(PwdTree* first, PwdTree* second){
	const char* firstBuff = first->getName()->getUnsecureString();
	const char* secondBuff = second->getName()->getUnsecureString();

	bool returnValue = compareStrings(firstBuff, secondBuff);

	first->getName()->UnsecuredStringFinished();
	second->getName()->UnsecuredStringFinished();

	return returnValue;
}

bool comparePasswords(Pwd* first, Pwd* second){
	const char* firstBuff = first->getDescription()->getUnsecureString();
	const char* secondBuff = second->getDescription()->getUnsecureString();

	bool returnValue = compareStrings(firstBuff, secondBuff);

	first->getDescription()->UnsecuredStringFinished();
	second->getDescription()->UnsecuredStringFinished();

	return returnValue;
}

void PwdTree::sortEntireSubTree(){
	//throw UnresolvableException("sortEntireSubTree() not yet implemented!");
	passwords.sort(comparePasswords);
	subtrees.sort(compareSubtrees);
	for(pwtreelist::iterator it = subtrees.begin(); it != subtrees.end(); it++){
		(*it)->sortEntireSubTree();
	}
}

void PwdTree::writeTreeSecureString(SecureString* string){

	if(parent){
		//START SUBTREE
		string->append(TREESTART NEWLINE);
		//Write tree name
		string->append( *getName());
		string->append(NEWLINE);
		getName()->UnsecuredStringFinished();
	}

	//write all sublists
	for(pwtreelist::iterator it = subtrees.begin(); it != subtrees.end(); it++){
		(*it)->writeTreeSecureString(string);
	}

	//write all passwords
	for(pwlist::iterator it = passwords.begin(); it != passwords.end(); it++){
		//START
		string->append(PASSWORDSTART NEWLINE);
		//write description
		string->append( *((*it)->getDescription()));
		string->append(NEWLINE);
		//write password
		string->append( *((*it)->getPassword()));
		string->append(NEWLINE);
		//write username (if it exists)
		if((*it)->getUsername()){
			string->append( *((*it)->getUsername()));
			string->append(NEWLINE);
		}
		//END
		string->append(PASSWORDEND NEWLINE);
	}
	
	if(parent){
		//END SUBTREE
		string->append(TREEEND NEWLINE);
	}
}