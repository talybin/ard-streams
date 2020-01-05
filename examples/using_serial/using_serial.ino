#include <iostream.hpp>
#include <serstream.hpp>

namespace ard
{
    ard::oserialstream cout(Serial1);
    ard::iserialstream cin(Serial1);
}

void setup()
{
    Serial1.begin(9600);
    ard::cout << "Hello World\n";
}

void loop()
{
    ard::cout << "Give me an integer: ";

    int data;
    if (ard::cin >> data)
        ard::cout << "2 + " << data << " = " << (2 + data) << '\n';
    else {
        ard::cin.clear();
        ard::cin.ignore();
    }
}

