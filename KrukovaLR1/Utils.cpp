#include "Utils.h"
using namespace std;

string ReadLine(const string& prompt)
{
    cout << prompt;
    cerr << prompt;
    string line;
    if (!getline(cin, line))
    {
        cerr << "Конец ввода." << endl;
        throw runtime_error("Ввод завершён.");
    }
    cerr << line << endl;
    return line;
}

string ReadName(const string& prompt)
{
    while (true)
    {
        string name = ReadLine(prompt);
        if (name.find_first_not_of(" \t\r") != string::npos)
            return name;
        Message("Строка не должна быть пустой.");
    }
}

void Message(const string& text)
{
    cout << text << endl;
    cerr << text << endl;
}
