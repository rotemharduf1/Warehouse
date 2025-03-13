#include "../include/Customer.h"
#include <iostream>
#include <string>
using std::string;


///////////////////////////////////////////////////////////////////////////////////
//Customer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
Customer::Customer(int id, const string &name, int locationDistance, int maxOrders):
//customerorders 
 orderCounter(0),id(id),name(name),locationDistance(locationDistance),maxOrders(maxOrders), ordersId() {}

//return name
const string& Customer:: getName() const{
    return name;
}

//return id
int Customer:: getId() const{
    return id;
}

//return locationDistance
int Customer:: getCustomerDistance() const{
    return locationDistance;
}

//return maxOrders
int Customer:: getMaxOrders() const{
   return maxOrders;
}

//return number of orders
int Customer:: getNumOrders() const{
   return ordersId.size();
}

//return true if the customer hasnt reach his max orders
bool Customer::canMakeOrder() const{
    return(ordersId.size() < unsigned (maxOrders));
}

//return orders Id
const vector<int> &Customer:: getOrdersIds() const{
    return ordersId;
}

int Customer:: addOrder(int orderId){
    if(canMakeOrder()){
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////////
//SoldierCustomer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
SoldierCustomer:: SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders):
    Customer(id,name, locationDistance, maxOrders), type(0) {}

//copy function - creates new Soldier Customer
 SoldierCustomer* SoldierCustomer::clone() const{
    //on heap$$$
    SoldierCustomer* clone = new SoldierCustomer(*this);
    return clone;
}

///////////////////////////////////////////////////////////////////////////////////
//CivilianCustomer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
CivilianCustomer:: CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders):
    Customer(id,name, locationDistance, maxOrders), type(1) {}

//copy function - creates new Civilian Customer
 CivilianCustomer* CivilianCustomer::clone() const{
    //on heap$$$
    CivilianCustomer* clone = new CivilianCustomer(*this);
    return clone;
}
