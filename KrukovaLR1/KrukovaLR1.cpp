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

int Find_Pipe_By_ID(const vector<Pipe>& pipes, int id)
{
    for (int i = 0; i < (int)pipes.size(); i++)
        if (pipes[i].id == id)
            return i;
    return -1;
}

void Delete_Pipe(vector<Pipe>& pipes, int id)
{
    int pos = Find_Pipe_By_ID(pipes, id);
    if (pos == -1)
    {
        cout << "Объект не найден." << endl;
        LogAction("Не найден Pipe ID=" + to_string(id));
        return;
    }
    for (int i = pos; i + 1 < (int)pipes.size(); i++)
        pipes[i] = pipes[i + 1];
    pipes.pop_back();
    LogAction("Удален Pipe ID=" + to_string(id));
    cout << "Удален ID=" << id << endl;
}

int Find_CS_By_ID(const vector<CS>& cs, int id)
{
    for (int i = 0; i < (int)cs.size(); i++)
        if (cs[i].id == id)
            return i;
    return -1;
}

void Delete_CS(vector<CS>& cs, int id)
{
    int pos = Find_CS_By_ID(cs, id);
    if (pos == -1)
    {
        cout << "Объект не найден." << endl;
        LogAction("Не найден CS ID=" + to_string(id));
        return;
    }
    for (int i = pos; i + 1 < (int)cs.size(); i++)
        cs[i] = cs[i + 1];
    cs.pop_back();
    LogAction("Удален CS ID=" + to_string(id));
    cout << "Удален ID=" << id << endl;
}

void Set_Repair(Pipe& p, bool state)
{
    if (p.repair == state)
    {
        if (p.repair)
            cout << "Труба ID=" << p.id << " уже находится в ремонте." << endl;
        else
            cout << "Труба ID=" << p.id << " уже не находится в ремонте." << endl;
        LogAction("Отказ изменения трубы ID=" + to_string(p.id)
            + ": состояние ремонта уже равно " + to_string(state));
        return;
    }
    LogAction("Труба ID=" + to_string(p.id) + ": ремонт "
        + to_string(p.repair) + " -> " + to_string(state));
    p.repair = state;
    if (p.repair)
        cout << "Труба ID=" << p.id << " отправлена в ремонт." << endl;
    else
        cout << "Труба ID=" << p.id << " выведена из ремонта." << endl;
}

void Edit_Pipe(Pipe& p)
{
    cout << "\nТекущее состояние трубы:" << endl;
    Show_Pipe(p);
    cout << "1. Отправить в ремонт\n2. Убрать из ремонта\n0. Назад\n";
    switch (ReadRange("Действие с трубой: ", 0, 2))
    {
    case 1: Set_Repair(p, true); break;
    case 2: Set_Repair(p, false); break;
    case 0: return;
    }
}

void Edit_CS(CS& k)
{
    cout << "\nТекущее состояние КС:" << endl;
    Show_CS(k);
    cout << "Остановленных цехов: " << k.workshops - k.workshops_work << endl;
    cout << "1. Запустить один остановленный цех\n2. Остановить один работающий цех\n0. Назад\n";
    switch (ReadRange("Действие с КС: ", 0, 2))
    {
    case 1:
        if (k.workshops_work >= k.workshops)
        {
            cout << "Все цеха уже работают. Запуск невозможен." << endl;
            LogAction("Отказ запуска цеха КС ID=" + to_string(k.id)
                + ": все цеха уже работают");
            return;
        }
        k.workshops_work++;
        cout << "Один остановленный цех запущен." << endl;
        LogAction("Запущен цех КС ID=" + to_string(k.id));
        break;
    case 2:
        if (k.workshops_work <= 0)
        {
            cout << "Все цеха уже остановлены. Остановка невозможна." << endl;
            LogAction("Отказ остановки цеха КС ID=" + to_string(k.id)
                + ": работающих цехов нет");
            return;
        }
        k.workshops_work--;
        cout << "Один работающий цех остановлен." << endl;
        LogAction("Остановлен цех КС ID=" + to_string(k.id));
        break;
    case 0: return;
    }
    cout << "Теперь работает цехов: " << k.workshops_work << endl;
    cout << "Остановленных цехов: " << k.workshops - k.workshops_work << endl;
    LogAction("КС ID=" + to_string(k.id) + ": работает цехов="
        + to_string(k.workshops_work));
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

void Work_Pipe(vector<Pipe>& pipes)
{
    while (true)
    {
        cout << "1. Найти по ID\n2. Редактировать по ID\n3. Удалить по ID\n0. Назад\n";
        int cmd = ReadRange("Работа с Pipe: ", 0, 3);
        switch (cmd)
        {
        case 0: return;
        }
        int id = ReadRange("ID: ", 1, INT_MAX);
        int pos = Find_Pipe_By_ID(pipes, id);
        if (pos == -1)
        {
            cout << "Объект не найден." << endl;
            LogAction("Не найден Pipe ID=" + to_string(id));
            continue;
        }
        switch (cmd)
        {
        case 1: Show_Pipe(pipes[pos]); break;
        case 2: Edit_Pipe(pipes[pos]); break;
        case 3: Delete_Pipe(pipes, id); break;
        }
    }
}

void Work_CS(vector<CS>& cs)
{
    while (true)
    {
        cout << "1. Найти по ID\n2. Редактировать по ID\n3. Удалить по ID\n0. Назад\n";
        int cmd = ReadRange("Работа с CS: ", 0, 3);
        switch (cmd)
        {
        case 0: return;
        }
        int id = ReadRange("ID: ", 1, INT_MAX);
        int pos = Find_CS_By_ID(cs, id);
        if (pos == -1)
        {
            cout << "Объект не найден." << endl;
            LogAction("Не найден CS ID=" + to_string(id));
            continue;
        }
        switch (cmd)
        {
        case 1: Show_CS(cs[pos]); break;
        case 2: Edit_CS(cs[pos]); break;
        case 3: Delete_CS(cs, id); break;
        }
    }
}

void Work_Menu(vector<Pipe>& pipes, vector<CS>& cs)
{
    cout << "1. Трубы\n2. КС\n0. Назад\n";
    switch (ReadRange("Работа с объектами: ", 0, 2))
    {
    case 1: Work_Pipe(pipes); break;
    case 2: Work_CS(cs); break;
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
        case 4: Work_Menu(pipes, cs); break;
        case 0:
            LogAction("Выход из программы");
            return 0;
        }
    }
}
