#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <cmath>
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

    double IdlePercent() const
    {
        return 100.0 * (workshops - workshops_work) / workshops;
    }
};

void LogAction(const string& action)
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

bool EndInput()
{
    string rest;
    getline(cin, rest);
    LogAction("Остаток ввода: " + rest);
    for (int i = 0; i < (int)rest.size(); i++)
        if (rest[i] != ' ' && rest[i] != '\t' && rest[i] != '\r')
            return false;
    return true;
}

int ReadInt()
{
    int x;
    while (true)
    {
        if (cin >> x)
        {
            LogAction("Введено целое число: " + to_string(x));
            if (EndInput())
                return x;
        }
        else
        {
            if (cin.eof())
            {
                LogAction("Завершение ввода");
                exit(0);
            }
            cin.clear();
            LogAction("Ошибка чтения числа");
            cin.ignore(INT_MAX, '\n');
        }
        if (cin.eof())
        {
            LogAction("Завершение ввода");
            exit(0);
        }
        cout << "Ошибка. Введите целое число ещё раз: ";
        LogAction("Ошибка ввода целого числа");
    }
}

double ReadDouble()
{
    double x;
    while (true)
    {
        if (cin >> x)
        {
            LogAction("Введено число: " + to_string(x));
            bool ok = EndInput();
            if (ok && isfinite(x))
                return x;
        }
        else
        {
            if (cin.eof())
            {
                LogAction("Завершение ввода");
                exit(0);
            }
            cin.clear();
            LogAction("Ошибка чтения числа");
            cin.ignore(INT_MAX, '\n');
        }
        if (cin.eof())
        {
            LogAction("Завершение ввода");
            exit(0);
        }
        cout << "Ошибка. Введите число ещё раз: ";
        LogAction("Ошибка ввода числа");
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
        LogAction("Введена строка: " + name);
        if (!name.empty() && name.size() <= 200)
            return name;
        cout << "Введите название длиной от 1 до 200 байт: ";
        LogAction("Ошибка ввода названия");
    }
}

int ReadPositiveInt()
{
    int x = ReadInt();
    while (x <= 0)
    {
        cout << "Введите число больше 0: ";
        LogAction("Ожидалось положительное целое число");
        x = ReadInt();
    }
    return x;
}

double ReadPositiveDouble()
{
    double x = ReadDouble();
    while (x <= 0)
    {
        cout << "Введите число больше 0: ";
        LogAction("Недопустимая длина трубы");
        x = ReadDouble();
    }
    return x;
}

int Read01()
{
    int x = ReadInt();
    while (x != 0 && x != 1)
    {
        cout << "Введите 0 или 1: ";
        LogAction("Ожидалось 0 или 1");
        x = ReadInt();
    }
    return x;
}

int ReadRange(const string& text, int lo, int hi)
{
    cout << text;
    LogAction(text);
    int x = ReadInt();
    while (x < lo || x > hi)
    {
        cout << "Введите число от " << lo << " до " << hi << ": ";
        LogAction("Число вне диапазона");
        x = ReadInt();
    }
    return x;
}

void Show_Pipe(const Pipe& p)
{
    cout << "\nТруба ID: " << p.id << endl;
    cout << "Название: " << p.name << endl;
    cout << "Длина: " << p.length << " км" << endl;
    cout << "Диаметр: " << p.diameter << " мм" << endl;
    if (p.repair)
        cout << "В ремонте: да" << endl;
    else
        cout << "В ремонте: нет" << endl;
}

void Show_CS(const CS& k)
{
    cout << "\nКС ID: " << k.id << endl;
    cout << "Название: " << k.name << endl;
    cout << "Всего цехов: " << k.workshops << endl;
    cout << "Работает цехов: " << k.workshops_work << endl;
    cout << "Класс: " << k.class_cs << endl;
    cout << "Незадействованных цехов: " << k.IdlePercent() << "%" << endl;
}

void Show_Pipes(const vector<Pipe>& pipes)
{
    LogAction("Просмотр труб");
    if (pipes.empty())
        cout << "Труб пока нет." << endl;
    for (int i = 0; i < (int)pipes.size(); i++)
        Show_Pipe(pipes[i]);
}

void Show_Stations(const vector<CS>& cs)
{
    LogAction("Просмотр КС");
    if (cs.empty())
        cout << "КС пока нет." << endl;
    for (int i = 0; i < (int)cs.size(); i++)
        Show_CS(cs[i]);
}

void Show_All(const vector<Pipe>& pipes, const vector<CS>& cs)
{
    Show_Pipes(pipes);
    Show_Stations(cs);
}

void Add_Pipe(vector<Pipe>& pipes, int& next)
{
    if (pipes.size() >= MAX_OBJECTS || next == INT_MAX)
    {
        cout << "Достигнут предел добавления труб." << endl;
        LogAction("Отказ добавления трубы: предел");
        return;
    }
    Pipe p;
    cout << "Название трубы: ";
    LogAction("Название трубы: ");
    p.name = ReadName();
    cout << "Длина трубы (км): ";
    LogAction("Ввод длины трубы");
    p.length = ReadPositiveDouble();
    cout << "Диаметр трубы (мм): ";
    LogAction("Ввод диаметра трубы");
    p.diameter = ReadPositiveInt();
    cout << "В ремонте (1 - да, 0 - нет): ";
    LogAction("Ввод состояния трубы");
    p.repair = Read01();
    p.id = next;
    pipes.push_back(p);
    next++;
    cout << "Добавлена труба ID=" << p.id << endl;
    LogAction("Добавлена труба ID=" + to_string(p.id));
}

void Add_CS(vector<CS>& cs, int& next)
{
    if (cs.size() >= MAX_OBJECTS || next == INT_MAX)
    {
        cout << "Достигнут предел добавления КС." << endl;
        LogAction("Отказ добавления КС: предел");
        return;
    }
    CS k;
    cout << "Название КС: ";
    LogAction("Название КС: ");
    k.name = ReadName();
    cout << "Всего цехов: ";
    LogAction("Ввод количества цехов");
    k.workshops = ReadPositiveInt();
    k.workshops_work = ReadRange("Работающих цехов: ", 0, k.workshops);
    cout << "Класс станции: ";
    LogAction("Ввод класса станции");
    k.class_cs = ReadPositiveInt();
    k.id = next;
    cs.push_back(k);
    next++;
    cout << "Добавлена КС ID=" << k.id << endl;
    LogAction("Добавлена КС ID=" + to_string(k.id));
}

void Show_Menu(const vector<Pipe>& pipes, const vector<CS>& cs)
{
    cout << "1. Трубы\n2. КС\n3. Все объекты\n0. Назад\n";
    switch (ReadRange("Просмотреть: ", 0, 3))
    {
    case 1: Show_Pipes(pipes); break;
    case 2: Show_Stations(cs); break;
    case 3: Show_All(pipes, cs); break;
    case 0: return;
    }
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    vector<Pipe> pipes;
    vector<CS> cs;
    int next = 1;
    LogAction("Запуск программы");
    while (true)
    {
        cout << "\n1. Добавить трубу"
            << "\n2. Добавить КС"
            << "\n3. Просмотреть объекты"
            << "\n4. Работа с объектами"
            << "\n5. Работа с файлами\n0. Выход\n";
        switch (ReadRange("Главное меню: ", 0, 5))
        {
        case 1: Add_Pipe(pipes, next); break;
        case 2: Add_CS(cs, next); break;
        case 3: Show_Menu(pipes, cs); break;
        case 0:
            LogAction("Выход из программы");
            return 0;
        }
    }
}
