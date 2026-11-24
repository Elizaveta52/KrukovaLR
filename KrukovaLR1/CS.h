#pragma once
#include <string>
#include <iosfwd>

class CS
{
private:
    int id = 0;
    std::string name;
    int workshops = 0;
    int workshops_work = 0;
    int class_cs = 0;
public:
    explicit CS(int newId = 0);
    int GetID() const;
    const std::string& GetName() const;
    double GetUnusedPercent() const;
    void Input();
    bool Edit();
    friend std::ostream& operator<<(std::ostream& out, const CS& object);
    void Save(std::ostream& out) const;
    bool Load(std::istream& in);
};
