#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "Tree.h"
#include <iomanip>
//#include "textEditor.h"
using namespace std;

class Dos
{

	vector<string> commands = { "mkdir","exit","cls","create","cd","cd..","attrib","pwd","cd\\","prompt","ver","find","findf","del","convert","rmdir","format","findstr" };
	vector<string> helps = { "Creates a virtual directory.","Quits the CMD.EXE program (command interpreter).","Clears the screen.","Creates a virtual file.","Changes the current virtual directory.","Changes the current virtual directory to its parent.","Displays the file attributes.","Displays the working virtual directory.","Changes to the virtual root directory.","Changes the MS DOS command prompt.","Displays the MS DOS version.","Searches for a file in current virtual directory.","Searches for a text string in a file.","Deletes a virtual file.","Convert the extension of all files with given extension.","Deletes a virtual directory.","Formats the current virtual directory.","Searches for a text string in all files of current virtual directory." };
	vector<string> methods = { "MKDIR [drive:]path","EXIT","CLS","CREATE [drive:]path[extension]\n\n\t\tthe extension should be .txt,.cpp or .exe.","CD [drive:][path]","CD..","ATTRIB [drive:][path][filename]","PWD","CD\\","PROMPT","VER","FIND [[drive:][path]filename[...]]","FINDF 'string' [[drive:][path]filename[...]]","convert [extension] [extension]","RMDIR [drive:]path","FORMAT","FIND 'string'","DEL [drive:]path[extension]" };

public:
	tree t;
	string user = "ABDUL QAYYUM RAJPOOT";
	string version = "1.0.0";
	Dos()
	{
	}
	void printName()
	{
		system("cls");
		cout << "\t\t\t\t\t\t\t" << user << endl
			<< endl;
	}
	void run()
	{
		bool flag = true;
		while (flag)
		{
			flag = menu();
		}
	}
	bool menu()
	{
		t.print();
		string input;
		getline(cin, input);
		toLower(input);
		input = removeExtraSpaces(input);
		string op = input.substr(0, input.find(' '));
		if (input == "cd\\")
		{
			t.CF = t.root;
		}
		else if (input == "help") {
			for (int i = 0; i < commands.size(); i++)
			{
				cout << left << setw(10) << commands[i] << "-->\t" << setw(10) << helps[i] << endl;
			}
		}
		else if (input._Equal("cd.."))
		{
			folder* f = t.getParent();
			if (f != nullptr)
			{
				this->t.CF = f;
			}
		}
		else if (input == "ver")
		{
			cout << "\u00A9 ABDUL QAYYUM RAJPOOT. All rights reserved. \nThe version of Dos is: " << version << endl;
		}
		else if (input == "cls")
		{
			system("cls");
			printName();
		}
		else if (input == "pwd")
		{
			cout << "Current working directory is: \t\t<" << t.CF->path << ">" << endl;
		}
		else if (input == "format")
		{
			t.CF->files = dLinkList<file*>();
			t.CF->folders = dLinkList<folder*>();
		}
		else if (input == "prompt")
		{
			t.isPrompt = !t.isPrompt;
		}
		else if (input == "tree")
		{
			t.CF->printStructureOfFolder(t.root, t.CF);
		}
		else if (input == "pqueue")
		{
			t.printPriorityQueue();
		}
		else if (input == "queue")
		{
			t.printSimpleQueue();
		}
		else if (op == "help")
		{
			string hName = input.substr(op.length() + 1, input.length());
			if (hName != "")
			{
				int idx = checkHelpCommand(hName);
				if (idx != -1)
				{
					cout << left << setw(10) << op << setw(10) << helps[idx] << endl;
				}
			}
		}
		else if (op == "cd")
		{
			string fName = input.substr(op.length() + 1, input.length());
			if (fName == "..")
			{
				folder* f = t.getParent();
				if (f != nullptr)
				{
					this->t.CF = f;
				}
			}
			else if (fName == ".")
			{
				t.CF = t.root;
			}
			else
			{
				folder* folderFound = t.CF->findFolder(fName);
				if (folderFound != nullptr)
				{
					t.CF = folderFound;
				}
				else
				{
					cerr << "No Directory Found with Name < " << fName << " >" << endl;
				}
			}
		}
		else if (op == "mkdir")
		{
			if (input.length() >= op.length() + 1)
			{
				string folderName = input.substr(op.length() + 1);
				string fName = input.substr(op.length() + 1 + countSpace(folderName), input.length());
				if (t.CF->isFoldNamePresent(fName))
				{
					string str;
					cout << "Another folder with this name already exist. Do you want to overwrite it (Y/N): ";
					cin >> str;
					toLower(str);
					if (str == "y")
					{

						folder* found = t.CF->findFolder(fName);
						string path = t.CF->path + "\\" + fName;
						folder* nfolder = new folder(fName, path, t.CF);
						*found = *nfolder;
						found->parent->fsize = nfolder->fsize;
					}
				}
				else
				{
					if (fName.length() == 0)
					{
						cerr << "You have to provide name of folder..." << endl;
					}
					else
					{

						string path = t.CF->path + "\\" + fName;
						folder* nfolder = new folder(fName, path, t.CF);
						t.CF->addFolder(nfolder);
						t.CF->fsize += nfolder->fsize;
					}
				}
			}
			else
			{
				cerr << "You have to provide name of folder..." << endl;
			}
		}
		else if (op == "rmdir")
		{
			if (input.length() >= op.length() + 1)
			{

				string fName = input.substr(op.length() + 1, input.length());
				folder* nfolder = t.CF->findFolder(fName);
				if (nfolder != nullptr)
				{
					int idx = t.CF->getFolderIndex(fName);
					t.CF->folders.deleteDataWithIndex(idx);
				}
				else
				{
					cerr << "No folder found by name <" << fName << ">" << endl;
				}
			}
			else
			{
				cerr << "You have to provide name of folder..." << endl;
			}
		}
		else if (op == "attrib" and input.length() == op.length())
		{
			if (this->t.CF->files.size != 0)
			{
				for (int i = 0; i < t.CF->files.size; i++)
				{
					t.CF->files[i]->showFileAtrrib();
				}
			}
			else
			{
				cerr << "No file found in this folder..." << endl;
			}
		}
		else if (op == "del")
		{
			if (input.length() >= op.length() + 1)
			{
				string fName = input.substr(op.length() + 1, input.length());
				int idx = t.CF->getFileIndex(fName);
				if (idx != -1)
				{
					t.CF->files.deleteDataWithIndex(idx);
				}
				else
				{
					cerr << "No file found with the name <" << fName << ">" << endl;
				}
			}
		}
		else if (op == "create")
		{
			if (input.length() >= op.length() + 1)
			{
				string fName = input.substr(op.length() + 1, input.length());
				int dots = fName.find('.');
				if (dots == 1)
				{
					if (isValidFName(fName))
					{
						string path = "\\" + t.CF->path + "\\" + fName;
						file* nfile = new file(sepName(fName), path, fName.substr(fName.find('.'), fName.length()));
						t.CF->addFile(nfile);
						ofstream write;
						write.open(fName);
						write.close();
						t.CF->fsize += nfile->fsize;
					}
					else
					{
						cerr << "You cannot enter special character in name of file or folder..." << endl;
					}
				}
				else if (dots == -1)
				{
					if (isValidFName(fName))
					{
						string path = "\\" + t.CF->path + "\\" + fName + ".txt";
						file* nfile = new file(fName, path, ".txt");
						t.CF->addFile(nfile);
						ofstream write;
						write.open(nfile->name + nfile->extension);
						write.close();
						t.CF->fsize += nfile->fsize;
					}
					else
					{
						cerr << "You cannot enter special character in name of file or folder..." << endl;
					}
				}


			}
		}
		else if (op == "dir")
		{
			if (t.CF->folders.size == 0 and t.CF->files.size == 0)
			{
				cerr << "No folders and files in this directory..." << endl;
			}
			else
			{
				cout << "Folders: " << endl;
				for (int i = 0; i < t.CF->folders.size; i++)
				{
					t.CF->folders[i]->printFolder();
				}
				cout << "Files: " << endl;
				for (int i = 0; i < t.CF->files.size; i++)
				{
					t.CF->files[i]->showFileAtrrib();
				}
			}
		}
		else if (op == "find")
		{
			if (input.length() >= op.length() + 1)
			{
				string fName = input.substr(op.length() + 1, input.length());
				file* nfile = t.CF->FindFunction(fName);
				if (nfile == nullptr)
				{
					cerr << "file not found..." << endl;
				}
				else
				{
					// cout <<
					nfile->showFileAtrrib(); //<<endl;
				}
			}
		}
		else if (op == "copy")
		{
			// path = source\\space destination\\ 
			string cp = input.substr(op.length() + 3, input.find(' ')+2);
			string paste = input.substr((op.length() + cp.length() + 4), input.length());
			file* copy;

			// finding file in path folder
			folder* tmp = t.root;
			string f;
			while (cp.length() > 0)
			{
				f = cp.substr(0, cp.find('\\'));
				cp = cp.substr(f.length() + 1, cp.length());
				if (cp.length() != 0)
				{
					tmp = tmp->findFolder(f);
				}
				if (tmp == nullptr)
				{
					cerr << "path does not exists..." << endl;
					return true;
				}
			}
			string ff = sepName(f);
			copy = tmp->findFile(ff);

			tmp = t.root;
			while (paste.length() > 0)
			{
				f = paste.substr(0, paste.find('\\'));
				paste = paste.substr(f.length() + 1, paste.length());
				tmp = tmp->findFolder(f);
				if (tmp == nullptr)
				{
					cerr << "path does not exists..." << endl;
					return true;
				}
			}
			copy->path = tmp->path + "\\" + copy->name;
			tmp->addFile(copy);
		}
		else if (op == "mov")
		{
			string cp = input.substr(op.length() + 3, input.find('\\') - 1);
			string paste = input.substr((op.length() + cp.length() + 4), input.length());
			file* move;

			// finding file in path folder
			folder* tmp = t.root;
			string f;
			while (cp.length() > 0)
			{
				f = cp.substr(0, cp.find(' '));
				cp = cp.substr(f.length() + 1, cp.length());
				if (cp.length() != 0)
				{
					tmp = tmp->findFolder(f);
				}
				if (tmp == nullptr)
				{
					cerr << "path does not exists..." << endl;
					return true;
				}
			}

			move = tmp->findFile(f);
			int idx = tmp->getFileIndex(move->name);
			tmp->files.deleteDataWithIndex(idx);
			tmp = t.root;
			while (paste.length() > 0)
			{
				f = paste.substr(0, paste.find('\\'));
				paste = paste.substr(f.length() + 1, paste.length());
				tmp = tmp->findFolder(f);
				if (tmp == nullptr)
				{
					cerr << "path does not exists..." << endl;
					return true;
				}
			}
			move->path = tmp->path + "\\" + move->name;
			tmp->addFile(move);
		}
		else if (op == "rename")
		{
			string old = input.substr(op.length() + 1, input.find(' '));
			string nname = input.substr(old.length() + 1, input.length());
			file* f = t.CF->findFile(old);
			if (f != nullptr)
			{
				f->name = nname;
			}
			else
			{
				cerr << "No file found with this name..." << endl;
			}
		}
		else if (op == "convert")
		{
			string old = input.substr(op.length() + 1, input.find(' '));
			string nextention = input.substr(old.length() + 1, input.length());
			t.CF->changeAllExtention(old, nextention);
		}
		else if (op == "pprint")
		{
			string source = input.substr(op.length() + 3, input.length());
			file* file;
			// finding file in path folder
			folder* tmp = t.root;
			string f;
			while (source.length() > 0)
			{
				f = source.substr(0, source.find('\\'));
				source = source.substr(f.length() + 1, source.length());
				if (source.length() != 0)
				{
					tmp = tmp->findFolder(f);
				}
				if (tmp == nullptr)
				{
					cerr << "path does not exists..." << endl;
					return true;
				}
			}

			file = tmp->findFile(f);
			if (file != nullptr)
			{
				t.pQueue.push(file);
			}
			else
			{
				throw "No file with this name...";
			}
			t.printPriorityQueue();
		}
		else if (op == "print")
		{
			string source = input.substr(op.length() + 3, input.length());
			file* file;
			// finding file in path folder
			folder* tmp = t.root;
			string f;
			while (source.length() > 0)
			{
				f = source.substr(0, source.find('\\'));
				source = source.substr(f.length() + 1, source.length());
				if (source.length() != 0)
				{
					tmp = tmp->findFolder(f);
				}
				if (tmp == nullptr)
				{
					cerr << "path does not exists..." << endl;
					return true;
				}
			}

			file = tmp->findFile(f);
			if (file != nullptr)
			{
				t.simpleQueue.push(file);
			}
			else
			{
				throw "No file with this name...";
			}
			t.printSimpleQueue();
		}
		else if (op == "edit")
		{
			string fName = input.substr(op.length() + 1, input.length());
			file* f = t.CF->findFile(fName);
			if (f != nullptr)
			{
				ifstream read;
				string filename = f->name + f->extension;
				read.open(filename);
				Editor e;
				e.openSaveFile(read);
				read.close();
				ofstream write;
				write.open(filename);
				system("cls");
				e.editFile(write);
				system("cls");
				write.close();
				printName();
			}
		}
		else if (op == "loadtree")
		{
			string fName = input.substr(op.length() + 1, input.length());
			t.loadTree(fName);
		}
		else if (input == "exit")
		{
			return false;
		}
		else
		{
			cerr << "No command found with this name..." << endl;
			return true;
		}
	}
	string removeExtraSpaces(const string& input)
	{
		istringstream iss(input);
		ostringstream oss;

		// Copy non-empty tokens separated by spaces to the output stream
		string token;
		while (iss >> token)
		{
			oss << token << ' ';
		}

		// Remove trailing space
		return oss.str().substr(0, oss.str().size() - 1);
	}
	void toLower(string& str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			str[i] = (char)tolower(str[i]);
		}
	}
	int countSpace(string& command)
	{
		int count = 0;
		for (int i = 0; i < command.length(); i++)
		{
			if (command[i] == ' ')
			{
				count++;
			}
			else
			{
				break;
			}
		}
		return count;
	}
	bool isValidFName(string& name)
	{
		char c;
		for (int i = 0; i < name.length(); i++)
		{
			c = name[i];
			bool check1 = (c >= 65 and c <= 90);
			bool check2 = (c >= 97 and c <= 122);
			bool check3 = c == '_' or c == 46;
			bool check4 = c >= 48 and c <= 57;

			if (check1 or check2 or check3 or check4)
			{
				continue;
			}
			else return false;
		}
		return true;
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
	string giveExtention(string& nm)
	{
		string str = "";
		for (int i = nm.length(); i > 0; i--)
		{
			str = nm[i] + str;
		}
		return str.substr(1, str.length());
	}




	
	/*else if (op == "help")
	{	string name = "";
		if (op.length() != command.length())
		{
			string substring = command.substr(5, command.length());
			name = removeSpace(substring);
		}
		if (name == "")
		{
			cout << "For more information on a specific command, type HELP command-name\n";
			for (int i = 0; i < commands.size(); i++)
			{
				cout << commands[i] << "\t\t" << helps[i] << endl;
			}
		}
		else
		{
			int comIndex = checkHelpCommand(name, commands);
			if (comIndex != -1)
			{
				cout << helps[comIndex] << "\n\n" << methods[comIndex] << endl;
			}
			else
			{
				cout << "This command is not supported by the help utility.";
			}
		}
		cout << endl;
		t.print();
		return false;
		}


		*/




	int checkHelpCommand(string& com)
	{
		for (int i = 0; i < commands.size(); i++)
		{
			if (com == commands[i])
			{
				return i;
			}
		}
		return -1;
	}
};