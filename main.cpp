#include <iostream>
#include <unistd.h>
#include "OrderGenerator.h"
#include "depth.h"
#include <thread>


int main()
{
    srand(getpid());
    Order order;
    OrderGenerator og;
    
    std::thread generate(&OrderGenerator::generateOrder,&og,order);
        
    generate.join();

 
}

