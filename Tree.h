#pragma once
#include "Folder.h"
#include <fstream>
#include <queue>
#include <string>
// template <class T>

class Compare {
public:
	bool operator()(file* below, file* above)
	{
		return below->size() < above->size();
	}
};
class tree
{
public:
	folder* root;
	folder* CF;

	priority_queue<file*, vector<file*>, Compare> pQueue;
	queue<file*> simpleQueue;
	bool isPrompt = true;

	tree()
	{
		root = new folder("v", "v\\");
		CF = root;
	}

	void print()
	{
		cout << CF->path << printPrompt();
	}
	string printPrompt()
	{
		if (isPrompt)
		{
			return "\\> ";
		}
		else
		{
			return "$> ";
		}
	}
	folder* doesPathExist(string& pth)
	{
		folder* f = root;
		string str = "";
		for (char c : pth)
		{
			if (c != '\\')
			{
				str += c;
			}
			else
			{
				f = f->findFolder(str);
				if (f != nullptr)
				{
					str.clear();
				}
				else
				{
					// Folder not found, return nullptr
					return nullptr;
				}
			}
		}

		// Check the last folder in the path
		if (!str.empty())
		{
			f = f->findFolder(str);
		}

		return f;
	}
	void printPriorityQueue()
	{
		priority_queue<file*, vector<file*>, Compare>  q = pQueue;
		while (!q.empty())
		{
			cout << q.top()->name << "\t< " << q.top()->extension << " >" << endl;
			q.pop();
		}
	}
	void printSimpleQueue()
	{
		queue<file*>  q = simpleQueue;
		while (!q.empty())
		{
			cout << q.front()->name << "\t< " << q.front()->extension << " >" << endl;
			q.pop();
		}
	}
	folder* getParent()
	{
		return CF->parent;
	}
	void loadTree(string& fileName)
	{
		ifstream read;
		read.open(fileName);
		if (!read.is_open())
		{
			cerr << "No File Found with this Name..." << endl;
			return;
		}

		string line = "";
		getline(read, line);
		folder* nroot = new folder(line, CF->path + line);
		this->CF->folders.insertAtTail(nroot);
		line.clear();
		folder* tempCurr = nroot;

		while (!read.eof())
		{
			getline(read, line);
			if (line.empty()) continue;

			processLine(line, tempCurr);
		}

		read.close();
	}

	void processLine(string& line, folder* parent)
	{
		while (!line.empty())
		{
			string local = line.substr(0, line.find('\\'));
			line = line.substr(line.find('\\') + 1, line.length());

			int idx = local.find('.');
			if (idx != -1)
			{
				// Here i assumed that this would be a file...
				file* nfile = new file(sepName(local), parent->path + "\\" + local, local.substr(local.find('.')));
				parent->files.insertAtTail(nfile);
			}
			else
			{
				//  Here i assumed that this would be a folder
				folder* tempFolder = parent->findFolder(local);
				if (tempFolder == nullptr)
				{
					folder* nnew = new folder(local, parent->path + "\\" + local);
					parent->addFolder(nnew);
					parent = nnew;
				}
				else
				{
					parent = tempFolder;
				}
			}
		}
	}
	string sepName(string& nm)
	{
		string str = "";
		for (char c : nm)
		{
			if (c == '.')
			{
				break;
			}
			str += c;
		}
		return str;
	}



};