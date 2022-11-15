#include "utilities.hpp"

std::vector<std::string> utilities::split(const std::string& val, const char& deliminator)
{
    std::vector<std::string> result;
    std::stringstream s_stream(val);

    while (s_stream.good())
    {
        std::string substr;
        std::getline(s_stream, substr, deliminator);
        result.push_back(substr);
    }

    return result;
}

std::vector<std::string> utilities::split(std::string val, const std::string& deliminator)
{
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;

    while ((pos = val.find(deliminator)) != std::string::npos)
    {
        token = val.substr(0, pos);
        result.push_back(token);
        val.erase(0, pos + deliminator.length());
    }

    result.push_back(val);
    return result;
}

void utilities::replace_all(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;

    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}


template<class T>
void utilities::push_back_if(std::vector<T>& vector, const T& val, const bool& condition)
{
    if (condition)
    {
        vector.push_back(val);
    }
}