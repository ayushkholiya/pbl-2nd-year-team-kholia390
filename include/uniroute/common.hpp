#ifndef UNIROUTE_COMMON_HPP
#define UNIROUTE_COMMON_HPP

#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace uniroute {

using std::cin;
using std::getline;
using std::size_t;
using std::string;
using std::stringstream;
using std::streamsize;
using std::numeric_limits;

extern bool inputClosed;
extern const string STUDENT_FILE;
extern const string LOG_FILE;

inline string toLower(string value)
{
    for (size_t i = 0; i < value.size(); i++)
        value[i] = static_cast<char>(
            std::tolower(static_cast<unsigned char>(value[i])));
    return value;
}

inline bool splitLine(const string& line, string parts[], int n)
{
    stringstream stream(line);
    string item;
    int count = 0;

    while (getline(stream, item, '|'))
    {
        if (count < n)
            parts[count] = item;
        count++;
    }

    return count == n;
}

inline bool readInt(int& value)
{
    if (!(cin >> value))
    {
        if (cin.eof())
            inputClosed = true;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

inline bool readLine(string& text)
{
    if (!getline(cin, text))
    {
        inputClosed = true;
        return false;
    }

    return true;
}

} // namespace uniroute

#endif
