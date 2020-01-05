# ElectronSTL

This library includes iostreams and some features from C++14 and C++17.

All files in this library are copy-pasted from GCC 7.3 standard library with some modifications to be compilable under C++11 and to avoid linking of libstdc++ that otherwise overflow the flash memory. Also to avoid file and namespace conflicts, all files in this library has `.hpp` extension and `ard` namespace (instead of `std`).

## Using streams with serial port

The serial stream is not in standard GCC library but included as usefull part of Arduino projects.

```c++
#include <ElectronSTL.h>

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

```

