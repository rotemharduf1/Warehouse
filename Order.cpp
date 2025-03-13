#include "../include/Order.h"
#include <iostream>
#include <string>
using std::string;


///////////////////////////////////////////////////////////////////////////////////
//Order class
///////////////////////////////////////////////////////////////////////////////////

//constructor
Order:: Order(int id, int customerId, int distance):
    FinishStep(false),id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), 
    collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){}

//return id
int Order:: getId() const{
    return id;
}

//return customerId
int Order:: getCustomerId() const{
    return customerId;
}

//set status
void Order:: setStatus(OrderStatus status){
    this->status = status;
}

//set collectorId
void Order:: setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

//set driverId
void Order:: setDriverId(int driverId){
    this->driverId = driverId;
}

//return Collector Id
int Order:: getCollectorId() const{
    return collectorId;
}

//return driver Id
int Order:: getDriverId() const{
    return driverId;
}

//return status
OrderStatus Order:: getStatus() const{
    return status;
}

//printing
const string Order:: toString() const{
    string str = "OrderId: "+  std::to_string(id) + "\n"+
    "OrderStatus: " + OrderStatusEnumToString(status) + "\n"+
    "CustomerId: " + std::to_string(customerId) + "\n"+
    "Collector: " + std::to_string(collectorId) + "\n"+
    "Driver: " + std::to_string(driverId);
    return str;
}

//return distance
int Order:: getDistance() const{
    return distance;
}

string Order::OrderStatusEnumToString(OrderStatus orderStatus)const {
    if(orderStatus==OrderStatus::PENDING){
        return "Pending";
    }
    else if (orderStatus==OrderStatus::COLLECTING){
        return "Collecting";
    }
    else if(orderStatus==OrderStatus::DELIVERING){
        return "Delivering";
    }
    else{
        return "Completed";
    }
}

// making a copy of the order
Order* Order::clone() const{
    Order* clone = new Order(*this);
    return clone;
}