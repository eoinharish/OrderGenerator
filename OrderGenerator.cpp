#include "OrderGenerator.h"
#include "depth.h"
#include <iostream>
#include <unistd.h>


static int64_t o_no=0;

/*-----------------------------------
    Function to generate new order
-------------------------------------*/

void OrderGenerator:: newOrder()
{
    Order *neworder = new Order;
    neworder->orderNumber = ++o_no;
    neworder->token = rand()%2 + 1;
    neworder->orderMode = rand()%2;
    neworder->price = 1000 +(rand() % (1010-1000+1));
    neworder->qty = 1+(rand()%(10-1+1)); 

    // Buy/Sell price check
    int flag=1;
    for(auto it=_orderMap.begin();it!=_orderMap.end();it++)
    {   

        if(   (neworder->orderMode == 0 && (it->second).token==neworder->token && neworder->price >= (it->second).price && (it->second).orderMode ==1)  //new buy, map sell
                                                    ||
               (neworder->orderMode == 1 && (it->second).token==neworder->token && neworder->price <= (it->second).price && (it->second).orderMode ==0) //new sell, map buy
          ){  flag=0; break;}                                                                             //dont add to the map
                                                                                                        
        else{
            continue;
        }
    }
    if(flag==1){
        _orderMap.insert({neworder->orderNumber,*neworder});
        showOrder();
        TokenDepthCreator::getInstance().insertOrder(*neworder);
        TokenDepthCreator::getInstance().printDepth();
    }
    delete neworder;
    neworder=nullptr;
    

}

/*------------------------------
    Funtion to modify an order
--------------------------------*/

void OrderGenerator::modifyOrder()
{
        
    int64_t num = 1 + ( std::rand() % o_no );       //random no. bw 1 and o_no
    std::cout<<"Modify order no: "<<num<<std::endl;
    if(_orderMap.find(num) != _orderMap.end())
    {
        auto itr = _orderMap.find(num);   
        int32_t oldprice = (itr->second).price;
        int32_t oldqty = (itr->second).qty;
        
        (itr->second).price = 1000 + (rand() % (1010-1000+1));
        (itr->second).qty = 5 + (rand()%(10-5+1));

        if((itr->second).price == oldprice){
            showOrder();
            TokenDepthCreator::getInstance().onSamePriceModify(itr->second,oldqty);
        }
        else{          //different price

            int flag = 1;    //price check
            for(auto it=_orderMap.begin();it!=_orderMap.end();it++)
            {   

                if( ((itr->second).orderMode == 0 && (it->second).token==(itr->second).token && (itr->second).price >= (it->second).price && (it->second).orderMode ==1) //new buy, map sell
                                                    ||
                    ((itr->second).orderMode == 1 && (it->second).token==(itr->second).token && (itr->second).price <= (it->second).price && (it->second).orderMode ==0) //new sell, map buy
                ){  flag=0; break;}                                                                             //dont update to the map
                                                                                                        
                else{
                    continue;
                }
            }

            if(flag==1){
                showOrder();
                TokenDepthCreator::getInstance().onDiffPriceModify(itr->second,oldprice,oldqty);
            }
            
        }
    }

    else{
        std::cout<<"Order not found"<<std::endl;
    }
    TokenDepthCreator::getInstance().printDepth();

}

/*------------------------------
    Function to cancel order
---------------------------------*/
void OrderGenerator::cancelOrder()
{

    int64_t num = 1 + ( std::rand() % o_no  );
    std::cout<<"Cancel order no."<<num<<std::endl;
    if(_orderMap.find(num) != _orderMap.end())
    {   
        auto it=_orderMap.find(num);
        TokenDepthCreator::getInstance().onCancelUpdate(it->second);
        _orderMap.erase(num);
    }
    else{
        std::cout<<"Order not found"<<std::endl;
    }
    showOrder();
    TokenDepthCreator::getInstance().printDepth();
}


/*---------------------------------------------
    FUNCTION TO GENERATE ORDERS
-----------------------------------------------*/
void OrderGenerator::generateOrder(Order order)             
{ 
    order.orderNumber = ++o_no;
    order.token = rand()%2 + 1;       
    order.orderMode = rand()%2;                  //0->buy 1->sell
    order.price = 1000 +(rand() % (1010-1000+1));         // [1000,1010]
    order.qty = 1+(rand()%(10-1+1));                    // [1,10]
    
    _orderMap.insert({order.orderNumber,order});
    showOrder();
    TokenDepthCreator::getInstance().insertOrder(order);
    TokenDepthCreator::getInstance().printDepth();
    
    int n;
    while(1)
    {
        n = rand()%3;          //0->New   1->Modify     2->cancel
        std::cout<<"n:"<<n<<std::endl;
        if(n == 0) {
            std::cout<<"New Order"<<std::endl;
            newOrder();
        }
        else if(n == 1){
            modifyOrder();
        }
        else if(n == 2){
            cancelOrder();
        }

        usleep(1000000);
    }
        
}

/*---------------------------------
    Function to show orders
-----------------------------------*/
void OrderGenerator::showOrder()
{
   std::cout<<"MAP"<<std::endl;
   for(auto it=_orderMap.begin();it!=_orderMap.end();it++)
   {
       (it->second).print();
   }

}

