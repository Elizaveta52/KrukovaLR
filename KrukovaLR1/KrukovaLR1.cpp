
#include <iostream>
#include <string>
using namespace std;

struct Pipe
{
    string name;
    double length;
    int diameter;
    bool repair;
};
void Add_Pipe(Pipe& p)
{
    cout << "Enter pipe name: ";
    cin >> p.name;
    cout << "Enter pipe length (km): ";
    cin >> p.length;
    cout << "Enter pipe diameter (mm): ";
    cin >> p.diameter;
    p.repair = false;
}

int main()
{
    Pipe p;
    Add_Pipe(p);
    return 0;
}

