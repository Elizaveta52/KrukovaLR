#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <windows.h>

using namespace std;

const int MAX_OBJECTS = 10000;

class Pipe
{
public:
    int id = 0;
    string name;
    double length = 0;
    int diameter = 0;
    bool repair = false;
};

class CS
{
public:
    int id = 0;
    string name;
    int workshops = 0;
    int workshops_work = 0;
    int class_cs = 0;
};

void LogAction(string action)
{
    ofstream log("actions.log", ios::app);

    if (!log.is_open())
    {
        cout << "Ошибка открытия журнала действий." << endl;
        return;
    }

    SYSTEMTIME t;
    GetLocalTime(&t);

    log << t.wDay << "." << t.wMonth << "." << t.wYear
        << " " << t.wHour << ":" << t.wMinute << ":" << t.wSecond
        << " | " << action << endl;

    if (!log)
        cout << "Ошибка записи журнала действий." << endl;
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

        if (cin.eof())
        {
            LogAction("Завершение ввода");
            exit(0);
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Ошибка. Введите целое число ещё раз: ";
        LogAction("Ошибка ввода целого числа");
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

        if (cin.eof())
        {
            LogAction("Завершение ввода");
            exit(0);
        }

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Ошибка. Введите число ещё раз: ";
        LogAction("Ошибка ввода вещественного числа");
    }
}

string ReadName()
{
    string name;

    while (true)
    {
        if (!getline(cin >> ws, name))
        {
            LogAction("Завершение ввода");
            exit(0);
        }

        if (!name.empty() && name.size() <= 200)
            return name;

        cout << "Введите название длиной от 1 до 200 символов: ";
        LogAction("Ошибка ввода названия");
    }
}

int ReadPositiveInt()
{
    int value = ReadInt();

    while (value <= 0)
    {
        cout << "Введите число больше 0: ";
        value = ReadInt();
    }

    return value;
}

double ReadPositiveDouble()
{
    double value = ReadDouble();

    while (value <= 0)
    {
        cout << "Введите число больше 0: ";
        value = ReadDouble();
    }

    return value;
}

int Read01()
{
    int value = ReadInt();

    while (value != 0 && value != 1)
    {
        cout << "Введите 0 или 1: ";
        value = ReadInt();
    }

    return value;
}

void Show_Menu()
{
    cout << "\nМЕНЮ:" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотреть все объекты" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

void Show_Pipe(const Pipe& p)
{
    cout << "\nТруба ID: " << p.id << endl;
    cout << "Название: " << p.name << endl;
    cout << "Длина: " << p.length << " км" << endl;
    cout << "Диаметр: " << p.diameter << " мм" << endl;
    cout << "В ремонте: " << (p.repair ? "да" : "нет") << endl;
}

void Show_CS(const CS& k)
{
    cout << "\nКС ID: " << k.id << endl;
    cout << "Название: " << k.name << endl;
    cout << "Всего цехов: " << k.workshops << endl;
    cout << "Работающих цехов: " << k.workshops_work << endl;
    cout << "Класс станции: " << k.class_cs << endl;

    double percent = 100.0 *
        (k.workshops - k.workshops_work) / k.workshops;

    cout << "Незадействованных цехов: "
        << percent << "%" << endl;
}

void Add_Pipe(vector<Pipe>& pipes, int& nextId)
{
    if (pipes.size() >= MAX_OBJECTS || nextId == INT_MAX)
    {
        cout << "Невозможно добавить трубу: достигнут предел." << endl;
        LogAction("Отказ в добавлении трубы: достигнут предел");
        return;
    }

    Pipe p;

    cout << "\nВведите название трубы: ";
    p.name = ReadName();

    cout << "Введите длину трубы (км): ";
    p.length = ReadPositiveDouble();

    cout << "Введите диаметр трубы (мм): ";
    p.diameter = ReadPositiveInt();

    cout << "В ремонте? (1 - да, 0 - нет): ";
    p.repair = Read01();

    p.id = nextId;
    nextId++;

    pipes.push_back(p);

    cout << "Труба добавлена. ID: " << p.id << endl;

    LogAction("Добавлена труба ID=" + to_string(p.id)
        + ", название=" + p.name);
}

void Add_CS(vector<CS>& stations, int& nextId)
{
    if (stations.size() >= MAX_OBJECTS || nextId == INT_MAX)
    {
        cout << "Невозможно добавить КС: достигнут предел." << endl;
        LogAction("Отказ в добавлении КС: достигнут предел");
        return;
    }

    CS k;

    cout << "\nВведите название КС: ";
    k.name = ReadName();

    cout << "Введите количество цехов: ";
    k.workshops = ReadPositiveInt();

    cout << "Введите количество работающих цехов: ";
    k.workshops_work = ReadInt();

    while (k.workshops_work < 0 ||
        k.workshops_work > k.workshops)
    {
        cout << "Введите число от 0 до "
            << k.workshops << ": ";
        k.workshops_work = ReadInt();
    }

    cout << "Введите класс станции: ";
    k.class_cs = ReadPositiveInt();

    k.id = nextId;
    nextId++;

    stations.push_back(k);

    cout << "КС добавлена. ID: " << k.id << endl;

    LogAction("Добавлена КС ID=" + to_string(k.id)
        + ", название=" + k.name);
}

void Show_All(const vector<Pipe>& pipes,
    const vector<CS>& stations)
{
    cout << "\nСПИСОК ТРУБ:" << endl;

    if (pipes.empty())
        cout << "Труб пока нет." << endl;

    for (int i = 0; i < (int)pipes.size(); i++)
        Show_Pipe(pipes[i]);

    cout << "\nСПИСОК КС:" << endl;

    if (stations.empty())
        cout << "КС пока нет." << endl;

    for (int i = 0; i < (int)stations.size(); i++)
        Show_CS(stations[i]);

    LogAction("Просмотр всех объектов");
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    vector<Pipe> pipes;
    vector<CS> stations;

    int nextId = 1;
    int choice;

    LogAction("Запуск программы");

    while (true)
    {
        Show_Menu();
        choice = ReadInt();

        LogAction("Выбран пункт меню: " + to_string(choice));

        switch (choice)
        {
        case 1:
            Add_Pipe(pipes, nextId);
            break;

        case 2:
            Add_CS(stations, nextId);
            break;

        case 3:
            Show_All(pipes, stations);
            break;

        case 0:
            LogAction("Выход из программы");
            cout << "Программа завершена." << endl;
            return 0;

        default:
            cout << "Такого пункта меню нет." << endl;
            LogAction("Неверный пункт меню");
            break;
        }
    }
}