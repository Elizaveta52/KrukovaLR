#include "Pipeline.h"
#include "Utils.h"
#include <iomanip>
#include <limits>
#include <cmath>
using namespace std;

Pipeline::Pipeline(int newId) : id(newId) {}

int Pipeline::GetID() const { return id; }
const string& Pipeline::GetName() const { return name; }
bool Pipeline::IsRepair() const { return repair; }
bool Pipeline::SetRepair(bool value)
{
    if (repair == value)
    {
        Message("Труба ID " + to_string(id) + (repair
            ? " уже находится в ремонте. Изменений нет."
            : " уже находится в работе. Изменений нет."));
        return false;
    }
    cerr << "Труба ID " << id << ": ремонт " << repair << " -> " << value << endl;
    repair = value;
    return true;
}

void Pipeline::Input()
{
    name = ReadName("Введите название трубы: ");
    length = GetCorrectNumber("Введите положительную длину трубы в км (дробная часть через точку): ",
        numeric_limits<double>::min(), numeric_limits<double>::max());
    diameter = GetCorrectNumber("Введите диаметр трубы в мм — целое положительное число: ", 1, numeric_limits<int>::max());
    repair = GetCorrectNumber("Состояние трубы: 0 — в работе, 1 — в ремонте. Введите 0 или 1: ", 0, 1) == 1;
}

ostream& operator<<(ostream& out, const Pipeline& object)
{
    out << "\nТруба ID: " << object.id << "\nНазвание: " << object.name
        << "\nДлина: " << object.length << " км\nДиаметр: " << object.diameter
        << " мм\nВ ремонте: " << (object.repair ? "да" : "нет") << endl;
    return out;
}

void Pipeline::Save(ostream& out) const
{
    out << id << ' ' << quoted(name) << ' ' << setprecision(17)
        << length << ' ' << diameter << ' ' << repair << '\n';
}

bool Pipeline::Load(istream& in)
{
    int state;
    if (!(in >> id >> quoted(name) >> length >> diameter >> state))
        return false;
    repair = state == 1;
    return id > 0 && name.find_first_not_of(" \t\r\n") != string::npos
        && isfinite(length) && length > 0 && diameter > 0
        && (state == 0 || state == 1);
}
