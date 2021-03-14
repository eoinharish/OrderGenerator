#ifndef STRUC_H
#define STRUC_H
#include<stdint.h>
#include<iostream>

//stdint class

struct Order
{
    int64_t orderNumber;  // Unique Id for New Orders
    int32_t token;        // Unique Token represent different symbols
    int32_t price;        // Price of Order
    int32_t qty;          // Quantity of Order
    int16_t orderMode;    // 0->Buy   1-> Sell
    int8_t orderType;

    void print()
    {
        std::cout<<"Order No."<<orderNumber<<" ";
        std::cout<<"token:"<<token<<" ";
        std::cout<<"Price:"<<price<<" ";
        std::cout<<"Qty:"<<qty<<" ";
        std::cout<<"Mode:"<<orderMode<<" ";
        std::cout<<std::endl;

    }
    
};

#endif // STRUC_H
