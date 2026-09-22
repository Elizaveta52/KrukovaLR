
#include <iostream>
#include <string>
#include <fstream>
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
int ReadInt()
{
    int value;
    while (true)
    {
        if (cin >> value)
        {
            if (cin.peek() == '\n')
            {
                return value;
            }
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Ошибка. Введите целое число ещё раз: ";
    }
}

double ReadDouble()
{
    double value;
    while (true)
    {
        if (cin >> value)
        {
            if (cin.peek() == '\n')
            {
                return value;
            }
        }

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Ошибка. Введите число ещё раз: ";
    }
}

void Add_Pipe(Pipe& p)
{
    cout << "\nДобавление трубы" << endl;
    cout << "Введите название трубы: ";
    cin >> p.name;

    cout << "Введите длину трубы (км): ";
    p.length = ReadDouble();
    while (p.length <= 0)
    {
        cout << "Длина должна быть больше 0: ";
        p.length = ReadDouble();
    }

    cout << "Введите диаметр трубы (мм): ";
    p.diameter = ReadInt();
    while (p.diameter <= 0)
    {
        cout << "Диаметр должен быть больше 0: ";
        p.diameter = ReadInt();
    }


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
    k.workshops = ReadInt();
    while (k.workshops <= 0)
    {
        cout << "Количество цехов должно быть больше 0: ";
        k.workshops = ReadInt();
    }

    k.workshops_work = k.workshops;

    cout << "Введите класс станции: ";
    k.class_cs = ReadInt();
    while (k.class_cs <= 0)
    {
        cout << "Класс станции должен быть больше 0: ";
        k.class_cs = ReadInt();
    }
    k.exists = true;

    cout << "КС добавлена." << endl;
}
void Show_Pipe(Pipe p)
{   
    if (!p.exists)
    {
        cout << "Труба ещё не добавлена." << endl;
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
void Edit_Pipe(Pipe& p)
{
    if (!p.exists)
    {
        cout << "Сначала добавьте трубу." << endl;
        return;
    }
    cout << "\nТруба: " << p.name << endl;
    cout << "1. Отправить трубу в ремонт" << endl;
    cout << "2. Убрать трубу из ремонта" << endl;
    cout << "Выберите действие: ";

    int choice;
    choice = ReadInt();
    if (choice == 1)
    {
        p.repair = true;
        cout << "Труба отправлена в ремонт." << endl;
    }
    else if (choice == 2)
    {
        p.repair = false;
        cout << "Труба выведена из ремонта." << endl;
    }
    else
    {
        cout << "Неверный выбор." << endl;
    }
}

void Edit_CS(CS& k)
{
    if (!k.exists)
    {
        cout << "Сначала добавьте КС." << endl;
        return;
    }

    cout << "\nКС: " << k.name << endl;
    cout << "Всего цехов: " << k.workshops << endl;
    cout << "Работает: " << k.workshops_work << endl;

    cout << "1. Запустить цех" << endl;
    cout << "2. Остановить цех" << endl;
    cout << "Выберите действие: ";

    int choice;
    choice = ReadInt();

    if (choice == 1)
    {
        if (k.workshops_work < k.workshops)
        {
            k.workshops_work++;
            cout << "Цех запущен." << endl;
        }
        else
        {
            cout << "Все цеха уже работают." << endl;
        }
    }
    else if (choice == 2)
    {
        if (k.workshops_work > 0)
        {
            k.workshops_work--;
            cout << "Цех остановлен." << endl;
        }
        else
        {
            cout << "Нет работающих цехов." << endl;
        }
    }
    else
    {
        cout << "Неверный выбор." << endl;
    }
}

void Save_File(Pipe p, CS k, string filename)
{
    ofstream fout;

    fout.open(filename);

    if (!fout.is_open())
    {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    fout << "ТРУБА" << endl;

    if (p.exists)
    {
        fout << "Название: " << p.name << endl;
        fout << "Длина трубы (км): " << p.length << endl;
        fout << "Диаметр трубы (мм): " << p.diameter << endl;
        fout << "В ремонте: " << p.repair << endl;
    }
    else
    {
        fout << "Труба не добавлена." << endl;
    }

    fout << endl;

    fout << "КОМПРЕССОРНАЯ СТАНЦИЯ" << endl;

    if (k.exists)
    {
        fout << "Название: " << k.name << endl;
        fout << "Количество цехов: " << k.workshops << endl;
        fout << "Количество работающих цехов: " << k.workshops_work << endl;
        fout << "Класс станции: " << k.class_cs << endl;
    }
    else
    {
        fout << "КС не добавлена." << endl;
    }

    fout.close();

    cout << "Данные сохранены в файл: " << filename << endl;
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Pipe p{};
    CS k{};
    int choice;
    string filename;
    p.exists = false;
    k.exists = false;

    while (true)
    {
        Show_Menu();

        choice = ReadInt();

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
        case 4:
            Edit_Pipe(p);
            break;
        case 5:
            Edit_CS(k);
            break;
        case 6:
            cout << "Введите имя файла: ";
            cin >> filename;
            Save_File(p, k, filename);
            break;

        case 0:
            return 0;
        }
    }
}

