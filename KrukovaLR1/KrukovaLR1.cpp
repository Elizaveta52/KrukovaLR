
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
struct CS
{
    string name;
    int workshops;
    int workshops_work;
    int class_cs;
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
void Add_CS(CS& k)
{
    cout << "\nДобавление компрессорной станции" << endl;
    cout << "Введите название КС: ";
    cin >> k.name;

    cout << "Введите количество цехов: ";
    cin >> k.workshops;

    k.workshops_work = k.workshops;

    cout << "Введите класс станции: ";
    cin >> k.class_cs;

    k.exists = true;

    cout << "КС добавлена." << endl;
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
void Show_CS(CS k)
{
    if (!k.exists)
    {
        cout << "КС ещё не добавлена." << endl;
        return;
    }

    cout << "\nКомпрессорная станция:" << endl;
    cout << "Название: " << k.name << endl;
    cout << "Количество цехов: " << k.workshops << endl;
    cout << "Количество работающих цехов: " << k.workshops_work << endl;
    cout << "Класс станции: " << k.class_cs << endl;
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Pipe p{};
    CS k{};
    Add_Pipe(p);
    Show_Pipe(p);
    Add_CS(k);
    Show_CS(k);
    return 0;
}

