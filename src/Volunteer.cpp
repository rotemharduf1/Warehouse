#include "../include/Volunteer.h"
#include <iostream>
#include <string>
using std::string;

///////////////////////////////////////////////////////////////////////////////////
//Volunteer class
///////////////////////////////////////////////////////////////////////////////////


//constructor
Volunteer::Volunteer(int id, const string &name):
   completedOrderId(NO_ORDER),activeOrderId(NO_ORDER), id(id), name(name) {}



//get volunteer's ID
int Volunteer::getId() const{
    return id;
}

//get volunteer's name
const string& Volunteer:: getName() const{
    return name;
}

//get volunteer's active order
int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

//get volunteer's completed order
int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

//check if the volunteer takes care of an order or not
bool Volunteer:: isBusy() const {
    return (activeOrderId!=NO_ORDER);
}

void Volunteer::setActiveOrderId(int num){
    activeOrderId = num;
}


///////////////////////////////////////////////////////////////////////////////////
//CollectorVolunteer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
CollectorVolunteer:: CollectorVolunteer(int id, const string &name, int coolDown):
Volunteer(id,name),coolDown(coolDown),timeLeft(coolDown) {}

//copy function - creates new collector volunteer
CollectorVolunteer* CollectorVolunteer:: clone() const{
    //on heap$$$
    CollectorVolunteer* clone = new CollectorVolunteer (*this);
    return clone;
}

//decrease time left by 1
void CollectorVolunteer:: step(){
    timeLeft=timeLeft-1;

}

//get cooldown
int CollectorVolunteer:: getCoolDown() const{
    return coolDown;
}

//get time left for the current order
int CollectorVolunteer:: getTimeLeft() const{
    return timeLeft;
}

//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
bool CollectorVolunteer:: decreaseCoolDown(){
    timeLeft-=1;
    return (timeLeft==0);
}

//not limited, always can get new orders
bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}

//order can be taken by this collector if its panding and havent been proccessed by
//other collector, and only if this collector is not busy
bool CollectorVolunteer:: canTakeOrder(const Order &order) const{
    if(order.getStatus() != OrderStatus(0))
        return false;
    else if (order.getCollectorId() != NO_VOLUNTEER)
        return false;
    if(isBusy())
        return false;
    return true;
}

//nullify fields before getting a new order
void CollectorVolunteer:: acceptOrder(const Order &order){
    //this->activeOrder = new Order(-1,-1,-1);
    this->activeOrderId = NO_ORDER;
    this->timeLeft = coolDown;
    this->completedOrderId = order.getId();
}

//to string
string CollectorVolunteer:: toString() const{
    string str = "Id: " + std::to_string(this->getId()) +"\n"+
    "Name: " + this->getName() +"\n"+
    "Is this collector busy now? " + std::to_string(this->isBusy()) +"\n"+
    "Cool Down: " + std::to_string(this->coolDown) +"\n"+
    "Time Left: " + std::to_string(this->timeLeft);
    return str;
}



///////////////////////////////////////////////////////////////////////////////////
//LimitedCollectorVolunteer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
LimitedCollectorVolunteer :: LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):
    CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

//copy function - creates new limited collector volunteer
LimitedCollectorVolunteer* LimitedCollectorVolunteer :: clone() const {
    //on heap$$$
    LimitedCollectorVolunteer* clone = new LimitedCollectorVolunteer(*this);
    return clone;
}

//return true if the collctor hasn't reached to his maxOrders
bool LimitedCollectorVolunteer:: hasOrdersLeft() const{
    return(ordersLeft > 0);
}

//order can be taken by this collector if its panding and havent been proccessed by
//other collector, and only if this collector is not busy or hasn't reached to his maxOrders
bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const{
    if(!hasOrdersLeft())
        return false;
    if(order.getStatus() != OrderStatus::PENDING)
        return false;
    else if (order.getCollectorId() != NO_VOLUNTEER)
        return false;
    if(isBusy())
        return false;
    return true;
}

//call the function from the CollectorVolunteer class and decrease ordersLeft by 1
void LimitedCollectorVolunteer:: acceptOrder(const Order &order){
    CollectorVolunteer:: acceptOrder(order);
}

//return maxOrders
int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

//return number of Orders Left
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

//to string
string LimitedCollectorVolunteer:: toString() const{
    string str = this->toString() + "\n"+
    "Max Orders: " + std::to_string(this->maxOrders) + "\n"+
    "Orders Left: " + std::to_string(this->ordersLeft);
    return str;
}

void LimitedCollectorVolunteer::setOrdersLeft(){
    this->ordersLeft =  ordersLeft-1;
}


///////////////////////////////////////////////////////////////////////////////////
//DriverVolunteer class
///////////////////////////////////////////////////////////////////////////////////

//Constructor
DriverVolunteer:: DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
    Volunteer(id,name),maxDistance(maxDistance),distancePerStep(distancePerStep), distanceLeft(maxDistance) {}

//copy function - creates new driver volunteer
DriverVolunteer * DriverVolunteer:: clone() const{
    //on heap$$$
    DriverVolunteer* clone = new DriverVolunteer(*this);
    return clone;
}

//return distanceLeft
int DriverVolunteer:: getDistanceLeft() const {
    return this->distanceLeft;
}

//return maxDistance
int DriverVolunteer:: getMaxDistance() const {
    return maxDistance;
}

//return DistancePerStep
int DriverVolunteer:: getDistancePerStep() const {
    return distancePerStep;
}

//decrease distance left by distance per step
//return true if the driver reach his destination
bool DriverVolunteer:: decreaseDistanceLeft(){
    this->distanceLeft=(this->distanceLeft)-(this->distancePerStep);
    if(this->distanceLeft<0){
        this->distanceLeft = 0;
    }
    return (distanceLeft<=0);
}

//not limited, always can get orders
bool DriverVolunteer:: hasOrdersLeft() const{
    return true;
}

//order can be taken by this driver if its panding and has been proccessed by collector
//and havent been proccessed by other driver.
//and only if this driver is not busy or the distane of the order is less than the max distance
bool DriverVolunteer:: canTakeOrder(const Order &order) const{
    if(order.getStatus() != OrderStatus(0))
        return false;
    else if (order.getCollectorId() == NO_VOLUNTEER || order.getDriverId() != NO_VOLUNTEER)
        return false;
    if(isBusy())
        return false;
    if(order.getDistance()>maxDistance)
        return false;
    return true;
}

//Reset distanceLeft to maxDistance
 void DriverVolunteer:: acceptOrder(const Order &order){
    //this->activeOrder = new Order(-1,-1,-1);
    this->activeOrderId = NO_ORDER;
    this->distanceLeft = maxDistance;
    this->completedOrderId = order.getId();
 }

//Decrease distanceLeft by distancePerStep
 void DriverVolunteer:: step() { 
    decreaseDistanceLeft();
 }

 //to string
string DriverVolunteer:: toString() const{
    string str = "Id: " + std::to_string(this->getId()) +"\n"+
    "Name: " + this->getName() +"\n"+
    "Is this collector busy now? " + std::to_string(this->isBusy()) +"\n"+
    "Max Distance: " + std::to_string(this->maxDistance) +"\n"+
    "Distance Per Step: " + std::to_string(this->distancePerStep) +"\n"+
    "Distance Left: " + std::to_string(this->distanceLeft);
    return str;
}

//change distance left according to the customer distance
void DriverVolunteer::setDistanceLeft(int dis){
    this->distanceLeft = dis;
}

///////////////////////////////////////////////////////////////////////////////////
//LimitedDriverVolunteer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
LimitedDriverVolunteer:: LimitedDriverVolunteer
(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders){}   

//copy function - creates new limited driver volunteer
LimitedDriverVolunteer* LimitedDriverVolunteer :: clone() const {
    //on heap$$$
    LimitedDriverVolunteer* clone = new LimitedDriverVolunteer(*this);
    return clone;
} 

//return maxOrders
int LimitedDriverVolunteer:: getMaxOrders() const{
    return maxOrders;
}

//return orders left
int LimitedDriverVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}

//return true if ordersLeft>0
bool LimitedDriverVolunteer:: hasOrdersLeft() const{
    return (ordersLeft>0);
}

//order can be taken by this driver if its panding and has been proccessed by collector
//and havent been proccessed by other driver.
//and only if this driver is not busy or the distane of the order is less than the max distance
//and only if he has orders left
bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const{
    if(!hasOrdersLeft())
        return false;
    if(order.getStatus() != OrderStatus(0))
        return false;
    else if (order.getCollectorId() == NO_VOLUNTEER || order.getDriverId() != NO_VOLUNTEER)
        return false;
    if(isBusy())
        return false;
    if(order.getDistance()>getMaxDistance())
        return false;
    return true;
}

//Reset distanceLeft to maxDistance
//decrease orders left by 1
 void LimitedDriverVolunteer:: acceptOrder(const Order &order){
    DriverVolunteer::acceptOrder(order);
 }

 //to string
string LimitedDriverVolunteer:: toString() const{
    string str = this->toString() + "\n"+
    "Max Orders: " + std::to_string(this->maxOrders) + "\n"+
    "Orders Left: " + std::to_string(this->ordersLeft);
    return str;
}

void LimitedDriverVolunteer::setOrdersLeft(){
    this->ordersLeft =  ordersLeft-1;
}

