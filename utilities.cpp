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


template<class T>
void utilities::push_back_if(std::vector<T>& vector, const T& val, const bool& condition)
{
    if (condition)
    {
        vector.push_back(val);
    }
}