#ifndef DEPTH_H
#define DEPTH_H
#include <unordered_map>
#include <map>
#include "struc.h"
#include <iostream>


class DepthCreator
{
    public:
        std::map<int32_t,int32_t,std::greater<int>> buy;       //[{price,qty}}     
        std::map<int32_t,int32_t> sell; 

};

/*
 Singleton class 
*/
class TokenDepthCreator
{
    private:
        std::unordered_map<int32_t,DepthCreator> _tokenDepth;      //         <token,DepthCreator>
        TokenDepthCreator(){};
        TokenDepthCreator(const TokenDepthCreator&) = delete;
        TokenDepthCreator& operator=(const TokenDepthCreator&) = delete;
        TokenDepthCreator(TokenDepthCreator&&) = delete;

        
    
    public:
        static TokenDepthCreator& getInstance(){
            static TokenDepthCreator instance;                  
            return instance;
        }

        void insertOrder(Order& order)
        {
            if(order.orderMode == 0){           //buy order
                if(_tokenDepth.find(order.token) != _tokenDepth.end() &&
                  _tokenDepth[order.token].buy.find(order.price) != _tokenDepth[order.token].buy.end()){

                    _tokenDepth[order.token].buy[order.price] += order.qty;
                }
                else{
                    _tokenDepth[order.token].buy.insert({order.price,order.qty});
                }

            }
            if(order.orderMode == 1){           //sell order
                if(_tokenDepth.find(order.token) != _tokenDepth.end() &&
                  _tokenDepth[order.token].sell.find(order.price) != _tokenDepth[order.token].sell.end()){
                    
                    _tokenDepth[order.token].sell[order.price] += order.qty;
                }
                else{
                    _tokenDepth[order.token].sell.insert({order.price,order.qty});
                }
            }

        }

        void onSamePriceModify(Order& order,int32_t &old_qty)
        {
            if(order.orderMode == 0){          //buy
                _tokenDepth[order.token].buy[order.price] = _tokenDepth[order.token].buy[order.price] -old_qty + order.qty;
                if(_tokenDepth[order.token].buy[order.price]<=0){
                    _tokenDepth[order.token].buy.erase(order.price);
                }
            }
            else{                   //sell
                _tokenDepth[order.token].sell[order.price] = _tokenDepth[order.token].sell[order.price] -old_qty + order.qty;
                if(_tokenDepth[order.token].sell[order.price]<=0){
                    _tokenDepth[order.token].sell.erase(order.price);
                }
            
            }

        }

        void onDiffPriceModify(Order& order,int32_t &old_price,int32_t &old_qty)
        {
            if(order.orderMode == 0){           //buy
                _tokenDepth[order.token].buy[order.price] = _tokenDepth[order.token].buy[order.price] + order.qty;
                _tokenDepth[order.token].buy[old_price] = _tokenDepth[order.token].buy[old_price] - old_qty;
                
                if(_tokenDepth[order.token].buy[old_price]<=0){
                    _tokenDepth[order.token].buy.erase(old_price);
                }
            }
            else{                            //sell
                _tokenDepth[order.token].sell[order.price] = _tokenDepth[order.token].sell[order.price] + order.qty;
                _tokenDepth[order.token].sell[old_price] = _tokenDepth[order.token].sell[old_price] - old_qty;
                
                if(_tokenDepth[order.token].sell[old_price]<=0){
                    _tokenDepth[order.token].sell.erase(old_price);
                }
            }

        }

        void onCancelUpdate(Order& order)
        {
            if(order.orderMode == 0){    //buy
                _tokenDepth[order.token].buy[order.price] -= order.qty;
                
                if(_tokenDepth[order.token].buy[order.price]<=0){
                    _tokenDepth[order.token].buy.erase(order.price);
                }
            }
            else{      //sell
                _tokenDepth[order.token].sell[order.price] -= order.qty;
                
                if(_tokenDepth[order.token].sell[order.price]<=0){
                    _tokenDepth[order.token].sell.erase(order.price);
                }
            }
        }

        void printDepth()
        {
            std::cout<<"Buy\n\n";
            for(auto it=_tokenDepth.begin();it!=_tokenDepth.end();it++){
                std::cout<<"Token:";
                std::cout<<it->first<<std::endl;
                for(auto itr=(it->second).buy.begin();itr!=(it->second).buy.end();itr++){

                    std::cout<<itr->first<<"     "<<itr->second<<std::endl;
                }
            }
            std::cout<<"\nSell\n\n";
            for(auto it=_tokenDepth.begin();it!=_tokenDepth.end();it++){
                std::cout<<"Token:";
                std::cout<<it->first<<std::endl;
                for(auto itr=(it->second).sell.begin();itr!=(it->second).sell.end();itr++){

                    std::cout<<itr->first<<"     "<<itr->second<<std::endl;
                }
            }
            std::cout<<std::endl<<std::endl;

        }

};

#endif






