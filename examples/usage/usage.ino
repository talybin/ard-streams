#include <sstream.hpp>

void setup()
{
    int n;
    double f;
    bool b;
    std::string s;

    ard::istringstream iss("41 3.14 false helloworld");
    iss >> n >> f >> ard::boolalpha >> b >> s;
}

