#pragma once
#include "Pipeline.h"
#include "CS.h"
#include <vector>

class Application
{
private:
    std::vector<Pipeline> pipes;
    std::vector<CS> stations;
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
    std::vector<int> FindPipes() const;
    void FindCS() const;
    void BatchPipes();
    void Save() const;
    void Load();
public:
    void Run();
};
