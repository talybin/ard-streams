# STL iostreams for Arduino

This library includes iostreams from STL with additional wrappers for Arduino hardware.

All files in this library are copy-pasted from GCC 7.3 standard library with some modifications to avoid linking of libstdc++ that otherwise overflow the flash memory.

## Using STL streams

To avoid file and namespace conflicts, all files in this library has `.hpp` extension and `ard` namespace (instead of `std`). Otherwise the full documentation is available on [cppreference.com](https://en.cppreference.com/w/cpp/io).

There are some limitations though:

* Locale is not supported
* `wchar_t` is not supported
* File streams not implemented

Usage example:

```c++
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
```

## Using streams with serial port

The serial stream is not in standard GCC library but included as usefull part of Arduino projects.

```c++
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

```

