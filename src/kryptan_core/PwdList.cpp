#include "PwdList.h"
#include <exception>

using namespace Kryptan::Core;
using namespace std;

PwdList::PwdList(void)
{
}


PwdList::~PwdList(void)
{
	for(auto it=pwds.begin(); it != pwds.end(); it++)
	{
		delete (*it);
	}
}

PwdList::PwdList(const PwdList& obj)
{
	throw std::logic_error("Not implemented");
}

PwdList::PwdVector PwdList::All() const
{
	return PwdVector(pwds.begin(), pwds.end());
}

PwdList::PwdVector PwdList::Filter(const SecureString& pattern) const
{
	return Filter(pattern, PwdLabelVector());
}

PwdList::PwdVector PwdList::Filter(const PwdLabelVector& labels) const
{
	return Filter(SecureString(), labels);
}

PwdList::PwdVector PwdList::Filter(const SecureString& pattern, const PwdLabelVector& labels) const
{
	PwdVector filtered;
	if(pattern.length() > 0)
	{
		SecureString patternCpy = pattern;
		const char* strPtrn = patternCpy.getUnsecureString();

		for(auto it = pwds.begin(); it != pwds.end(); it++)
		{
			SecureString description = (*it)->GetDescription();
			if(strstr(description.getUnsecureString(), strPtrn) != NULL)
			{
				filtered.push_back((*it));
			}
			description.UnsecuredStringFinished();
		}

		patternCpy.UnsecuredStringFinished();
	} else {
		filtered = PwdVector(pwds.begin(), pwds.end());
	}

	if(labels.size() > 0)
	{
		for(auto iLabel = labels.begin(); iLabel != labels.end(); iLabel++)
		{
			PwdVector passed;
			for(auto iPwd = filtered.begin(); iPwd != filtered.end(); iPwd++)
			{
				if((*iPwd)->HasLabel(*iLabel))
				{
					passed.push_back(*iPwd);
				}
			}
			filtered = passed;
		}
	}
	return filtered;
}


Pwd* PwdList::CreatePwd(const SecureString& desciption, const SecureString& password)
{
	return CreatePwd(desciption, SecureString(), password);
}

Pwd* PwdList::CreatePwd(const SecureString& desciption, const SecureString& username, const SecureString& password)
{
	//check if pwd is unique
	for(auto it = pwds.begin(); it != pwds.end(); it++)
	{
		if((*it)->GetDescription().equals(desciption))
			throw runtime_error("A password already exist with that name!");
	}

	Pwd* pwd = new Pwd();
	pwd->SetDescription(desciption);
	pwd->SetUsername(username);
	pwd->SetPassword(password);
	
	pwds.push_back(pwd);

	return pwd;
}

void PwdList::DeletePwd(Pwd* pwd)
{
	pwds.remove(pwd);
}
            
PwdLabelVector PwdList::FilterLabels(SecureString pattern)
{
	PwdLabelVector vector;
	const char* strPtrn = pattern.getUnsecureString();
	for(auto it = existingLabels.begin(); it != existingLabels.end(); it++)
	{
		if(strstr(strPtrn, (*it).getUnsecureString()) != NULL)
		{
			vector.push_back((*it));
		}
		(*it).UnsecuredStringFinished();
	}
	pattern.UnsecuredStringFinished();

	return vector;
}

int PwdList::CountPwds()
{
	return pwds.size();
}

int PwdList::CountPwds(const SecureString& label)
{
	int count = 0;
	for(auto it = pwds.begin(); it != pwds.end(); it++)
	{
		if((*it)->HasLabel(label))
			count++;
	}
	return count++;
}

            
bool PwdList::AddPwdToLabel(Pwd* pwd, SecureString label)
{
	if(std::find(existingLabels.begin(), existingLabels.end(), label) == existingLabels.end())
	{
		existingLabels.push_back(label);
	}
	pwd->AddLabel(label);
	return false;
}

bool PwdList::RemovePwdFromLabel(Pwd* pwd, SecureString label)
{
	pwd->RemoveLabel(label);
	if(CountPwds(label) == 0)
	{
		existingLabels.remove(label);
	}
	return false;
}