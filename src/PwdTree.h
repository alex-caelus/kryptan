#ifndef PWDTREE_H
#define PWDTREE_H

#include <list>
#include "Pwd.h"

class PwdTree{
	private:
	/* VARIBLES */
		typedef std::list<Pwd*> pwlist;
		typedef std::list<PwdTree*> pwtreelist;
		pwlist passwords;
		pwtreelist subtrees;
		PwdTree* parent;

		SecureString* name;
	public:
	/* VARABLES */
	private:
	/* METHODS */
	public:
	/* METHODS */
		PwdTree(PwdTree* parent);
		~PwdTree();

		PwdTree* getParent();

		SecureString* getName();
		void setName(SecureString* name);

		void addPassword(Pwd* pass);
		void addSubTree(PwdTree* tree);

		PwdTree* getTreeOnScreen(const char* caption, bool allowNewCategories, const char* textChooseThis);
		Pwd* getPasswordOnScreen(const char* caption);

		bool removePassword(Pwd* pass);
		bool removeSubTree(PwdTree* tree);

		void sortEntireSubTree();

		void writeTreeSecureString(SecureString* string);

		bool empty(){
			return passwords.size() == 0 && subtrees.size() == 0;
		}
};

#endif