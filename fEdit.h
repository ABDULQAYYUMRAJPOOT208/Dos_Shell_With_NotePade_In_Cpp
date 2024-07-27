#pragma once
#include "list"
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <conio.h>
#include <deque>
#include <Windows.h>
using namespace std;

// Function to move the cursor to a specific row and column on the console
void gotoRowCol(int curRow, int curCol) {
    COORD coord;
    coord.X = curCol;
    coord.Y = curRow;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// List to store file names
list<string> Files;

// Structure to represent the state of the editor
struct state {
    list<list<char>> text;
    list<list<char>>::iterator riter;
    list<char>::iterator citer;
    int r, c;
};

// Class definition for the text editor
class Editor {
public:
    int curRow, curCol;
    list<list<char>> text;
    list<list<char>>::iterator riter;
    list<char>::iterator citer;
    deque<state> undo;
    stack<state> redo;

    // Constructor for the Editor class
    Editor() {
        text.push_back(list<char>());
        riter = text.begin();
        (*riter).push_back(' ');
        citer = (*riter).begin();
        curRow, curCol = 0;
    }

    // Function to save the current state of the editor
    state saveState() {
        state* s = new state;
        s->text.push_back(list<char>());
        auto r_itr = s->text.begin();
        for (auto row = text.begin(); row != text.end(); row++, r_itr++) {
            s->text.push_back(list<char>());
            for (auto col = (*row).begin(); col != (*row).end(); col++) {
                char ch = *col;
                (*r_itr).push_back(ch);
            }
        }
        s->riter = s->text.begin();
        s->citer = (*s->riter).begin();
        s->c = curCol;
        s->r = curRow;
        return *s;
    }

    // Function to load a previously saved state of the editor
    void loadState(state s) {
        text = s.text;
        riter = text.begin();
        curCol = s.c;
        curRow = s.r;
        for (int r = 0; r < s.r; r++) {
            riter++;
        }
        citer = (*riter).begin();
        for (int c = 0; c < s.c; c++) {
            citer++;
        }
    }

    // Function to close the file and save the list of file names to "SaveFile.txt"
    void closing(ifstream& rdr) {
        rdr.close();
        ofstream wtr;
        wtr.open("SaveFile.txt");
        for (auto i = Files.begin(); i != Files.end(); i++) {
            wtr << (*i) << endl;
        }
        wtr.close();
    }

    // Function to create a new file and start editing it
    void createFile(string& fileName) {
        ofstream wrt(fileName.c_str());
        system("cls");
        editFile(wrt);
        system("cls");
        wrt.close();
    }

    // Function to display a note at the top of the console
    void note() {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, FOREGROUND_RED);
        gotoxy(140, 7);
        cout << "Press esc to save File...";
        SetConsoleTextAttribute(h, FOREGROUND_GREEN);
    }

    // Function to edit the content of the file
    void editFile(ofstream& wrt) {
        system("color 3F");
        char c;
        c = _getch();
        gotoRowCol(curRow, curCol);
        cout << c;
        (*citer) = c;
        curCol++;

        while (true) {
            gotoRowCol(curRow, curCol);

            c = _getch();

            if (c == -32) {
                // Arrow key handling
            A:
                c = _getch();
                // Up Arrow
                if (c == 72) {
                    if (curRow == 0) {
                        continue;
                    }
                    riter--;
                    citer = (*riter).begin();
                    curRow--;
                    curCol = 0;
                }
                // Down Arrow
                else if (c == 80) {
                    if (text.size() == 1)
                    {
                        continue;
                    }
                    if (curRow < text.size()-1)
                    {
                        riter++;
                        citer = (*riter).begin();
                        curRow++;
                        curCol = 0;
                    }
                    
                }
                // Left Arrow
                else if (c == 75) {
                    if (curCol - 1 > 0) {
                        citer--;
                        curCol--;
                    }
                }
                // Right Arrow
                else if (c == 77) {
                    if (curCol == 159) {
                        continue;
                    }
                    if (curCol < (*riter).size()) {
                        citer++;
                        curCol++;
                    }
                }
                // Delete key
                else if (c == 83) {
                    auto temp = citer;
                    (*riter).erase(citer);
                    citer = temp;
                    system("cls");
                    print();
                    updateUndo();
                }
                // Handling for starting of the line
                if (curCol == 0) {
                    gotoRowCol(curRow, 0);
                    c = _getch();
                    if (c == -32) {
                        goto A;
                    }
                    (*riter).push_front(c);
                    citer = (*riter).begin();
                    curCol = 1;
                    system("cls");
                    print();
                    updateUndo();
                }
                continue;
            }
            // Enter key
            else if (c == 13) {
                if (citer != (*riter).end()) {
                    citer++;
                    list<char> nline(citer, (*riter).end());
                    (*riter).erase(citer, (*riter).end());
                    auto temp = riter;
                    riter++;
                    text.insert(riter, nline);
                    riter = ++temp;
                    citer = (*riter).begin();
                    curRow++;
                    curCol = 0;
                    gotoRowCol(curRow, curCol);
                }
                else {
                    auto temp = riter;
                    riter++;
                    text.insert(riter, list<char>());
                    riter = ++temp;
                    curRow++;
                    curCol = 0;
                    gotoRowCol(curRow, curCol);
                }
                system("cls");
                print();
                updateUndo();
                continue;
            }
            // Backspace key
            else if (c == 8) {
                if (curCol == 0) {
                    continue;
                }
                if (citer != (*riter).begin()) {
                    auto temp = --citer;
                    citer++;
                    (*riter).erase(citer);
                    citer = temp;
                    curCol--;
                    system("cls");
                    print();
                    updateUndo();
                    continue;
                }
                else {
                    (*riter).erase(citer);
                    citer = (*riter).begin();
                    goto afterSetCiter;
                }
                (*riter).erase(citer);
                afterSetCiter:
                curCol--;
                system("cls");
                print();
                updateUndo();
                continue;
            }
            // Undo key (Ctrl + Z
            // Undo key (Ctrl + Z)
            else if (c == 26) {
                if (!undo.empty()) {
                    state s = undo.back();
                    loadState(s);
                    redo.push(undo.back());
                    undo.pop_back();
                    system("cls");
                    print();
                }
                continue;
            }
            // Redo key (Ctrl + Y)
            else if (c == 25) {
                if (!redo.empty()) {
                    undo.push_back(redo.top());
                    state s = redo.top();
                    redo.pop();
                    loadState(s);
                    system("cls");
                    print();
                }
                continue;
            }
            // Escape key
            else if (c == 27) {
                writeTextToFile(wrt);
                break;
            }

            // Check if a new line needs to be added
            if (curCol == 100) {
                text.push_back(list<char>());
                riter++;
                curCol = 0;
                curRow++;
            }
            else {
                if (curCol == 0) {
                    (*riter).push_back(c);
                    citer = (*riter).begin();
                    curCol++;
                }
                else {
                    auto temp = citer;
                    citer++;
                    (*riter).insert(citer, c);
                    citer = ++temp;
                    curCol++;
                }
            }
            system("cls");
            print();
            updateUndo();
        }
        system("color 0F");
    }

    // Function to write the content of the editor to a file
    void writeTextToFile(ofstream& wrt) {
        for (auto r = text.begin(); r != text.end(); r++) {
            for (auto c = (*r).begin(); c != (*r).end(); c++) {
                wrt << *c;
            }
            wrt << '\n';
        }
    }

    // Function to update the undo stack
    void updateUndo() {
        if (undo.size() > 5) {
            undo.erase(undo.begin());
        }
        state s = saveState();
        undo.push_back(s);
    }

    // Function to open a file for editing
    void openSaveFile(ifstream& rdr) {
        char ch;
        while (!rdr.eof()) {
            rdr.get(ch);
            if ((*riter).size() == 1 and (*citer) == ' ') {
                break;
            }
            if (ch != '\n') {
                (*riter).push_back((ch));
            }
            else {
                text.push_back(list<char>());
                riter++;
            }
        }
        riter = text.begin();
        citer = (*riter).begin();
        curRow = 0;
        curCol = 0;
        gotoRowCol(curRow, curCol);
    }

    // Function to open an existing file for editing
    void openFile() {
        system("cls");
        string fileName;
        cout << "Enter File Name: ";
        cin >> fileName;
        if (find(Files.begin(), Files.end(), fileName) == Files.end()) {
            cout << "File Does not exist ";
            return;
        }
        ifstream rdr(fileName.c_str());
        openSaveFile(rdr);
        rdr.close();
        ofstream wrt(fileName.c_str());
        system("cls");
        print();
        gotoRowCol(curRow, curCol);
        editFile(wrt);
        system("cls");
        wrt.close();
    }

    // Function to print the content of the editor to the console
    void print() {
        for (auto r = text.begin(); r != text.end(); r++) {
            for (auto c = (*r).begin(); c != (*r).end(); c++) {
                cout << *c;
            }
            cout << endl;
        }
        note();
    }

    // Function to set the cursor position on the console
    void gotoxy(int x, int y) {
        COORD coordinates;
        coordinates.X = x;
        coordinates.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
    }
};
