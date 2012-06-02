#include "PwdTree.h"
#include "Exceptions.h"


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

PwdTree* PwdTree::getTreeOnScreen(){
	throw UnresolvableException("getTreeOnScreen() not yet implemented!");
	return NULL;
}

Pwd* PwdTree::getPasswordOnScreen(){
	UiElementList list;

	//add first row
	if(parent){
		list.push_back(UiElement(MENUSTRING_GO_UP_TREE));
	} else {
		list.push_back(UiElement(MENUSTRING_GO_TO_MAIN));
	}

	//Add divisor element
	list.push_back(UiElement(MENU_SEPPARATOR, A_NORMAL));

	//list counter
	int i=1;
	int buffwidth = numDigits( 1 + subtrees.size() + passwords.size()) + 3;

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
	Ui::getInstance()->PromtList(list, UiElement(getName()));

	return NULL;
}

void PwdTree::removePassword(Pwd* pass){
	throw UnresolvableException("removePassword() not yet implemented!");
}

void PwdTree::removeSubTree(PwdTree* tree){
	throw UnresolvableException("removeSubTree() not yet implemented!");
}

void PwdTree::sortEntireSubTree(){
	throw UnresolvableException("sortEntireSubTree() not yet implemented!");
}

void PwdTree::sortPasswords(){
	throw UnresolvableException("sortPasswords() not yet implemented!");
}