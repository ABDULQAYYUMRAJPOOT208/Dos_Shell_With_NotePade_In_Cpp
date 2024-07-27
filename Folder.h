#pragma once
#include <iostream>
#include "DoubleLinkedList.h"
#include <time.h>
#include <ctime>
#include "File.h"
using namespace std;
// template <class T>
class folder
{

public:
	int fsize;
	string name;
	dLinkList<folder*> folders;
	dLinkList<file*> files;
	folder* parent;
	string owner;
	string path;
	string timeCreated;
	bool isHidden;
	bool isReadOnly;
	folder(string nm, string pth)
		//: name(nm = "NUll"), parent(prnt = nullptr), owner(onr = "ABDUL QAYYUM RAJPOOT"), path(""), folders(fldrs = dLinkList<folder *>()), files(fls = dLinkList<file *>()), isHidden(false), isReadOnly(false) {}
	{
		name = nm;
		parent = nullptr;
		owner = "ABDUL QAYYUM RAJPOOT";
		path = pth;
		folders = dLinkList<folder*>();
		files = dLinkList<file*>();
		setTime();
		fsize = size();
	}
	folder(string nm, string pth, folder* prnt)
		//: name(nm = "NUll"), parent(prnt = nullptr), owner(onr = "ABDUL QAYYUM RAJPOOT"), path(""), folders(fldrs = dLinkList<folder *>()), files(fls = dLinkList<file *>()), isHidden(false), isReadOnly(false) {}
	{
		name = nm;
		parent = prnt;
		owner = "ABDUL QAYYUM RAJPOOT";
		path = pth;
		folders = dLinkList<folder*>();
		files = dLinkList<file*>();
		setTime();
		fsize = size();
	}
	folder(string nm, folder* prnt = nullptr, string onr = "ABDUL QAYYUM RAJPOOT", string pth = "notDefined", dLinkList<folder*> fldrs = dLinkList<folder*>(), dLinkList<file*> fls = dLinkList<file*>())
		//: name(nm = "NUll"), parent(prnt = nullptr), owner(onr = "ABDUL QAYYUM RAJPOOT"), path(""), folders(fldrs = dLinkList<folder *>()), files(fls = dLinkList<file *>()), isHidden(false), isReadOnly(false) {}
	{
		name = nm;
		path = pth;
		setTime();
		fsize = size();
	}
	// folder(string nm, folder *prnt = nullptr, string onr = "ABDUL QAYYUM RAJPOOT", string pth, dLinkList<folder *> fldrs = dLinkList<folder *>(), dLinkList<file *> fls = dLinkList<file *>())
	////: name(nm = "NUll"), parent(prnt = nullptr), owner(onr = "ABDUL QAYYUM RAJPOOT"), path(""), folders(fldrs = dLinkList<folder *>()), files(fls = dLinkList<file *>()), isHidden(false), isReadOnly(false) {}
	//{
	//    name = nm;
	//    path = pth;
	//}
	// calculate and return size of this object
	int size() const { return sizeof(*this); }
	void setTime()
	{
		time_t now = time(0);
		struct tm timeinfo;

		if (localtime_s(&timeinfo, &now) == 0)
		{
			char dt[25];
			strftime(dt, sizeof(dt), "%m/%d/%Y   %I:%M %p", &timeinfo);
			timeCreated = dt;
		}
		else
		{
			timeCreated = "error";
		}
	}
	file* findFile(string& Name)
	{
		for (int i = 0; i < files.size; i++)
		{
			if (Name == files[i]->name)
			{
				return files[i];
			}
		}
		return nullptr;
	}
	int getFileIndex(string& Name)
	{
		for (int i = 0; i < files.size; i++)
		{
			if (Name == files[i]->name)
			{
				return i;
			}
		}
		return -1;
	}

	file* FindFunction(string& nm)
	{
		file* nfile;
		if (this->files.size == 0 and this->folders.size == 0)
		{
			return nullptr;
		}
		else if (this->folders.size == 0)
		{
			for (int i = 0; i < this->files.size; i++)
			{
				if (this->files[i]->name == nm)
				{
					nfile = this->files[i];
					break;
				}
			}
		}
		else if (this->folders.size != 0 and this->files.size != 0)
		{
			file* result = nullptr;
			for (int i = 0; i < this->files.size; i++)
			{
				if (this->files[i]->name == nm)
				{
					return this->files[i];
				}
			}
			if (result == nullptr)
			{
				deepFind(folders.head, nm);
			}
		}
		else
		{
			deepFind(folders.head, nm);
		}
	}

	file* deepFind(Node<folder*>* nf, string& nm)
	{
		if (nf->data == nullptr)
		{
			return nullptr;
		}
		else if (nf->data->files.size != 0 and nf->data->folders.size == 0)
		{
			for (int i = 0; i < nf->data->files.size; i++)
			{
				if (nf->data->files[i]->name == nm)
				{
					return nf->data->files[i];
				}
			}
			return nullptr;
		}
		else if (nf->data->folders.size != 0 and nf->data->files.size == 0)
		{
			deepFind(nf->next, nm);
		}
		else
		{
			file* result = nullptr;
			for (int i = 0; i < nf->data->files.size; i++)
			{
				if (nf->data->files[i]->name == nm)
				{
					result = nf->data->files[i];
				}
			}
			if (result == nullptr)
			{
				deepFind(nf->next, nm);
			}
		}
	}

	folder* findFolder(string& Name)
	{
		if (this->name == Name)
		{
			return this;
		}
		for (int i = 0; i < folders.size; i++)
		{
			if (Name == folders[i]->name)
			{
				return folders[i];
			}
		}
		return nullptr;
	}
	int getFolderIndex(string& Name)
	{
		for (int i = 0; i < folders.size; i++)
		{
			if (Name == folders[i]->name)
			{
				return i;
			}
		}
		return -1;
	}


	void changeAllExtention(string& old, string& nnew)
	{
		for (int i = 0; i < this->files.size; i++)
		{
			if (files[i]->extension == old)
			{
				files[i]->extension = nnew;
			}
		}
	}

	void addFolder(folder* f)
	{
		folders.insertAtTail(f);
	}

	void addFile(file* f)
	{
		files.insertAtTail(f);
	}
	void printFolder()
	{
		if (!isHidden)
		{
			if (isReadOnly)
			{
				cout << "ReadOnly---"
					<< "\t\t" << timeCreated << "\t\t" << fsize << " bytes"
					<< "\t\t" << path << "\t\t"
					<< "<" << name << ">" << endl;
			}
			else
			{
				cout << "Read & Write"
					<< "\t\t" << timeCreated << "\t\t" << fsize << " bytes"
					<< "\t\t" << path << "\t\t"
					<< "<" << name << ">" << endl;
			}
		}
	}

	bool isFoldNamePresent(string& n)
	{
		for (int i = 0; i < folders.size; i++)
		{
			if (folders[i]->name == n)
			{
				return true;
			}
		}
		return false;
	}

	void printStructureOfFolder(folder* root, folder*& f, int count = 1)
	{
		helperStructure(f, count);
	}
	void helperStructure(folder*& nf, int count)
	{
		try
		{
			if (nf == nullptr)
			{
				return;
			}
			for (int i = 0; i < count; i++)
			{
				cout << "   ";
			}
			cout << "|___" << nf->name << endl;
			if (nf->folders.size != 0)
			{
				count++;
				for (int i = 0; i < folders.size; i++)
				{

					helperStructure(nf->folders[i], count);
				}
			}
		}
		catch (const std::exception&)
		{
			return;
		}
	}
};