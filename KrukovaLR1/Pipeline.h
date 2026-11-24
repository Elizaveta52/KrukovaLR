#pragma once
#include <string>
#include <iosfwd>

class Pipeline
{
private:
    int id = 0;
    std::string name;
    double length = 0;
    int diameter = 0;
    bool repair = false;
public:
    explicit Pipeline(int newId = 0);
    int GetID() const;
    const std::string& GetName() const;
    bool IsRepair() const;
    bool SetRepair(bool value);
    void Input();
    friend std::ostream& operator<<(std::ostream& out, const Pipeline& object);
    void Save(std::ostream& out) const;
    bool Load(std::istream& in);
};
