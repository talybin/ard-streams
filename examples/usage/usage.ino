#include <iostream.hpp>
#include <sstream.hpp>

void setup()
{
    std::string name;
    int version;

    ard::istringstream iss("ard-streams 1.0.0");
    iss >> name >> version;
}

