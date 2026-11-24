#include "CS.h"
#include "Utils.h"
#include <iomanip>
#include <limits>
using namespace std;

CS::CS(int newId) : id(newId) {}

int CS::GetID() const { return id; }
const string& CS::GetName() const { return name; }
double CS::GetUnusedPercent() const
{
    return workshops == 0 ? 0 : 100.0 * (workshops - workshops_work) / workshops;
}

void CS::Input()
{
    name = ReadName("Введите название КС: ");
    workshops = GetCorrectNumber("Введите общее количество цехов — целое положительное число: ", 1, numeric_limits<int>::max());
    workshops_work = GetCorrectNumber("Введите количество работающих цехов — целое число от 0 до " + to_string(workshops) + ": ", 0, workshops);
    class_cs = GetCorrectNumber("Введите класс станции — целое положительное число: ", 1, numeric_limits<int>::max());
}

bool CS::Edit()
{
    cout << "Сейчас работает цехов: " << workshops_work << " из " << workshops << endl;
    int newCount = GetCorrectNumber("Введите новое количество работающих цехов — целое число от 0 до "
        + to_string(workshops) + ": ", 0, workshops);
    if (newCount == workshops_work)
    {
        if (workshops_work == workshops)
            Message("Все цеха КС ID " + to_string(id) + " уже запущены. Изменений нет.");
        else if (workshops_work == 0)
            Message("Все цеха КС ID " + to_string(id) + " уже остановлены. Изменений нет.");
        else
            Message("На КС ID " + to_string(id) + " уже работает указанное количество цехов. Изменений нет.");
        return false;
    }
    cerr << "КС ID " << id << ": работающих цехов " << workshops_work << " -> " << newCount << endl;
    workshops_work = newCount;
    return true;
}

ostream& operator<<(ostream& out, const CS& object)
{
    out << "\nКС ID: " << object.id << "\nНазвание: " << object.name
        << "\nКоличество цехов: " << object.workshops
        << "\nРаботает цехов: " << object.workshops_work
        << "\nКласс станции: " << object.class_cs
        << "\nНезадействованных цехов: " << object.GetUnusedPercent() << "%" << endl;
    return out;
}

void CS::Save(ostream& out) const
{
    out << id << ' ' << quoted(name) << ' ' << workshops << ' '
        << workshops_work << ' ' << class_cs << '\n';
}

bool CS::Load(istream& in)
{
    if (!(in >> id >> quoted(name) >> workshops >> workshops_work >> class_cs))
        return false;
    return id > 0 && name.find_first_not_of(" \t\r\n") != string::npos
        && workshops > 0 && workshops_work >= 0
        && workshops_work <= workshops && class_cs > 0;
}
