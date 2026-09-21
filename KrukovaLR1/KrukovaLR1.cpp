
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
void Show_Pipe(Pipe p)
{   if (!p.exists)
    {
        cout << "Труба еще не добавлена." << endl;
        return;
    }
    cout << "\nТруба: " << endl;
    cout << "Название: " << p.name << endl;
    cout << "Длина: " << p.length << " км" << endl;
    cout << "Диаметр: " << p.diameter << " мм" << endl;
    if (p.repair)
    {
        cout << "В ремонте: да" << endl;
    }
    else
    {
        cout << "В ремонте: нет" << endl;
    }
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Pipe p{};
    Add_Pipe(p);
    Show_Pipe(p);
    return 0;
}

