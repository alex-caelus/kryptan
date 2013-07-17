/* 
 * File:   Pwd.cpp
 * Author: alexander
 * 
 * Created on July 17, 2013, 5:48 PM
 */

#include "Pwd.h"

using namespace Kryptan::Core;

Pwd::Pwd() {
	PwdLabel* label = new PwdLabel(PwdLabelKey());
}

Pwd::Pwd(const Pwd& orig) {
}

Pwd::~Pwd() {
}

