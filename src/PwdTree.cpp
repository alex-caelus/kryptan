#include "PwdTree.h"
#include "Exceptions.h"

#include <ctype.h>
#include <fstream>
#include <string.h>


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

PwdTree* PwdTree::getTreeOnScreen(char* caption, bool allowNewCategories, char* textChooseThis){
	UiElementList list;
	SecureString* theCaption = new SecureString(caption, 0, false);
	if(parent != NULL){
		int len = strlen(caption);
		theCaption->append(" - ", 0, false);
		theCaption->append(*getName());
	}

	//list counter
	int i=1;
	int buffwidth = numDigits( 2 + subtrees.size() ) + 3;

	//list choices
	int UP_ONE_LEVEL = -1;
	int NEW_CATEGORY = -1;
	int PASSWORD_HERE;

	//add first row
	if(parent){
		char* buff = new char[buffwidth+strlen(MENUSTRING_GO_UP_TREE)];
		sprintf(buff, "%i. %s", i, MENUSTRING_GO_UP_TREE);
		list.push_back(UiElement(buff, A_BOLD, 2+numDigits(i), true)); //delete when done
		UP_ONE_LEVEL = i++;
	}

	//add option to create a new category
	if(allowNewCategories){
		char* buff = new char[buffwidth+strlen(CREATE_NEW_CATEGORY)];
		sprintf(buff, "%i. %s", i, CREATE_NEW_CATEGORY);
		list.push_back(UiElement(buff, A_BOLD, 2+numDigits(i), true)); //delete when done
		NEW_CATEGORY = i++;
	}

	//add option to choos this category
	{
		char* buff = new char[buffwidth+strlen(textChooseThis)];
		sprintf(buff, "%i. %s", i, textChooseThis);
		list.push_back(UiElement(buff, A_BOLD, 2+numDigits(i), true)); //delete when done
		PASSWORD_HERE = i++;
	}

	//Add divisor element
	list.push_back(UiElement(MENU_SEPPARATOR, A_NORMAL));


	{
		//add all sub categories to the list
		pwtreelist::iterator it = subtrees.begin();
		for(; it != subtrees.end(); it++, i++){
			char* buff = new char[buffwidth];
			sprintf(buff, "%i. ", i);
			SecureString* line = new SecureString(buff);
			line->append(*(*it)->getName());
			list.push_back(UiElement(line, A_BOLD | A_UNDERLINE, 2+numDigits(i), true)); //delete it when done
		}
	}

	//display the list
	unsigned int choice = Ui::getInstance()->PromtList(list, UiElement(theCaption));

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
			for(unsigned int i=0; i < limit; i++){
				ch++;
			}
			return (*ch)->getTreeOnScreen(caption, allowNewCategories, textChooseThis);
		}
	}

	return getTreeOnScreen(caption, allowNewCategories, textChooseThis); //if response is not valid, try again
}

Pwd* PwdTree::getPasswordOnScreen(char* caption){
	UiElementList list;
	SecureString* theCaption = new SecureString(caption, 0, false);
	if(parent != NULL){
		int len = strlen(caption);
		theCaption->append(" - ", 0, false);
		theCaption->append(*getName());
	}

	//list counter
	int i=1;
	int buffwidth = numDigits( 1 + subtrees.size() + passwords.size()) + 3;
	int UP_ONE_LEVEL=-1, GO_TO_MAIN=-1;

	//add first row
	if(parent){
		char* buff = new char[buffwidth+strlen(MENUSTRING_GO_UP_TREE)];
		sprintf(buff, "%i. %s", i, MENUSTRING_GO_UP_TREE);
		list.push_back(UiElement(buff, A_BOLD, 2+numDigits(i), true)); //delete when done
		UP_ONE_LEVEL = i++;
	} else {
		char* buff = new char[buffwidth+strlen(MENUSTRING_GO_TO_MAIN)];
		sprintf(buff, "%i. %s", i, MENUSTRING_GO_TO_MAIN);
		list.push_back(UiElement(buff, A_BOLD, 2+numDigits(i), true)); //delete when done
		GO_TO_MAIN = i++;
	}

	//Add divisor element
	list.push_back(UiElement(MENU_SEPPARATOR, A_NORMAL));
	
	{
		//add all sub categories to the list
		pwtreelist::iterator it = subtrees.begin();
		for(; it != subtrees.end(); it++, i++){
			char* buff = new char[buffwidth];
			sprintf(buff, "%i. ", i);
			SecureString* line = new SecureString(buff);
			line->append(*(*it)->getName());
			list.push_back(UiElement(line, A_BOLD | A_UNDERLINE, 2+numDigits(i), true)); //delete it when done
		}
	}

	{
		//add all passwords to the list
		pwlist::iterator it = passwords.begin();
		for(; it != passwords.end(); it++, i++){
			char* buff = new char[buffwidth];
			sprintf(buff, "%i. ", i);
			SecureString* line = new SecureString(buff);
			line->append(*(*it)->getDescription());
			list.push_back(UiElement(line, A_BOLD, 2+numDigits(i), true));
		}
	}

	//display the list
	unsigned int choice = Ui::getInstance()->PromtList(list, UiElement(theCaption));

	//validate the choice
	if(choice > 0 && choice <= i){
		//parse the choice
		if(choice == UP_ONE_LEVEL){
				return parent->getPasswordOnScreen(caption);
		} else if(choice == GO_TO_MAIN) {
			return NULL;
		} 
		choice -= (i - passwords.size() - subtrees.size() );
		if(choice < subtrees.size()){
			pwtreelist::iterator ch = subtrees.begin();
			for(unsigned int i=0; i < choice; i++){
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

bool compareStrings(char* firstBuff, char* secondBuff){
	int firstLength = strlen(firstBuff);
	int secondLength = strlen(secondBuff);

	unsigned int i=0;
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
	char* firstBuff = first->getName()->getUnsecureString();
	char* secondBuff = second->getName()->getUnsecureString();

	bool returnValue = compareStrings(firstBuff, secondBuff);

	first->getName()->UnsecuredStringFinished();
	second->getName()->UnsecuredStringFinished();

	return returnValue;
}

bool comparePasswords(Pwd* first, Pwd* second){
	char* firstBuff = first->getDescription()->getUnsecureString();
	char* secondBuff = second->getDescription()->getUnsecureString();

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
		string->append(TREESTART NEWLINE, 0, false);
		//Write tree name
		string->append( *getName());
		string->append(NEWLINE, 0, false);
		getName()->UnsecuredStringFinished();
	}

	//write all sublists
	for(pwtreelist::iterator it = subtrees.begin(); it != subtrees.end(); it++){
		(*it)->writeTreeSecureString(string);
	}

	//write all passwords
	for(pwlist::iterator it = passwords.begin(); it != passwords.end(); it++){
		//START
		string->append(PASSWORDSTART NEWLINE, 0, false);
		//write description
		string->append( *((*it)->getDescription()));
		string->append(NEWLINE, 0, false);
		//write password
		string->append( *((*it)->getPassword()));
		string->append(NEWLINE, 0, false);
		//write username (if it exists)
		if((*it)->getUsername()){
			string->append( *((*it)->getUsername()));
			string->append(NEWLINE, 0, false);
		}
		//END
		string->append(PASSWORDEND NEWLINE, 0, false);
	}
	
	if(parent){
		//END SUBTREE
		string->append(TREEEND NEWLINE, 0, false);
	}
}