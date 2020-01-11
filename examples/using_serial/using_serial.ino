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

    // Wait for input for 30 seconds.
    // Note, ard::iserialstream will use this
    // timeout value
    Serial1.setTimeout(30000);
}

void loop()
{
    ard::cout << "Give me an integer: ";

    int data;
    if (ard::cin >> data)
        ard::cout << "2 + " << data << " = " << (2 + data) << '\n';
    else {
        // Clear failbit
        ard::cin.clear();
        // Drain read buffer
        ard::cin.ignore(ard::cin.rdbuf()->in_avail());
    }
}

