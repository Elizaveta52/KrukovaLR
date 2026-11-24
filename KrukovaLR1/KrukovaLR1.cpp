#include "Application.h"
#include "Utils.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
    ofstream logfile("log.txt", ios::app);
    if (!logfile)
    {
        cout << "Не удалось открыть log.txt. Запуск без логирования невозможен." << endl;
        return 1;
    }
    redirect_output_wrapper cerr_out(cerr);
    cerr_out.redirect(logfile);
    time_t now = time(nullptr);
    tm local{};
#ifdef _WIN32
    bool timeReady = localtime_s(&local, &now) == 0;
#else
    bool timeReady = localtime_r(&now, &local) != nullptr;
#endif
    if (timeReady)
        cerr << "\nНачало сеанса: " << put_time(&local, "%Y-%m-%d %H:%M:%S") << endl;
    try
    {
        Application app;
        app.Run();
    }
    catch (const exception& error)
    {
        Message(error.what());
    }
    cerr << "Конец сеанса." << endl;
    return 0;
}
