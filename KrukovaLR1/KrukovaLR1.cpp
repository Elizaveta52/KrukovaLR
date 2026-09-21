
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

struct Pipe
{
    string name;
    double length;
    int diameter;
    bool repair;
    bool exists;
};
void Add_Pipe(Pipe& p)
{
    cout << "\nДобавление трубы" << endl;
    cout << "Введите название трубы: ";
    cin >> p.name;
    cout << "Введите длину трубы (км): ";
    cin >> p.length;
    cout << "Введите диаметр трубы (мм): ";
    cin >> p.diameter;
    p.repair = false;
    p.exists = true;
    cout << "Труба добавлена." << endl;
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Pipe p{};
    Add_Pipe(p);
    return 0;
}

