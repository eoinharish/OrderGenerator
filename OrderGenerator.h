#ifndef OG_H
#define OG_H

#include "struc.h"
#include <unordered_map>


class OrderGenerator
{
    private:
        std::unordered_map<int64_t,Order> _orderMap;    // { O_no , order }
    
    public:
        void generateOrder(Order);
        void newOrder();
        void modifyOrder();
        void cancelOrder();
        void showOrder();
};

#endif
