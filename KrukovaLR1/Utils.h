#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
    redirect_output_wrapper(std::ostream& src)
        : stream(src), old_buf(src.rdbuf()) {}
    ~redirect_output_wrapper() { stream.rdbuf(old_buf); }
    void redirect(std::ostream& dest) { stream.rdbuf(dest.rdbuf()); }
};

std::string ReadLine(const std::string& prompt);
std::string ReadName(const std::string& prompt);
void Message(const std::string& text);

template <typename T>
T GetCorrectNumber(const std::string& prompt, T min, T max)
{
    while (true)
    {
        std::istringstream input(ReadLine(prompt));
        T value;
        if ((input >> value) && (input >> std::ws).eof()
            && value >= min && value <= max)
            return value;
        Message("Ошибка: неверный тип числа, лишние символы или выход за допустимый диапазон.");
        std::cout << "Допустимый диапазон: от " << min << " до " << max << "." << std::endl;
        std::cerr << "Допустимый диапазон: от " << min << " до " << max << "." << std::endl;
    }
}
