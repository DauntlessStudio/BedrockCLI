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
