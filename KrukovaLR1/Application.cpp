#include "Application.h"
#include "Utils.h"
#include <fstream>
#include <filesystem>
#include <limits>
using namespace std;

int Application::nextId = 1;

template <typename Object>
int FindIndexByID(const vector<Object>& objects, int id)
{
    for (size_t i = 0; i < objects.size(); ++i)
        if (objects[i].GetID() == id)
            return static_cast<int>(i);
    return -1;
}

bool ContainsID(const vector<int>& ids, int id)
{
    for (int current : ids)
        if (current == id)
            return true;
    return false;
}

template <typename Object, typename Param>
using Filter = bool (*)(const Object&, const Param&);

template <typename Object, typename Param>
vector<int> FindByFilter(const vector<Object>& objects,
    Filter<Object, Param> filter, const Param& param)
{
    vector<int> result;
    for (const auto& object : objects)
        if (filter(object, param))
            result.push_back(object.GetID());
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
void ShowObjects(const vector<Object>& objects, const vector<int>& ids)
{
    if (ids.empty())
        Message("Объекты не найдены.");
    for (int id : ids)
    {
        int index = FindIndexByID(objects, id);
        if (index != -1)
        {
            cout << objects[index];
            cerr << "Показан объект ID " << id << endl;
        }
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
    pipes.push_back(pipe);
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
    stations.push_back(station);
    Message("Добавлена КС ID " + to_string(nextId++));
}

void Application::ShowAll() const
{
    if (HasPipes())
        for (const auto& pipe : pipes)
            cout << pipe;
    if (HasCS())
        for (const auto& station : stations)
            cout << station;
    cerr << "Просмотр всех объектов: труб " << pipes.size()
        << ", КС " << stations.size() << endl;
}

void Application::EditPipe()
{
    if (!HasPipes())
        return;
    int id = GetCorrectNumber("Введите ID трубы — целое положительное число: ", 1, numeric_limits<int>::max());
    int index = FindIndexByID(pipes, id);
    if (index == -1)
        Message("Труба с таким ID не найдена.");
    else
    {
        bool repair = GetCorrectNumber("Состояние трубы: 0 — в работе, 1 — в ремонте. Введите 0 или 1: ", 0, 1) == 1;
        if (pipes[index].SetRepair(repair))
            Message("Изменена труба ID " + to_string(id));
    }
}

void Application::EditCS()
{
    if (!HasCS())
        return;
    int id = GetCorrectNumber("Введите ID КС — целое положительное число: ", 1, numeric_limits<int>::max());
    int index = FindIndexByID(stations, id);
    if (index == -1)
        Message("КС с таким ID не найдена.");
    else
    {
        if (stations[index].Edit())
            Message("Изменена КС ID " + to_string(id));
    }
}

void Application::DeletePipe()
{
    if (!HasPipes())
        return;
    int id = GetCorrectNumber("Введите ID трубы — целое положительное число: ", 1, numeric_limits<int>::max());
    int index = FindIndexByID(pipes, id);
    if (index == -1)
        Message("Труба с таким ID не найдена.");
    else
    {
        pipes.erase(pipes.begin() + index);
        Message("Удалена труба ID " + to_string(id));
    }
}

void Application::DeleteCS()
{
    if (!HasCS())
        return;
    int id = GetCorrectNumber("Введите ID КС — целое положительное число: ", 1, numeric_limits<int>::max());
    int index = FindIndexByID(stations, id);
    if (index == -1)
        Message("КС с таким ID не найдена.");
    else
    {
        stations.erase(stations.begin() + index);
        Message("Удалена КС ID " + to_string(id));
    }
}

vector<int> Application::FindPipes() const
{
    if (!HasPipes())
        return {};
    int filter = GetCorrectNumber("Фильтр труб: 1 — название, 2 — в ремонте: ", 1, 2);
    vector<int> found;
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
    vector<int> found;
    if (filter == 1)
        found = FindByFilter(stations, CSByName, ReadName("Часть названия (с учётом регистра): "));
    else
    {
        double percent = GetCorrectNumber("Введите минимальный процент от 0 до 100 (дробная часть через точку): ", 0.0, 100.0);
        found = FindByFilter(stations, CSByPercent, percent);
    }
    ShowObjects(stations, found);
}

vector<int> SelectPipes(const vector<int>& found)
{
    int mode = GetCorrectNumber("1 — все найденные, 2 — выбрать ID, 0 — отмена: ", 0, 2);
    if (mode == 1)
        return found;
    vector<int> selected;
    if (mode == 2)
    {
        Message("Вводите ID по одному. 0 — закончить выбор.");
        while (true)
        {
            int id = GetCorrectNumber("Введите целое положительное ID из найденных или 0 для завершения выбора: ", 0, numeric_limits<int>::max());
            if (id == 0)
                break;
            if (ContainsID(found, id))
            {
                if (!ContainsID(selected, id))
                    selected.push_back(id);
            }
            else
                Message("Такого ID нет среди найденных труб.");
        }
    }
    return selected;
}

void Application::BatchPipes()
{
    vector<int> found = FindPipes();
    if (found.empty())
        return;
    vector<int> selected = SelectPipes(found);
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
        int index = FindIndexByID(pipes, id);
        if (index == -1)
            continue;
        if (action == 1)
        {
            if (pipes[index].SetRepair(repair))
                Message("Изменена труба ID " + to_string(id));
        }
        else
        {
            pipes.erase(pipes.begin() + index);
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
    for (const auto& pipe : pipes)
        pipe.Save(out);
    out << stations.size() << '\n';
    for (const auto& station : stations)
        station.Save(out);
    out.close();
    Message(out ? "Данные сохранены: " + filename : "Ошибка записи файла.");
}

bool ReadObjects(istream& in, vector<Pipeline>& pipes,
    vector<CS>& stations, int& savedNextId)
{
    string header;
    long long count;
    vector<int> ids;
    if (!(in >> header >> savedNextId) || header != "GAS_LAB_2" || savedNextId < 1)
        return false;
    if (!(in >> count) || count < 0 || count >= savedNextId)
        return false;
    for (long long i = 0; i < count; ++i)
    {
        Pipeline pipe;
        if (!pipe.Load(in) || pipe.GetID() >= savedNextId || ContainsID(ids, pipe.GetID()))
            return false;
        ids.push_back(pipe.GetID());
        pipes.push_back(pipe);
    }
    if (!(in >> count) || count < 0 || count >= savedNextId)
        return false;
    for (long long i = 0; i < count; ++i)
    {
        CS station;
        if (!station.Load(in) || station.GetID() >= savedNextId || ContainsID(ids, station.GetID()))
            return false;
        ids.push_back(station.GetID());
        stations.push_back(station);
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
    vector<Pipeline> loadedPipes;
    vector<CS> loadedStations;
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
