#pragma once
#include "Pipeline.h"
#include "CS.h"
#include <map>
#include <set>

class Application
{
private:
    std::map<int, Pipeline> pipes;
    std::map<int, CS> stations;
    static int nextId;
    bool HasPipes() const;
    bool HasCS() const;
    void AddPipe();
    void AddCS();
    void ShowAll() const;
    void EditPipe();
    void EditCS();
    void DeletePipe();
    void DeleteCS();
    std::set<int> FindPipes() const;
    void FindCS() const;
    void BatchPipes();
    void Save() const;
    void Load();
public:
    void Run();
};
