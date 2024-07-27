#pragma once
#include <iostream>
using namespace std;
// template <class T>
class file
{
public:
    int fsize;
    string name;
    string extension;
    bool isHidden;
    string creationTime;
    string data;
    string path;

    file(string nm, string pth, string ex) : name(nm), isHidden(false),extension(ex)
    {
        name = nm;
       // extension = giveExtention(nm);
        setTime();
        fsize = size();
    }
    int size()
    {
        return sizeof(*this);
    }
    void showFileAtrrib()
    {
        cout << " A\t\t"<<creationTime<<"\t\t" << size() << " bytes" << "\t\t< " << extension << " >\t\t" << this->path << "\t\t< " << name << " >" << endl;
    }
  /*  string giveExtention(string& nm)
    {
        string str= "";
        for (int i = nm.length(); i > 0; i--)
        {
            str += nm[i];
        }
        return str.substr(0,str.find('.'));
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

    }*/
    void setTime()
    {
        time_t now = time(0);
        struct tm timeinfo;

        if (localtime_s(&timeinfo, &now) == 0) {
            char dt[25];
            strftime(dt, sizeof(dt), "%m/%d/%Y   %I:%M %p", &timeinfo);
            creationTime = dt;
        }
        else {
            creationTime = "error";
        }
    }
};