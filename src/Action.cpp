#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/WareHouse.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using std::string;

class Order;
class Volunteer;
class Customer;

extern WareHouse* backup;


///////////////////////////////////////////////////////////////////////////////////
//BaseAction class
///////////////////////////////////////////////////////////////////////////////////

//constructor
BaseAction:: BaseAction():errorMsg("-1"), status(ActionStatus::COMPLETED) {}

//return status
ActionStatus BaseAction:: getStatus() const{
    return status;
}

//update the action status to completed
void BaseAction:: complete(){
    status = ActionStatus(ActionStatus::COMPLETED);
}

//update the status and error msg
void BaseAction:: error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

//return the error msg
string BaseAction:: getErrorMsg() const{
    return errorMsg;
}



 // convert enum status to string
string BaseAction::StatusEnumToString(ActionStatus enumStatus)const{
    if(enumStatus==ActionStatus:: COMPLETED){
        return "COMPLETED";
    }
    else{
        return "ERORR";
    }
}

//convert enum type to string
string BaseAction::TypeEnumToString(CustomerType enumType)const{
    if(enumType==CustomerType::Civilian){
        return "Civilian";
    }
    else{
        return "Soldier";
    }
}

CustomerType BaseAction::TypeStringToEnum(const string &type) const{
    if (type == "soldier" || type == "Soldier" || type == "SOLDIER") {
           return CustomerType::Soldier;
    }
    else {
            return CustomerType::Civilian;
    }
}

void BaseAction::setErrorMsg(const string& msg){
    this->errorMsg = msg;
 }
void BaseAction::setStatus(ActionStatus status){
    this->status = status;
}

///////////////////////////////////////////////////////////////////////////////////
//SimulateStep class
///////////////////////////////////////////////////////////////////////////////////

//constructor
SimulateStep:: SimulateStep(int numOfSteps): BaseAction(), numOfSteps(numOfSteps) {}

//simulate step - new
void SimulateStep::act(WareHouse &wareHouse){
    int step = numOfSteps;
    while(step > 0){
        Step1(wareHouse);
        Step2(wareHouse);
        for(Order* order : wareHouse.getPendingOrders()){
            if(order->FinishStep){
                order->FinishStep=false;
            }
        }
        for(Order* order : wareHouse.getInProcessOrders()){
            if(order->FinishStep){
                order->FinishStep=false;
            }
        }
        for(Order* order : wareHouse.getCompletedOrders()){
            if(order->FinishStep){
                order->FinishStep=false;
            }
        }
        step=step-1;
    }
    wareHouse.addAction(this);
}

void SimulateStep::Step1(WareHouse &wareHouse){
    vector<Order*>& pendingOrders = wareHouse.getPendingOrders();
    vector<Volunteer*>& volunteers = wareHouse.getVolunteers();
    for(auto ito = pendingOrders.begin(); ito != pendingOrders.end();){
        Order* i = *ito;
            if(i->getStatus() == OrderStatus::PENDING){
                bool finished = false;
                for(auto it = volunteers.begin(); (!finished) && it != volunteers.end();){
                    Volunteer* vol = *it;
                    if(dynamic_cast<CollectorVolunteer*>(vol) || dynamic_cast<LimitedCollectorVolunteer*>(vol)){
                        if(!(vol->isBusy())){
                            vol->setActiveOrderId(i->getId());
                            //vol->activeOrder = i;
                            i->setStatus(OrderStatus::COLLECTING);
                            ito = pendingOrders.erase(ito);
                            --ito;
                            wareHouse.setInProcessOrders(i);
                            i->FinishStep = true;
                            i->setCollectorId(vol->getId());
                            finished = true;
                            if(dynamic_cast<LimitedCollectorVolunteer*>(vol)){
                                LimitedCollectorVolunteer* limited = dynamic_cast<LimitedCollectorVolunteer*>(vol);
                                limited->setOrdersLeft();
                            }
                        }
                    }
                ++it;
                }
            }
            if(!(i->FinishStep) && i->getStatus() == OrderStatus::COLLECTING){
                bool finished1 = false;
                for(auto it1 = volunteers.begin(); (!finished1) && it1 != volunteers.end();){
                    Volunteer* vol = *it1;
                    if(dynamic_cast<DriverVolunteer*>(vol) || dynamic_cast<LimitedDriverVolunteer*>(vol)){
                        if(!vol->isBusy()){
                            vol->setActiveOrderId(i->getId());
                            //vol->activeOrder = i;
                            i->setStatus(OrderStatus::DELIVERING);
                            ito = pendingOrders.erase(ito);     
                            --ito;          
                            wareHouse.setInProcessOrders(i);
                            i->FinishStep = true;
                            i->setDriverId(vol->getId());
                            finished1 = true;
                            if(dynamic_cast<LimitedDriverVolunteer*>(vol)){
                                LimitedDriverVolunteer* limited = dynamic_cast<LimitedDriverVolunteer*>(vol);
                                limited->setDistanceLeft(i->getDistance());
                                limited->setOrdersLeft();
                            }
                            else{
                                DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(vol);
                                driver->setDistanceLeft(i->getDistance());
                            }
                        }
                     }
                ++it1;
                }
            }
        ++ito;

    }
}

void SimulateStep::Step2(WareHouse &wareHouse){
    vector<Volunteer*>& volunteers = wareHouse.getVolunteers();
    for(Volunteer* vol: volunteers){
        if(vol->isBusy()){
            if(dynamic_cast<LimitedCollectorVolunteer*>(vol)){
                LimitedCollectorVolunteer* limcol = dynamic_cast<LimitedCollectorVolunteer*>(vol);
                limcol->step();
            }
            else if(dynamic_cast<CollectorVolunteer*>(vol)){
                CollectorVolunteer* col = dynamic_cast<CollectorVolunteer*>(vol);
                col->step();
            }
            if(dynamic_cast<LimitedDriverVolunteer*>(vol)){
                LimitedDriverVolunteer* limdriver = dynamic_cast<LimitedDriverVolunteer*>(vol);
                limdriver->step();
            }
            else if(dynamic_cast<DriverVolunteer*>(vol)){
                DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(vol);
                driver->step();
            }
        Step3(wareHouse,vol);    
        }
    }
}

void SimulateStep::Step3(WareHouse &wareHouse,Volunteer* vol){
    vector<Order*>& inProcessOrders = wareHouse.getInProcessOrders();
    if(dynamic_cast<LimitedCollectorVolunteer*>(vol)){
        LimitedCollectorVolunteer* limitedCollecter = dynamic_cast<LimitedCollectorVolunteer*>(vol);
        if(limitedCollecter->getTimeLeft() == 0){
            Order* activeOrder = wareHouse.getPointerOrder(limitedCollecter->getActiveOrderId());
            inProcessOrders.erase(remove(inProcessOrders.begin(),inProcessOrders.end(),activeOrder),inProcessOrders.end());
            wareHouse.setPendingOrders(activeOrder);
            limitedCollecter->acceptOrder(*activeOrder);
            Step4(wareHouse,vol);
        }
    }
    else if(dynamic_cast<CollectorVolunteer*>(vol)){
        CollectorVolunteer* collecter = dynamic_cast<CollectorVolunteer*>(vol);
        if(collecter->getTimeLeft() == 0){
            Order* activeOrder = wareHouse.getPointerOrder(collecter->getActiveOrderId());
            inProcessOrders.erase(remove(inProcessOrders.begin(),inProcessOrders.end(),activeOrder),inProcessOrders.end());
            wareHouse.setPendingOrders(activeOrder);
            collecter->acceptOrder(*activeOrder);
        }
    }
    else if(dynamic_cast<LimitedDriverVolunteer*>(vol)){
        LimitedDriverVolunteer* LimitedDriver = dynamic_cast<LimitedDriverVolunteer*>(vol);
        if(LimitedDriver->getDistanceLeft() == 0){
            Order* activeOrder = wareHouse.getPointerOrder(LimitedDriver->getActiveOrderId());
            inProcessOrders.erase(remove(inProcessOrders.begin(),inProcessOrders.end(),activeOrder),inProcessOrders.end());
            wareHouse.setCompletedOrders(activeOrder);
            activeOrder->setStatus(OrderStatus::COMPLETED);
            LimitedDriver->acceptOrder(*activeOrder);
            Step4(wareHouse,vol);
        }
    }
    else if(dynamic_cast<DriverVolunteer*>(vol)){
        DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(vol);
        if(driver->getDistanceLeft() == 0){
            Order* activeOrder = wareHouse.getPointerOrder(driver->getActiveOrderId());
            inProcessOrders.erase(remove(inProcessOrders.begin(),inProcessOrders.end(),activeOrder),inProcessOrders.end());
            wareHouse.setCompletedOrders(activeOrder);
            activeOrder->setStatus(OrderStatus::COMPLETED);
            driver->acceptOrder(*activeOrder);
        }
    }
    
}

void SimulateStep::Step4(WareHouse &wareHouse, Volunteer* vol){
    vector<Volunteer*>& volunteers = wareHouse.getVolunteers();
    if(dynamic_cast<LimitedCollectorVolunteer*>(vol)){
        LimitedCollectorVolunteer* limitedCollecter = dynamic_cast<LimitedCollectorVolunteer*>(vol);
        if(limitedCollecter->getNumOrdersLeft() == 0){
            volunteers.erase(remove(volunteers.begin(),volunteers.end(),limitedCollecter),volunteers.end());
            delete limitedCollecter;
        }
    }
    else if(dynamic_cast<LimitedDriverVolunteer*>(vol)){
        LimitedDriverVolunteer* LimitedDriver = dynamic_cast<LimitedDriverVolunteer*>(vol);
        if(LimitedDriver->getNumOrdersLeft() == 0){
            volunteers.erase(remove(volunteers.begin(),volunteers.end(),LimitedDriver),volunteers.end());
            delete LimitedDriver;
        }
    }
}

//to string
string SimulateStep:: toString() const{
    string str = "simulateStep " +std:: to_string(numOfSteps)+" "+ StatusEnumToString(getStatus());
    return str;
}

//copy function - creates new simulate step
SimulateStep* SimulateStep:: clone() const{
    //on heap$$$
    SimulateStep* clone = new SimulateStep (this->numOfSteps);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

///////////////////////////////////////////////////////////////////////////////////
//AddOrder class
///////////////////////////////////////////////////////////////////////////////////

//constructor
AddOrder:: AddOrder(int id): BaseAction(), customerId(id) {}

void AddOrder:: act(WareHouse &wareHouse){
    bool stop = false;
    Customer* cus = wareHouse.getPointerCustomer(this->customerId);
    if(!stop && cus->getId() == -1){
        this->error("Cannot place this order");
        cout<<"Error: " + this->getErrorMsg() << endl;
        stop = true;
    }
    if(!stop && !cus->canMakeOrder()){
        this->error("Cannot place this order");
        cout<<"Error: " + this->getErrorMsg() << endl;
        stop = true;
    }
    if(!stop){
        cus->orderCounter = cus->orderCounter+1;
        int idOrd = cus->addOrder(wareHouse.AllorderCounter);
        //on heap$$$
        Order *newOrder = new Order(idOrd,cus->getId(),cus->getCustomerDistance());
        wareHouse.addOrder(newOrder);
    }
    wareHouse.addAction(this);
}

//to string
string AddOrder:: toString() const{
    string str="order " + to_string(customerId) +" "+ StatusEnumToString(getStatus());
    if(StatusEnumToString(getStatus()) == "Error"){
        str = str+": " + getErrorMsg() + "\n";
    }
    return str;
}

//copy function - creates new order
AddOrder* AddOrder:: clone() const{
    //on heap$$$
    AddOrder* clone = new AddOrder (this->customerId);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

///////////////////////////////////////////////////////////////////////////////////
//AddCustomer class
///////////////////////////////////////////////////////////////////////////////////

//constructor
AddCustomer:: AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders): 
BaseAction(),customerName(customerName),customerType(TypeStringToEnum(customerType)),distance(distance),
maxOrders(maxOrders) {}

//add customer to the list
//onheap$$$
void AddCustomer:: act(WareHouse &wareHouse){
    int id = wareHouse.getCustomerCounter();
    if(this->customerType==CustomerType::Soldier){
        SoldierCustomer *cus = new SoldierCustomer(id,this->customerName,this->distance,this->maxOrders);
        wareHouse.setCustomers(cus);
        wareHouse.setCustomerCounter();
    }
    else{
        CivilianCustomer *cus = new CivilianCustomer(id,this->customerName,this->distance,this->maxOrders);
        wareHouse.setCustomers(cus);
        wareHouse.setCustomerCounter();
    }
    wareHouse.addAction(this);
}

//copy function - creates new customer
AddCustomer* AddCustomer:: clone() const{
    //on heap$$$
    AddCustomer* clone = new AddCustomer (this->customerName,TypeEnumToString(this->customerType),this->distance,this->maxOrders);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string AddCustomer:: toString() const{
    string str = "customer " + customerName + " " + TypeEnumToString(customerType) +" "+ 
    to_string(distance) +" " +to_string(maxOrders);
    return str;
}


///////////////////////////////////////////////////////////////////////////////////
//PrintOrderStatus class
///////////////////////////////////////////////////////////////////////////////////

//constructor
PrintOrderStatus:: PrintOrderStatus(int id): BaseAction(), orderId(id) {}

//print order if exists
void PrintOrderStatus:: act(WareHouse &wareHouse){
    bool stop = false;
    Order printOrder = wareHouse.getOrder(this->orderId);
    if(!stop && printOrder.getId() == -1){
        this->error("Order doesn't exsits");
        cout << "Error: " + this->getErrorMsg() << endl;
        stop=true;
    }
    if(!stop){
        cout << "OrderId: " << to_string (printOrder.getId()) << endl;
        cout << "OrderStatus: " << printOrder.OrderStatusEnumToString(printOrder.getStatus()) << endl;
        cout << "CustomerId: " << to_string(printOrder.getCustomerId()) << endl;
        if(printOrder.getCollectorId()==-1){
            cout << "Collector: None " << endl;
        }
        else{
            cout << "Collector: " << to_string(printOrder.getCollectorId()) << endl;
        }
        if(printOrder.getDriverId()==-1){
            cout << "Driver: None" <<  endl;
        }
        else{
            cout << "Driver: " << to_string(printOrder.getDriverId()) << endl;
        }
    }
    wareHouse.addAction(this);
}

//copy function - creates new instance
PrintOrderStatus* PrintOrderStatus:: clone() const{
    //on heap$$$
    PrintOrderStatus* clone = new PrintOrderStatus (this->orderId);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string PrintOrderStatus:: toString() const{
    string str= "orderStatus: " + to_string(this->orderId)+ " "+
    StatusEnumToString(getStatus());
    if(StatusEnumToString(getStatus()) == "ERORR"){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

///////////////////////////////////////////////////////////////////////////////////
//PrintCustomerStatus class
///////////////////////////////////////////////////////////////////////////////////

//constructor
PrintCustomerStatus:: PrintCustomerStatus(int id): BaseAction(), customerId(id) {}

//print customer's orders if exist
void PrintCustomerStatus:: act(WareHouse &wareHouse){
    bool stop = false;
    Customer* printCus = wareHouse.getPointerCustomer(customerId);
    if(!stop && printCus->getId() == -1){
        this->error("Customer doesn't exsits");
        cout << "Error: " + this->getErrorMsg() << endl;
        stop = true;
    }
    if (!stop){
        cout << "CustomerID: " << to_string(printCus->getId()) << endl;
        for( Order* i : wareHouse.getPendingOrders()){
            if(i->getCustomerId()==printCus->getId()){
                cout << "OrderID: " << to_string(i->getId()) << endl;
                cout << "OrderStatus: " << i->OrderStatusEnumToString(i->getStatus()) << endl;
            }
        }
        for(Order* i : wareHouse.getInProcessOrders()){
            if(i->getCustomerId()==printCus->getId()){
                cout << "OrderID: " << to_string(i->getId()) << endl;
                cout << "OrderStatus: " << i->OrderStatusEnumToString(i->getStatus()) << endl;
            }
        }
        for(Order* i : wareHouse.getCompletedOrders()){
            if(i->getCustomerId()==printCus->getId()){
                cout << "OrderID: " << to_string(i->getId()) << endl;
                cout << "OrderStatus: " << i->OrderStatusEnumToString(i->getStatus()) << endl;
            }
        }
        cout << "numOerdersLeft: " << to_string(printCus->getMaxOrders() - printCus->orderCounter) << endl;
    }
    wareHouse.addAction(this);
}

//copy function - creates new instance
PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    //on heap$$$
    PrintCustomerStatus* clone = new PrintCustomerStatus (this->customerId);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string PrintCustomerStatus:: toString() const{
    string str= "customerStatus: " + to_string(this->customerId)+ " "+
    StatusEnumToString(getStatus());
    if(StatusEnumToString(getStatus()) == "ERORR"){
        str = str + ": " + getErrorMsg();
    }
    return str;
}

///////////////////////////////////////////////////////////////////////////////////
//PrintVolunteerStatus class
///////////////////////////////////////////////////////////////////////////////////

//constructor
PrintVolunteerStatus:: PrintVolunteerStatus(int id): BaseAction(), volunteerId(id) {}

//print volunteer status
void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    bool stop = false;
    Volunteer* printVol = wareHouse.getPointerVolunteer(this->volunteerId);
    if(!stop && printVol->getId() == -1){
        this->error("Volunteer doesn't exsits");
        cout << "Error: " + this->getErrorMsg() << endl;
        stop = true;
    }
    if(!stop){
        cout << "VolunteerID: " << to_string(printVol->getId()) << endl;
        if(!(printVol->isBusy())){
            cout << "isBusy: False"  << endl;
            cout << "OrderID: None" << endl;
        }
        else{
            cout << "isBusy: True"  << endl;
            cout << "OrderID: " << to_string(printVol->getActiveOrderId()) << endl;
        }

        if(dynamic_cast<CollectorVolunteer*>(printVol) || dynamic_cast<LimitedCollectorVolunteer*>(printVol)){
            CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(printVol);
            if(!(printVol->isBusy()))
                cout << "timeLeft: None" << endl;
            else{
                cout << "timeLeft: " << to_string(collector->getTimeLeft()) << endl;
            }
        }
        else{
            DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(printVol);
            if(!(printVol->isBusy()))
                cout << "timeLeft: None" << endl;
            else{
                cout << "timeLeft: " << to_string(driver->getDistanceLeft()) << endl;
            }
        }
        if(dynamic_cast<LimitedCollectorVolunteer*>(printVol) || dynamic_cast<LimitedDriverVolunteer*>(printVol)){
            LimitedCollectorVolunteer* limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(printVol);
            LimitedDriverVolunteer* limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(printVol);
            if (limitedCollector) {
                cout << "OrdersLeft: " << to_string(limitedCollector->getNumOrdersLeft()) << endl;
            } 
            else if (limitedDriver) {
                cout << "OrdersLeft: " << to_string(limitedDriver->getNumOrdersLeft()) << endl;
            }
        }
        else{
            cout << "OrdersLeft: No Limit"<< endl; 
        }
    }
    wareHouse.addAction(this);
}

//copy function - creates new instance
PrintVolunteerStatus* PrintVolunteerStatus:: clone() const{
    //on heap$$$
    PrintVolunteerStatus* clone = new PrintVolunteerStatus (this->volunteerId);
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string PrintVolunteerStatus:: toString() const{
    string str= "volunteerStatus: " + to_string(this->volunteerId)+ " "+
    StatusEnumToString(getStatus());
    if(StatusEnumToString(getStatus()) == "ERORR"){
        str = str + ": " + getErrorMsg();
    }
    return str;
}


///////////////////////////////////////////////////////////////////////////////////
//PrintActionsLog class
///////////////////////////////////////////////////////////////////////////////////

//constructor
PrintActionsLog:: PrintActionsLog(): BaseAction() {}

//copy function - creates new instance
PrintActionsLog* PrintActionsLog:: clone() const{
    //on heap$$$
    PrintActionsLog* clone = new PrintActionsLog ();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

void PrintActionsLog::act(WareHouse &wareHouse){
    for(BaseAction* i: wareHouse.getActions()){
        cout <<i->toString() << endl;
    }
    wareHouse.addAction(this);
}

//to string
string PrintActionsLog:: toString() const{
    string str="log " + StatusEnumToString(getStatus());
    return str;
}

///////////////////////////////////////////////////////////////////////////////////
//Close class
///////////////////////////////////////////////////////////////////////////////////

//constructor
Close:: Close(): BaseAction() {}

void Close:: act(WareHouse &wareHouse){
    for (Order *i : wareHouse.getPendingOrders()){
        cout << "orderID: "+ to_string(i->getId()) +", CustomerID: "+
        to_string(i->getCustomerId())+", Status: " + i->OrderStatusEnumToString(i->getStatus()) << endl;
    }
    for (Order *i : wareHouse.getInProcessOrders()){
        cout << "orderID: "+ to_string(i->getId()) +", CustomerID: "+
        to_string(i->getCustomerId())+", Status: " + i->OrderStatusEnumToString(i->getStatus()) << endl;
    }
    for (Order  *i : wareHouse.getCompletedOrders()){
        cout << "orderID: "+ to_string(i->getId()) +", CustomerID: "+
        to_string(i->getCustomerId())+", Status: " + i->OrderStatusEnumToString(i->getStatus()) << endl;
    }
    wareHouse.addAction(this);
    wareHouse.close();
}

//copy function - creates new instance
Close* Close:: clone() const{
    //on heap$$$
    Close* clone = new Close ();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string Close:: toString() const{
    string str= "close " + StatusEnumToString(getStatus());
    return str;
}

///////////////////////////////////////////////////////////////////////////////////
//BackupWareHouse class
///////////////////////////////////////////////////////////////////////////////////

//constructor
BackupWareHouse:: BackupWareHouse(): BaseAction() {}

//save the last data of the warehouse
void BackupWareHouse:: act(WareHouse &wareHouse){
    if(backup!=nullptr){
        delete backup;
    }
    wareHouse.addAction(this);
    backup=wareHouse.clone();
    
}

//copy function - creates new instance
BackupWareHouse* BackupWareHouse:: clone() const{
    //on heap$$$
    BackupWareHouse* clone = new BackupWareHouse ();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string BackupWareHouse:: toString() const{
    string str= "backup " + StatusEnumToString(getStatus());
    return str;
}

///////////////////////////////////////////////////////////////////////////////////
//RestoreWareHouse class
///////////////////////////////////////////////////////////////////////////////////

//constructor
RestoreWareHouse:: RestoreWareHouse(): BaseAction() {}

void RestoreWareHouse:: act(WareHouse &wareHouse){
    if (backup==nullptr){
        this->error("No backup available");
        cout<<"Error: " + this->getErrorMsg() << endl;
    }
    else{
        wareHouse=*backup;
    }
    wareHouse.addAction(this);
}

//copy function - creates new instance
RestoreWareHouse* RestoreWareHouse:: clone() const{
    //on heap$$$
    RestoreWareHouse* clone = new RestoreWareHouse ();
    clone->setErrorMsg(this->getErrorMsg());
    clone->setStatus(this->getStatus());
    return clone;
}

//to string
string RestoreWareHouse:: toString() const{
    string str= "restore " + StatusEnumToString(getStatus());
    return str;
}





