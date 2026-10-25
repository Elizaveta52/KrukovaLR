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
    cout << "\n  " << text;
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

double ReadRealRange(const string& text, double lo, double hi)
{
    cout << "\n  " << text;
    LogAction(text);
    double x = ReadDouble();
    while (x < lo || x > hi)
    {
        cout << "Введите число от " << lo << " до " << hi << ": ";
        LogAction("Число вне диапазона");
        x = ReadDouble();
    }
    return x;
}

void Show_Pipe(const Pipe& p)
{
    cout << "\n  Труба ID: " << p.id << endl;

    cout << "  Название:              " << p.name << endl;
    cout << "  Длина:                 " << p.length << " км" << endl;
    cout << "  Диаметр:               " << p.diameter << " мм" << endl;
    if (p.repair)
        cout << "  Состояние:             в ремонте" << endl;
    else
        cout << "  Состояние:             не в ремонте" << endl;

}

void Show_CS(const CS& k)
{
    cout << "\n  КС ID: " << k.id << endl;

    cout << "  Название:              " << k.name << endl;
    cout << "  Всего цехов:           " << k.workshops << endl;
    cout << "  Работающих цехов:      " << k.workshops_work << endl;
    cout << "  Остановленных цехов:   " << k.workshops - k.workshops_work << endl;
    cout << "  Класс станции:         " << k.class_cs << endl;
    cout << "  Незадействовано цехов:  " << k.IdlePercent() << "%" << endl;

}

void Show_All(const vector<Pipe>& pipes, const vector<CS>& cs)
{
    cout << "\n  ВСЕ ОБЪЕКТЫ\n" << endl;
    LogAction("Просмотр всех объектов");
    cout << "\n  ТРУБЫ: " << pipes.size() << endl;
    if (pipes.empty())
        cout << "  Труб пока нет." << endl;
    for (int i = 0; i < (int)pipes.size(); i++)
        Show_Pipe(pipes[i]);
    cout << "\n  КОМПРЕССОРНЫЕ СТАНЦИИ: " << cs.size() << endl;
    if (cs.empty())
        cout << "  КС пока нет." << endl;
    for (int i = 0; i < (int)cs.size(); i++)
        Show_CS(cs[i]);
    cout << "\n  Всего объектов: " << pipes.size() + cs.size() << endl;
}

void Add_Pipe(vector<Pipe>& pipes, int& next)
{
    cout << "\n  ДОБАВЛЕНИЕ ТРУБЫ\n" << endl;
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
    cout << "\nДобавлена труба ID=" << p.id << endl;
    LogAction("Добавлена труба ID=" + to_string(p.id));
}

void Add_CS(vector<CS>& cs, int& next)
{
    cout << "\n  ДОБАВЛЕНИЕ КС\n" << endl;
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
    cout << "\nДобавлена КС ID=" << k.id << endl;
    LogAction("Добавлена КС ID=" + to_string(k.id));
}

int Find_Pipe_By_ID(const vector<Pipe>& pipes, int id)
{
    for (int i = 0; i < (int)pipes.size(); i++)
        if (pipes[i].id == id)
            return i;
    return -1;
}

void Delete_Pipe_At(vector<Pipe>& pipes, int pos)
{
    if (pos < 0 || pos >= (int)pipes.size())
    {
        cout << "\nОбъект не найден." << endl;
        LogAction("Ошибка позиции при удалении Pipe");
        return;
    }
    int id = pipes[pos].id;
    for (int i = pos; i + 1 < (int)pipes.size(); i++)
        pipes[i] = pipes[i + 1];
    pipes.pop_back();
    LogAction("Удален Pipe ID=" + to_string(id));
    cout << "\nУдален ID=" << id << endl;
}

int Find_CS_By_ID(const vector<CS>& cs, int id)
{
    for (int i = 0; i < (int)cs.size(); i++)
        if (cs[i].id == id)
            return i;
    return -1;
}

void Delete_CS_At(vector<CS>& cs, int pos)
{
    if (pos < 0 || pos >= (int)cs.size())
    {
        cout << "\nОбъект не найден." << endl;
        LogAction("Ошибка позиции при удалении CS");
        return;
    }
    int id = cs[pos].id;
    for (int i = pos; i + 1 < (int)cs.size(); i++)
        cs[i] = cs[i + 1];
    cs.pop_back();
    LogAction("Удален CS ID=" + to_string(id));
    cout << "\nУдален ID=" << id << endl;
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
    cout << "\n  ИЗМЕНЕНИЕ ТРУБЫ\n" << endl;
    cout << "\n  Текущее состояние:" << endl;
    Show_Pipe(p);
    cout << "\n  1. Отправить в ремонт\n  2. Убрать из ремонта\n  0. Назад\n";
    switch (ReadRange("Действие с трубой: ", 0, 2))
    {
    case 1: Set_Repair(p, true); break;
    case 2: Set_Repair(p, false); break;
    case 0: return;
    }
}

void Edit_CS(CS& k)
{
    cout << "\n  ИЗМЕНЕНИЕ КС\n" << endl;
    cout << "\n  Текущее состояние:" << endl;
    Show_CS(k);
    cout << "\n  1. Запустить один остановленный цех\n  2. Остановить один работающий цех\n  0. Назад\n";
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

vector<int> Search_Pipes(const vector<Pipe>& pipes)
{
    cout << "\n  ПОИСК ТРУБ\n" << endl;
    vector<int> ids;
    cout << "\n  1. По части названия\n  2. По признаку ремонта\n  0. Назад\n";
    int cmd = ReadRange("Фильтр труб: ", 0, 2);
    string name;
    int state = 0;
    switch (cmd)
    {
    case 1: cout << "Часть названия: "; LogAction("Часть названия: "); name = ReadName(); break;
    case 2:
        cout << "  В ремонте (1 - да, 0 - нет): ";
        LogAction("Фильтр по состоянию ремонта");
        state = Read01();
        break;
    case 0: return ids;
    }
    for (int i = 0; i < (int)pipes.size(); i++)
    {
        bool match = false;
        switch (cmd)
        {
        case 1: match = pipes[i].name.find(name) != string::npos; break;
        case 2: match = pipes[i].repair == state; break;
        }
        if (match)
        {
            ids.push_back(pipes[i].id);
            Show_Pipe(pipes[i]);
        }
    }
    cout << "\nНайдено труб: " << ids.size() << endl;
    LogAction("Найдено труб: " + to_string(ids.size()));
    return ids;
}

void Search_Stations(const vector<CS>& cs)
{
    cout << "\n  ПОИСК КС\n" << endl;
    cout << "\n  1. По части названия\n  2. По диапазону процентов простоя\n  0. Назад\n";
    int cmd = ReadRange("Фильтр КС: ", 0, 2);
    string name;
    double lo = 0;
    double hi = 100;
    switch (cmd)
    {
    case 1: cout << "Часть названия: "; LogAction("Часть названия: "); name = ReadName(); break;
    case 2:
        lo = ReadRealRange("Процент от: ", 0, 100);
        hi = ReadRealRange("Процент до: ", lo, 100);
        break;
    case 0: return;
    }
    int n = 0;
    for (int i = 0; i < (int)cs.size(); i++)
    {
        bool match = false;
        switch (cmd)
        {
        case 1: match = cs[i].name.find(name) != string::npos; break;
        case 2:
            match = cs[i].IdlePercent() >= lo
                && cs[i].IdlePercent() <= hi;
            break;
        }
        if (match)
        {
            Show_CS(cs[i]);
            n++;
        }
    }
    cout << "\nНайдено КС: " << n << endl;
    LogAction("Найдено КС: " + to_string(n));
}

bool Has_ID(const vector<int>& ids, int id)
{
    for (int i = 0; i < (int)ids.size(); i++)
        if (ids[i] == id)
            return true;
    return false;
}

vector<int> Select_IDs(const vector<int>& found)
{
    cout << "\n  ВЫБОР ТРУБ\n" << endl;
    vector<int> sel;
    cout << "\n  1. Все найденные\n  2. Выбрать ID\n  0. Назад\n";
    switch (ReadRange("Выбор множества: ", 0, 2))
    {
    case 1: return found;
    case 0: return sel;
    case 2:
        while (true)
        {
            int id = ReadRange("ID трубы (0 - закончить выбор): ", 0, INT_MAX);
            if (id == 0)
                return sel;
            if (Has_ID(found, id) && !Has_ID(sel, id))
                sel.push_back(id);
            else
            {
                cout << "ID отсутствует среди найденных или уже выбран." << endl;
                LogAction("Недопустимый или повторный ID в пакете");
            }
        }
    }
    return sel;
}

void Batch_Pipes(vector<Pipe>& pipes)
{
    vector<int> found = Search_Pipes(pipes);
    if (found.empty())
        return;
    vector<int> sel = Select_IDs(found);
    if (sel.empty())
        return;
    cout << "\n  ПАКЕТНАЯ ОПЕРАЦИЯ\n" << endl;
    cout << "\n  Текущее состояние выбранных труб:" << endl;
    for (int i = 0; i < (int)sel.size(); i++)
    {
        int pos = Find_Pipe_By_ID(pipes, sel[i]);
        if (pos != -1)
            Show_Pipe(pipes[pos]);
    }
    cout << "\n  1. Отправить в ремонт\n  2. Убрать из ремонта\n  3. Удалить\n  0. Назад\n";
    int cmd = ReadRange("Пакетное действие: ", 0, 3);
    if (cmd == 0)
        return;
    for (int i = 0; i < (int)sel.size(); i++)
    {
        int pos = Find_Pipe_By_ID(pipes, sel[i]);
        if (pos == -1)
            continue;
        switch (cmd)
        {
        case 1: Set_Repair(pipes[pos], true); break;
        case 2: Set_Repair(pipes[pos], false); break;
        case 3: Delete_Pipe_At(pipes, pos); break;
        }
    }
    LogAction("Пакетная операция завершена");
}

bool Read_Label(ifstream& file, const string& expected)
{
    string label;
    return getline(file, label) && label == expected;
}

bool End_Line(ifstream& file)
{
    string rest;
    if (!getline(file, rest))
        return false;
    for (int i = 0; i < (int)rest.size(); i++)
        if (rest[i] != ' ' && rest[i] != '\t' && rest[i] != '\r')
            return false;
    return true;
}

bool Valid_Name(const string& name)
{
    if (name.empty() || name.size() > 200)
        return false;
    for (int i = 0; i < (int)name.size(); i++)
        if (name[i] != ' ' && name[i] != '\t' && name[i] != '\r')
            return true;
    return false;
}

void Save_Pipes(ofstream& file, const vector<Pipe>& pipes)
{
    file << "ТРУБЫ\n" << pipes.size() << '\n';
    for (int i = 0; i < (int)pipes.size(); i++)
    {
        const Pipe& p = pipes[i];
        file << "ID\n" << p.id << "\nНазвание\n" << p.name
            << "\nДлина (км)\n" << p.length
            << "\nДиаметр (мм)\n" << p.diameter
            << "\nВ ремонте\n" << p.repair << '\n';
    }
}

void Save_Stations(ofstream& file, const vector<CS>& cs)
{
    file << "КС\n" << cs.size() << '\n';
    for (int i = 0; i < (int)cs.size(); i++)
    {
        const CS& k = cs[i];
        file << "ID\n" << k.id << "\nНазвание\n" << k.name
            << "\nВсего цехов\n" << k.workshops
            << "\nРаботающих цехов\n" << k.workshops_work
            << "\nКласс\n" << k.class_cs << '\n';
    }
}

bool Load_Pipes(ifstream& file, vector<Pipe>& pipes, int next)
{
    int n;
    if (!Read_Label(file, "ТРУБЫ") || !(file >> n) || !End_Line(file)
        || n < 0 || n > MAX_OBJECTS)
        return false;
    for (int i = 0; i < n; i++)
    {
        Pipe p;
        int repair;
        if (!Read_Label(file, "ID") || !(file >> p.id) || !End_Line(file)
            || !Read_Label(file, "Название") || !getline(file, p.name)
            || !Read_Label(file, "Длина (км)") || !(file >> p.length) || !End_Line(file)
            || !Read_Label(file, "Диаметр (мм)") || !(file >> p.diameter) || !End_Line(file)
            || !Read_Label(file, "В ремонте") || !(file >> repair) || !End_Line(file))
            return false;
        if (p.id < 1 || p.id >= next || Find_Pipe_By_ID(pipes, p.id) != -1
            || !Valid_Name(p.name) || !isfinite(p.length) || p.length <= 0
            || p.diameter <= 0 || (repair != 0 && repair != 1))
            return false;
        p.repair = repair;
        pipes.push_back(p);
    }
    return true;
}

bool Load_Stations(ifstream& file, vector<CS>& cs,
    const vector<Pipe>& pipes, int next)
{
    int n;
    if (!Read_Label(file, "КС") || !(file >> n) || !End_Line(file)
        || n < 0 || n > MAX_OBJECTS)
        return false;
    for (int i = 0; i < n; i++)
    {
        CS k;
        if (!Read_Label(file, "ID") || !(file >> k.id) || !End_Line(file)
            || !Read_Label(file, "Название") || !getline(file, k.name)
            || !Read_Label(file, "Всего цехов") || !(file >> k.workshops) || !End_Line(file)
            || !Read_Label(file, "Работающих цехов") || !(file >> k.workshops_work) || !End_Line(file)
            || !Read_Label(file, "Класс") || !(file >> k.class_cs) || !End_Line(file))
            return false;
        if (k.id < 1 || k.id >= next || Find_CS_By_ID(cs, k.id) != -1
            || Find_Pipe_By_ID(pipes, k.id) != -1 || !Valid_Name(k.name)
            || k.workshops <= 0 || k.workshops_work < 0
            || k.workshops_work > k.workshops || k.class_cs <= 0)
            return false;
        cs.push_back(k);
    }
    return true;
}

void Save_File(const vector<Pipe>& pipes, const vector<CS>& cs, int next)
{
    cout << "\n  СОХРАНЕНИЕ\n" << endl;
    cout << "Имя файла сохранения: "; LogAction("Имя файла сохранения: "); string name = ReadName();
    ofstream file(name);
    if (!file)
    {
        cout << "Не удалось открыть файл." << endl;
        LogAction("Ошибка открытия файла сохранения");
        return;
    }
    file.precision(17);
    file << "PIPELINE 1\nСледующий ID\n" << next << '\n';
    Save_Pipes(file, pipes);
    Save_Stations(file, cs);
    file.close();
    if (!file)
    {
        cout << "Ошибка записи файла." << endl;
        LogAction("Ошибка записи файла");
        return;
    }
    cout << "Сохранено." << endl;
    LogAction("Сохранен файл: " + name);
}

void Load_File(vector<Pipe>& pipes, vector<CS>& cs, int& next)
{
    cout << "\n  ЗАГРУЗКА\n" << endl;
    cout << "Имя файла загрузки: "; LogAction("Имя файла загрузки: "); string name = ReadName();
    ifstream file(name);
    vector<Pipe> np;
    vector<CS> nc;
    int nid;
    if (!file || !Read_Label(file, "PIPELINE 1")
        || !Read_Label(file, "Следующий ID") || !(file >> nid)
        || !End_Line(file) || nid < 1
        || !Load_Pipes(file, np, nid)
        || !Load_Stations(file, nc, np, nid))
    {
        cout << "Файл недоступен или поврежден. Данные сохранены в памяти." << endl;
        LogAction("Отказ загрузки: " + name);
        return;
    }
    file >> ws;
    if (!file.eof())
    {
        cout << "Лишние данные в конце файла. Загрузка отменена." << endl;
        LogAction("Лишние данные в файле: " + name);
        return;
    }
    pipes.swap(np);
    cs.swap(nc);
    if (nid > next)
        next = nid;
    cout << "Данные заменены содержимым файла." << endl;
    LogAction("Загружен файл: " + name);
}

void Work_Pipe(vector<Pipe>& pipes)
{
    while (true)
    {
        cout << "\n  ТРУБЫ: ПОИСК И ИЗМЕНЕНИЯ\n" << endl;
        cout << "\n  1. Найти по ID\n  2. Редактировать по ID\n  3. Удалить по ID\n  4. Поиск по фильтру\n  5. Пакетные операции\n  0. Назад\n";
        int cmd = ReadRange("Выберите действие с трубами: ", 0, 5);
        switch (cmd)
        {
        case 0: return;
        case 4: Search_Pipes(pipes); continue;
        case 5: Batch_Pipes(pipes); continue;
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
        case 3: Delete_Pipe_At(pipes, pos); break;
        }
    }
}

void Work_CS(vector<CS>& cs)
{
    while (true)
    {
        cout << "\n  КС: ПОИСК И ИЗМЕНЕНИЯ\n" << endl;
        cout << "\n  1. Найти по ID\n  2. Редактировать по ID\n  3. Удалить по ID\n  4. Поиск по фильтру\n  0. Назад\n";
        int cmd = ReadRange("Выберите действие с КС: ", 0, 4);
        switch (cmd)
        {
        case 0: return;
        case 4: Search_Stations(cs); continue;
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
        case 3: Delete_CS_At(cs, pos); break;
        }
    }
}

void Work_Menu(vector<Pipe>& pipes, vector<CS>& cs)
{
    cout << "\n  РАБОТА С ОБЪЕКТАМИ\n" << endl;
    cout << "\n  1. Трубы\n  2. КС\n  0. Назад\n";
    switch (ReadRange("Работа с объектами: ", 0, 2))
    {
    case 1: Work_Pipe(pipes); break;
    case 2: Work_CS(cs); break;
    case 0: return;
    }
}

void File_Menu(vector<Pipe>& pipes, vector<CS>& cs, int& next)
{
    cout << "\n  РАБОТА С ФАЙЛАМИ\n" << endl;
    cout << "\n  1. Сохранить\n  2. Загрузить (заменить текущие объекты)\n  0. Назад\n";
    switch (ReadRange("Работа с файлами: ", 0, 2))
    {
    case 1: Save_File(pipes, cs, next); break;
    case 2: Load_File(pipes, cs, next); break;
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
        cout << "\n  ГЛАВНОЕ МЕНЮ\n" << endl;
        cout << "\n  1. Добавить трубу"
            << "\n  2. Добавить КС"
            << "\n  3. Просмотреть все объекты"
            << "\n  4. Работа с объектами"
            << "\n  5. Работа с файлами\n  0. Выход\n";
        switch (ReadRange("Главное меню: ", 0, 5))
        {
        case 1: Add_Pipe(pipes, next); break;
        case 2: Add_CS(cs, next); break;
        case 3: Show_All(pipes, cs); break;
        case 4: Work_Menu(pipes, cs); break;
        case 5: File_Menu(pipes, cs, next); break;
        case 0:
            LogAction("Выход из программы");
            return 0;
        }
    }
}
