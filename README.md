# Fruityvice
api for fruityvice.com webservice which provides data for all kinds of fruit! You can use Fruityvice to find out interesting information about fruit and educate yourself
# main
```cpp
#include "Fruityvice.h"
#include <iostream>

int main() {
   Fruityvice api;

    auto fruits = api.get_fruits().then([](json::value result) {
        std::cout << result<< std::endl;
    });
    fruits.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```

