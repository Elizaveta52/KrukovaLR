
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
};
struct CS
{
    string name;
    int workshops;
    int workshops_work;
    int class_cs;
};

void Show_Menu()
{
    cout << "\nМЕНЮ:" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотреть все объекты" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать КС" << endl;
    cout << "6. Сохранить" << endl;
    cout << "7. Загрузить" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

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


    cout << "КС добавлена." << endl;
}
void Show_Pipe(Pipe p)
{   
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
    int choice;

    while (true)
    {
        Show_Menu();

        cin >> choice;

        switch (choice)
        {
        case 1:
            Add_Pipe(p);
            break;

        case 2:
            Add_CS(k);
            break;

        case 3:
            Show_Pipe(p);
            Show_CS(k);
            break;

        case 0:
            return 0;
        }
    }
}

