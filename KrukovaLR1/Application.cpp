#include "Application.h"
#include "Utils.h"
#include <fstream>
#include <filesystem>
#include <limits>
using namespace std;

int Application::nextId = 1;

template <typename Object, typename Param>
using Filter = bool (*)(const Object&, const Param&);

template <typename Object, typename Param>
set<int> FindByFilter(const map<int, Object>& objects,
    Filter<Object, Param> filter, const Param& param)
{
    set<int> result;
    for (const auto& item : objects)
        if (filter(item.second, param))
            result.insert(item.first);
    return result;
}

bool PipeByName(const Pipeline& pipe, const string& name)
{
    return pipe.GetName().find(name) != string::npos;
}
bool PipeByRepair(const Pipeline& pipe, const bool& repair)
{
    return pipe.IsRepair() == repair;
}
bool CSByName(const CS& station, const string& name)
{
    return station.GetName().find(name) != string::npos;
}
bool CSByPercent(const CS& station, const double& percent)
{
    return station.GetUnusedPercent() >= percent;
}

template <typename Object>
void ShowObjects(const map<int, Object>& objects, const set<int>& ids)
{
    if (ids.empty())
        Message("Объекты не найдены.");
    for (int id : ids)
    {
        cout << objects.at(id);
        cerr << "Показан объект ID " << id << endl;
    }
}

bool Application::HasPipes() const
{
    if (pipes.empty())
    {
        Message("Трубы ещё не заданы.");
        return false;
    }
    return true;
}

bool Application::HasCS() const
{
    if (stations.empty())
    {
        Message("КС ещё не заданы.");
        return false;
    }
    return true;
}

void Application::AddPipe()
{
    if (nextId == numeric_limits<int>::max())
    {
        Message("Свободные ID закончились.");
        return;
    }
    Pipeline pipe(nextId);
    pipe.Input();
    pipes.emplace(nextId, pipe);
    Message("Добавлена труба ID " + to_string(nextId++));
}

void Application::AddCS()
{
    if (nextId == numeric_limits<int>::max())
    {
        Message("Свободные ID закончились.");
        return;
    }
    CS station(nextId);
    station.Input();
    stations.emplace(nextId, station);
    Message("Добавлена КС ID " + to_string(nextId++));
}

void Application::ShowAll() const
{
    if (HasPipes())
        for (const auto& item : pipes)
            cout << item.second;
    if (HasCS())
        for (const auto& item : stations)
            cout << item.second;
    cerr << "Просмотр всех объектов: труб " << pipes.size()
        << ", КС " << stations.size() << endl;
}

void Application::EditPipe()
{
    if (!HasPipes())
        return;
    int id = GetCorrectNumber("Введите ID трубы — целое положительное число: ", 1, numeric_limits<int>::max());
    auto it = pipes.find(id);
    if (it == pipes.end())
        Message("Труба с таким ID не найдена.");
    else
    {
        bool repair = GetCorrectNumber("Состояние трубы: 0 — в работе, 1 — в ремонте. Введите 0 или 1: ", 0, 1) == 1;
        if (it->second.SetRepair(repair))
            Message("Изменена труба ID " + to_string(id));
    }
}

void Application::EditCS()
{
    if (!HasCS())
        return;
    int id = GetCorrectNumber("Введите ID КС — целое положительное число: ", 1, numeric_limits<int>::max());
    auto it = stations.find(id);
    if (it == stations.end())
        Message("КС с таким ID не найдена.");
    else
    {
        if (it->second.Edit())
            Message("Изменена КС ID " + to_string(id));
    }
}

void Application::DeletePipe()
{
    if (!HasPipes())
        return;
    int id = GetCorrectNumber("Введите ID трубы — целое положительное число: ", 1, numeric_limits<int>::max());
    if (pipes.erase(id))
        Message("Удалена труба ID " + to_string(id));
    else
        Message("Труба с таким ID не найдена.");
}

void Application::DeleteCS()
{
    if (!HasCS())
        return;
    int id = GetCorrectNumber("Введите ID КС — целое положительное число: ", 1, numeric_limits<int>::max());
    if (stations.erase(id))
        Message("Удалена КС ID " + to_string(id));
    else
        Message("КС с таким ID не найдена.");
}

set<int> Application::FindPipes() const
{
    if (!HasPipes())
        return {};
    int filter = GetCorrectNumber("Фильтр труб: 1 — название, 2 — в ремонте: ", 1, 2);
    set<int> found;
    if (filter == 1)
        found = FindByFilter(pipes, PipeByName, ReadName("Часть названия (с учётом регистра): "));
    else
    {
        bool repair = GetCorrectNumber("Состояние трубы: 0 — в работе, 1 — в ремонте. Введите 0 или 1: ", 0, 1) == 1;
        found = FindByFilter(pipes, PipeByRepair, repair);
    }
    ShowObjects(pipes, found);
    return found;
}

void Application::FindCS() const
{
    if (!HasCS())
        return;
    int filter = GetCorrectNumber("Фильтр КС: 1 — название, 2 — процент незадействованных цехов: ", 1, 2);
    set<int> found;
    if (filter == 1)
        found = FindByFilter(stations, CSByName, ReadName("Часть названия (с учётом регистра): "));
    else
    {
        double percent = GetCorrectNumber("Введите минимальный процент от 0 до 100 (дробная часть через точку): ", 0.0, 100.0);
        found = FindByFilter(stations, CSByPercent, percent);
    }
    ShowObjects(stations, found);
}

set<int> SelectPipes(const set<int>& found)
{
    int mode = GetCorrectNumber("1 — все найденные, 2 — выбрать ID, 0 — отмена: ", 0, 2);
    if (mode == 1)
        return found;
    set<int> selected;
    if (mode == 2)
    {
        Message("Вводите ID по одному. 0 — закончить выбор.");
        while (true)
        {
            int id = GetCorrectNumber("Введите целое положительное ID из найденных или 0 для завершения выбора: ", 0, numeric_limits<int>::max());
            if (id == 0)
                break;
            if (found.count(id))
                selected.insert(id);
            else
                Message("Такого ID нет среди найденных труб.");
        }
    }
    return selected;
}

void Application::BatchPipes()
{
    set<int> found = FindPipes();
    if (found.empty())
        return;
    set<int> selected = SelectPipes(found);
    if (selected.empty())
    {
        Message("Трубы не выбраны. Изменений нет.");
        return;
    }
    int action = GetCorrectNumber("1 — изменить ремонт, 2 — удалить, 0 — отмена: ", 0, 2);
    if (action == 0)
        return;
    bool repair = false;
    if (action == 1)
        repair = GetCorrectNumber("Новое состояние труб: 0 — в работе, 1 — в ремонте. Введите 0 или 1: ", 0, 1) == 1;
    for (int id : selected)
    {
        if (action == 1)
        {
            if (pipes.at(id).SetRepair(repair))
                Message("Изменена труба ID " + to_string(id));
        }
        else
        {
            pipes.erase(id);
            Message("Удалена труба ID " + to_string(id));
        }
    }
}

void Application::Save() const
{
    HasPipes();
    HasCS();
    string filename = ReadName("Имя файла для сохранения: ");
    error_code error;
    if (filesystem::equivalent(filename, "log.txt", error))
    {
        Message("Нельзя сохранять данные в файл журнала log.txt.");
        return;
    }
    ofstream out(filename);
    if (!out)
    {
        Message("Ошибка открытия файла для записи.");
        return;
    }
    out << "GAS_LAB_2\n" << nextId << '\n' << pipes.size() << '\n';
    for (const auto& item : pipes)
        item.second.Save(out);
    out << stations.size() << '\n';
    for (const auto& item : stations)
        item.second.Save(out);
    out.close();
    Message(out ? "Данные сохранены: " + filename : "Ошибка записи файла.");
}

bool ReadObjects(istream& in, map<int, Pipeline>& pipes,
    map<int, CS>& stations, int& savedNextId)
{
    string header;
    long long count;
    set<int> ids;
    if (!(in >> header >> savedNextId) || header != "GAS_LAB_2" || savedNextId < 1)
        return false;
    if (!(in >> count) || count < 0 || count >= savedNextId)
        return false;
    for (long long i = 0; i < count; ++i)
    {
        Pipeline pipe;
        if (!pipe.Load(in) || pipe.GetID() >= savedNextId || !ids.insert(pipe.GetID()).second)
            return false;
        pipes.emplace(pipe.GetID(), pipe);
    }
    if (!(in >> count) || count < 0 || count >= savedNextId)
        return false;
    for (long long i = 0; i < count; ++i)
    {
        CS station;
        if (!station.Load(in) || station.GetID() >= savedNextId || !ids.insert(station.GetID()).second)
            return false;
        stations.emplace(station.GetID(), station);
    }
    in >> ws;
    return in.eof();
}

void Application::Load()
{
    string filename = ReadName("Имя файла для загрузки (заменит текущие объекты): ");
    ifstream in(filename);
    if (!in)
    {
        Message("Ошибка открытия файла для чтения.");
        return;
    }
    map<int, Pipeline> loadedPipes;
    map<int, CS> loadedStations;
    int savedNextId;
    if (!ReadObjects(in, loadedPipes, loadedStations, savedNextId))
    {
        Message("Неверный формат файла. Текущие данные сохранены.");
        return;
    }
    pipes.swap(loadedPipes);
    stations.swap(loadedStations);
    if (savedNextId > nextId)
        nextId = savedNextId;
    Message("Данные загружены: " + filename);
}

void Application::Run()
{
    while (true)
    {
        cout << "\nМЕНЮ:\n1. Добавить трубу\n2. Добавить КС"
            << "\n3. Просмотреть все объекты\n4. Редактировать трубу"
            << "\n5. Редактировать КС\n6. Сохранить\n7. Загрузить"
            << "\n8. Удалить трубу\n9. Удалить КС\n10. Поиск труб"
            << "\n11. Поиск КС\n12. Пакетное изменение или удаление труб\n0. Выход\n";
        int choice = GetCorrectNumber("Введите целый номер пункта меню от 0 до 12: ", 0, 12);
        cerr << "Пункт меню: " << choice << endl;
        switch (choice)
        {
        case 1: AddPipe(); break;
        case 2: AddCS(); break;
        case 3: ShowAll(); break;
        case 4: EditPipe(); break;
        case 5: EditCS(); break;
        case 6: Save(); break;
        case 7: Load(); break;
        case 8: DeletePipe(); break;
        case 9: DeleteCS(); break;
        case 10: FindPipes(); break;
        case 11: FindCS(); break;
        case 12: BatchPipes(); break;
        case 0: Message("Программа завершена."); return;
        }
    }
}
