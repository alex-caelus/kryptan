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

		PwdTree* getTreeOnScreen();
		Pwd* getPasswordOnScreen();

		void removePassword(Pwd* pass);
		void removeSubTree(PwdTree* tree);

		void sortEntireSubTree();
		void sortPasswords();
};

#endif