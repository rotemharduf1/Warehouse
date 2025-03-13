
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include "../include/Volunteer.h"
#include "../include/Action.h"
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/WareHouse.h"
using std::string;


///////////////////////////////////////////////////////////////////////////////////
//WareHouse class
///////////////////////////////////////////////////////////////////////////////////

//on heap$$$
//constructor
WareHouse:: WareHouse(const string &configFilePath):AllorderCounter(0),def_vol(),def_or(),def_cus(),isOpen(false),actionsLog(),volunteers(),pendingOrders(),
inProcessOrders(),completedOrders(),customers(),customerCounter(0),volunteerCounter(0)
{
    tuple<vector<string>, vector<string>> fileData = readFile(configFilePath);
    vector<Volunteer*> volunteers1 = createsVolunteer(get<0>(fileData));
    vector<Customer*> customers1 = createsCustomer(get<1>(fileData));
    volunteers=volunteers1;
    customers=customers1;
    customerCounter = customers.size();
    volunteerCounter = volunteers.size();
    def_vol.push_back(new CollectorVolunteer(-1,"no volunteer",-1));
    def_or.push_back(new Order(-1,-1,-1));
    def_cus.push_back(new SoldierCustomer(-1,"no customer",-1,-1));
}

//empty constructor
//on Heap$$
WareHouse:: WareHouse(): AllorderCounter(0),def_vol(),def_or(),def_cus(),
    isOpen(false), actionsLog(), volunteers(),
    pendingOrders(), inProcessOrders(), completedOrders(),
    customers(), customerCounter(0), volunteerCounter(0) {
    def_vol.push_back(new CollectorVolunteer(-1,"no volunteer",-1));
    def_or.push_back(new Order(-1,-1,-1));
    def_cus.push_back(new SoldierCustomer(-1,"no customer",-1,-1));
    }


//rule of 5 implementation

//copy constructor
 WareHouse:: WareHouse(const WareHouse* other): AllorderCounter(other->AllorderCounter),def_vol(),def_or(),def_cus(),
    isOpen(other->isOpen),actionsLog(),volunteers(),pendingOrders(), inProcessOrders(), completedOrders(),
    customers(), customerCounter(other->customerCounter),volunteerCounter(other->volunteerCounter) {
    for(BaseAction* i : other->actionsLog){
        this->actionsLog.push_back(i->clone());
    }
    for(Volunteer* i : other->volunteers){
        this->volunteers.push_back(i->clone());
    }
    for(Volunteer* i : other->def_vol){
        this->def_vol.push_back(i->clone());
    }
    for(Order* i : other->pendingOrders){
        this->pendingOrders.push_back(i->clone());
    }
    for(Order* i : other->inProcessOrders){
        this->inProcessOrders.push_back(i->clone());
    }
    for(Order* i : other->completedOrders){
        this->completedOrders.push_back(i->clone());
    } 
    for(Order* i : other->def_or){
        this->def_or.push_back(i->clone());
    } 
    for(Customer* i : other->customers){
        this->customers.push_back(i->clone());
    }
    for(Customer* i : other->def_cus){
        this->def_cus.push_back(i->clone());
    }
} 

//operator = 
WareHouse& WareHouse::operator=(const WareHouse& other){
    if(&other!=this){
        isOpen = other.isOpen;
        customerCounter=other.customerCounter; 
        volunteerCounter=other.volunteerCounter;
        AllorderCounter=other.AllorderCounter;
        for (BaseAction* i: actionsLog){
            delete i;
            i=nullptr;
        }
        actionsLog.clear();
         for(BaseAction* i : other.actionsLog){
            this->actionsLog.push_back(i->clone());
        }
        for (Volunteer* i: volunteers){
            delete i;
            i=nullptr;
        }
        volunteers.clear();
        for(Volunteer* i : other.volunteers){
            this->volunteers.push_back(i->clone());
        }
        for (Order* i: pendingOrders){
            delete i;
            i=nullptr;
        }
        pendingOrders.clear();
        for(Order* i : other.pendingOrders){
            this->pendingOrders.push_back(i->clone());
        }
        for (Order* i: inProcessOrders){
            delete i;
            i=nullptr;
        }
        inProcessOrders.clear();
        for(Order* i : other.inProcessOrders){
            this->inProcessOrders.push_back(i->clone());
        }
        for (Order* i: completedOrders){
            delete i;
            i=nullptr;
        }
        completedOrders.clear();
        for(Order* i : other.completedOrders){
            this->completedOrders.push_back(i->clone());
        }
        for (Customer* i: customers){
            delete i;
            i=nullptr;
        }
        customers.clear();
        for(Customer* i : other.customers){
            this->customers.push_back(i->clone());
        }
        for (Volunteer* i: def_vol){
            delete i;
            i=nullptr;
        }
        def_vol.clear();
        for(Volunteer* i : other.def_vol){
            this->def_vol.push_back(i->clone());
        }
        for (Order* i: def_or){
            delete i;
            i=nullptr;
        }
        def_or.clear();
        for(Order* i : other.def_or){
            this->def_or.push_back(i->clone());
        }
        for (Customer* i: def_cus){
            delete i;
            i=nullptr;
        }
        def_cus.clear();
        for(Customer* i : other.def_cus){
            this->def_cus.push_back(i->clone());
        }
    }
    return *this;
}

//move assignment operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (&other != this) {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        AllorderCounter = other.AllorderCounter;
        for (BaseAction* i : actionsLog) {
            delete i;
        }
        actionsLog.clear();
        for (Volunteer* i : volunteers) {
            delete i;
            i=nullptr;
        }
        volunteers.clear();
        for (Order* i : pendingOrders) {
            delete i;
            i=nullptr;
        }
        pendingOrders.clear();
        for (Order* i : inProcessOrders) {
            delete i;
            i=nullptr;
        }
        inProcessOrders.clear();
        for (Order* i : completedOrders) {
            delete i;
            i=nullptr;
        }
        completedOrders.clear();
        for (Customer* i : customers) {
            delete i;
            i=nullptr;
        }
        customers.clear();
        for (Volunteer* i : def_vol) {
            delete i;
            i=nullptr;
        }
        def_vol.clear();
        for (Order* i : def_or) {
            delete i;
            i=nullptr;
        }
        def_or.clear();
        for (Customer* i : def_cus) {
            delete i;
            i=nullptr;
        }
        def_cus.clear();
        actionsLog = move(other.actionsLog);
        volunteers = move(other.volunteers);
        pendingOrders = move(other.pendingOrders);
        inProcessOrders = move(other.inProcessOrders);
        completedOrders = move(other.completedOrders);
        customers = move(other.customers);
        def_vol = move(other.def_vol);
        def_or = move(other.def_or);
        def_cus = move(other.def_cus);
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.AllorderCounter = 0;
    }
    return *this;
}

//move copy constructor
WareHouse::WareHouse(WareHouse&& other) noexcept:
    AllorderCounter(other.AllorderCounter),
    def_vol(move(other.def_vol)),
    def_or(move(other.def_or)),
    def_cus(move(other.def_cus)),
    isOpen(other.isOpen),
    actionsLog(move(other.actionsLog)),
    volunteers(move(other.volunteers)),
    pendingOrders(move(other.pendingOrders)),
    inProcessOrders(move(other.inProcessOrders)),
    completedOrders(move(other.completedOrders)),
    customers(move(other.customers)),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter){
    other.isOpen = false;
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.AllorderCounter = 0;
}

//warehouse destructor
WareHouse::~WareHouse() {
    for (BaseAction* action : actionsLog) {
        delete action;
        action =nullptr;
    }
    actionsLog.clear(); 
    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
        volunteer=nullptr;
    }
    volunteers.clear();
    for (Order* order : pendingOrders) {
        delete order;
        order=nullptr;
    }
    pendingOrders.clear();
    for (Order* order : inProcessOrders) {
        delete order;
        order=nullptr;
    }
    inProcessOrders.clear();
    for (Order* order : completedOrders) {
        delete order;
        order=nullptr;
    }
    completedOrders.clear();
    for (Customer* customer : customers) {
        delete customer;
        customer=nullptr;
    }
    customers.clear();
    for (Volunteer* volunteer : def_vol) {
        delete volunteer;
        volunteer=nullptr;
    }
    def_vol.clear();
    for (Order* order : def_or) {
        delete order;
        order=nullptr;
    }
    def_or.clear();
    for (Customer* customer : def_cus) {
        delete customer;
        customer=nullptr;
    }
    def_cus.clear();
}

//onheap $$$
//opening the warehouse and operate all actions until closing the warehouse
void WareHouse:: start(){
    open();
    cout << "Warehouse is open!" << endl;
    string userAction;
    cout << "Please enter an action: " << endl;
    getline(cin,userAction);
    while (userAction != "close") {
        DecodeInput(userAction);
        cout << "Please enter an action: " << endl;
        getline(cin,userAction);
    }
    Close* closeAction = new Close();
    closeAction->act(*this);
}

void WareHouse::addOrder(Order *order){
    setPendingOrders(order);
    AllorderCounter=AllorderCounter+1;
}

void WareHouse::addAction(BaseAction* action){
    this->actionsLog.push_back(action);
}

//return customer if exist, otherwise return nullptr
Customer & WareHouse::getCustomer(int customerId) const{
    for (Customer *i : customers){
        if ((*i).getId()==customerId)
            return *i;
    }
    return *(def_cus.front());
}

//return volunteer if exist, otherwise return nullptr
Volunteer & WareHouse::getVolunteer(int volunteerId) const{
    for (Volunteer *i : volunteers){
        if ((*i).getId()==volunteerId)
            return *i;
    }
    return *(def_vol.front());
}

//return order if exist in one of the lists, otherwise return nullptr
Order & WareHouse::getOrder(int orderId) const{
    for (Order *i : pendingOrders){
        if (i->getId()==orderId)
            return *i;
    }
    for (Order *i : inProcessOrders){
        if (i->getId()==orderId)
            return *i;
    }
    for (Order *i : completedOrders){
        if (i->getId()==orderId)
            return *i;
    }
    return *(def_or.front());
}

// return actionLog 
const vector<BaseAction *> &WareHouse::getActions() const{
    return this->actionsLog;
}

//closing the warehouse
void WareHouse::close(){
    this->isOpen = false;
}

//opening the warehouse
void WareHouse::open(){
    isOpen = true;
}

//read the config file and create 2 vectors of string: one for customer and one for volunteer
tuple<vector<string>, vector<string>> WareHouse::readFile(const string &configFilePath){
    string line;
    ifstream readMyFile(configFilePath);
    vector<string> volunteer;
    vector<string> customer;
    while (getline(readMyFile, line)){
        istringstream iss(line);
        string word;
        iss >> word;
        if (word == "customer") {
            customer.push_back(line);
        } else if (word == "volunteer") {
            volunteer.push_back(line);
        }
    }
    return make_tuple(volunteer, customer);
}

//on heap$$$
//creates a vector of volunteers
vector<Volunteer*> WareHouse::createsVolunteer(const vector<string> &vectorVolenteer){
    vector<Volunteer *> volunteers = vector<Volunteer*>();
    string name;
    string type;
    string coolDown;
    int coolDownInt;
    string maxOrders;
    int maxOrdersInt;
    string maxDis;
    int maxDisInt;
    string disPerStep;
    int disPerStepInt;
    for (string line : vectorVolenteer){
        istringstream iss(line);
        string word;
        for(int i=0; i<3; i++){
            iss >> word;
            if(i==1)
                name = word;
            if(i==2){
                type = word;
                if(type=="collector"){
                    iss >> word;
                    coolDown=word;
                    coolDownInt=stoi(coolDown);
                    CollectorVolunteer *vol =  new CollectorVolunteer(volunteerCounter,name,coolDownInt);
                    setVolunteerCounter();
                    volunteers.push_back(vol);
                }
                else if (type=="limited_collector"){
                    iss >> word;
                    coolDown=word;
                    coolDownInt=stoi(coolDown);
                    iss >> word;
                    maxOrders=word;
                    maxOrdersInt = stoi(maxOrders);
                    LimitedCollectorVolunteer *vol =  new LimitedCollectorVolunteer(volunteerCounter,name,coolDownInt,maxOrdersInt);
                    setVolunteerCounter();
                    volunteers.push_back(vol);
                }
                else if (type=="driver"){
                    iss >> word;
                    maxDis=word;
                    maxDisInt=stoi(maxDis);
                    iss >> word;
                    disPerStep=word;
                    disPerStepInt=stoi(disPerStep);
                    DriverVolunteer* vol =  new DriverVolunteer(volunteerCounter,name,maxDisInt,disPerStepInt);
                    setVolunteerCounter();
                    volunteers.push_back(vol);
                }
                else if (type=="limited_driver"){
                    iss >> word;
                    maxDis=word;
                    maxDisInt=stoi(maxDis);
                    iss >> word;
                    disPerStep=word;
                    disPerStepInt=stoi(disPerStep);
                    iss >> word;
                    maxOrders=word; 
                    maxOrdersInt = stoi(maxOrders);
                    LimitedDriverVolunteer *vol =  new LimitedDriverVolunteer(volunteerCounter,name,maxDisInt,disPerStepInt,maxOrdersInt);
                    setVolunteerCounter();
                    volunteers.push_back(vol);
                }
            } 
        }
    }
    return volunteers;
}

//on heap$$$
//creates a vector of customers
vector<Customer *> WareHouse::createsCustomer(const vector<string> &vectorCustumer){
    vector<Customer *> customers = vector<Customer*>();
    string name;
    string type;
    string distance;
    int distanceInt;
    string maxOrders;
    int maxOrdersInt;
    for (string line : vectorCustumer){
        istringstream iss(line);
        string word;
        for(int i=0; i<3; i++){
            iss >> word;
            if(i==1)
                name = word;
            if(i==2){
                type = word;
                if(type=="soldier"){
                    iss >> word;
                    distance=word;
                    distanceInt = stoi(distance);
                    iss >> word;
                    maxOrders=word;
                    maxOrdersInt = stoi(maxOrders);
                    SoldierCustomer* cus =  new SoldierCustomer(customerCounter,name,distanceInt,maxOrdersInt);
                    setCustomerCounter();
                    customers.push_back(cus);
                }
                else{
                    iss >> word;
                    distance=word;
                    distanceInt = stoi(distance);
                    iss >> word;
                    maxOrders=word;
                    maxOrdersInt = stoi(maxOrders);
                    CivilianCustomer *cus =  new CivilianCustomer(customerCounter,name,distanceInt,maxOrdersInt);
                    setCustomerCounter();
                    customers.push_back(cus);
                }
            } 
        }
    }
    return customers;
}

//return customerCounter
int WareHouse::getCustomerCounter(){
    return customerCounter;
}

//return volunteerCounter
int WareHouse::getVolunteerCounter(){
    return volunteerCounter;
}

//return pending orders
vector<Order*>& WareHouse::getPendingOrders(){
    vector<Order*>& pendings = pendingOrders;
    return pendings;
}

//return in process orders
vector<Order*>& WareHouse::getInProcessOrders(){
    vector<Order*>& inProcess = inProcessOrders;
    return inProcess;
}

//return in completed orders
vector<Order*>& WareHouse::getCompletedOrders(){
    vector<Order*>& completed = completedOrders;
    return completed;
}

//return volunteers
vector<Volunteer*>& WareHouse::getVolunteers() {
    vector<Volunteer*>& volunteers1 = volunteers;
    return volunteers1;
}

//return customers
vector<Customer*>& WareHouse::getCustomers() {
    vector<Customer*>& customers1 = customers;
    return customers1;
}

Order* WareHouse::getPointerOrder(int orderId) const{
    for (Order *i : pendingOrders){
        if (i->getId()==orderId)
            return i;
    }
    for (Order *i : inProcessOrders){
        if (i->getId()==orderId)
            return i;
    }
    for (Order *i : completedOrders){
        if (i->getId()==orderId)
            return i;
    }
    return def_or.front();

}

//increase customer counter by 1
int WareHouse::setCustomerCounter(){
    customerCounter+=1;
    return customerCounter;

}

//increase volunteer counter by 1
int WareHouse::setVolunteerCounter(){
    volunteerCounter+=1;
    return volunteerCounter;
}

//increase volunteer counter by 1
int WareHouse::increaseVolunteerCounter(){
    volunteerCounter+=1;
    return volunteerCounter;
}

//decrease volunteer counter by 1
int WareHouse::decreaseVolunteerCounter(){
    volunteerCounter-=1;
    return volunteerCounter;
}


//add volunteer
void WareHouse::setVolunteers(Volunteer *volunteer){
    volunteers.push_back(volunteer);
}

//add pending order
void WareHouse::setPendingOrders(Order *order){
    pendingOrders.push_back(order);
}

//add in process order
void WareHouse::setInProcessOrders(Order *order){
    inProcessOrders.push_back(order);
}

//add completed order
void WareHouse::setCompletedOrders(Order *order){
    completedOrders.push_back(order);
}

//add customer
void WareHouse::setCustomers(Customer *customer){
    customers.push_back(customer);
}

//on heap$$$
//return pointer to volunteer object
Volunteer* WareHouse::getPointerVolunteer(int volunteerId) const{
    for (Volunteer *i : volunteers){
        if (i->getId()==volunteerId)
            return i;
    }
    return def_vol.front();
}

//on heap$$$
//return pointer to customer object
Customer* WareHouse::getPointerCustomer(int customerId) const{
    for (Customer *i : customers){
        if (i->getId()==customerId)
            return i;
    }
    return def_cus.front();
}

// identify input from the user, and call the relevant function
void WareHouse::DecodeInput(string &userAction) {
    istringstream iss(userAction);
    string word;
    iss >> word;
    if (word == "step"){
        iss >> word;
        SimulateStep *simulate = new SimulateStep(stoi(word));
        simulate->act(*this);
    }
    else if(word == "order") {
        iss >> word;
        AddOrder* addorder = new AddOrder(stoi(word));
        addorder->act(*this);
    }
    else if(word == "customer") {
        iss >> word;
        string name = word;
        iss >> word;
        string type = word;
        iss >> word;
        string dis = word;
        int distance = stoi(dis);
        iss >> word;
        string max = word;
        int maxOr = stoi(max);
        AddCustomer* addcustomer = new AddCustomer(name,type,distance,maxOr);
        addcustomer->act(*this);
    }
    else if(word == "orderStatus"){
        iss >> word;
        PrintOrderStatus* orderStatus =new PrintOrderStatus(stoi(word));
        orderStatus->act(*this);
    }
    else if(word == "customerStatus"){
        iss >> word;
        PrintCustomerStatus* customerStatus =new PrintCustomerStatus(stoi(word));
        customerStatus->act(*this);
    }
    else if(word == "volunteerStatus"){
        iss >> word;
        PrintVolunteerStatus* volStatus =new PrintVolunteerStatus(stoi(word));
        volStatus->act(*this);
    }
    else if(word == "log"){
        PrintActionsLog* log = new PrintActionsLog();
        log->act(*this);
    }
    else if(word == "close"){
        Close* close = new Close();
        close->act(*this);
    }
    else if(word == "backup"){
        BackupWareHouse* backup = new BackupWareHouse();
        backup->act(*this);
    }
    else if(word == "restore"){
        RestoreWareHouse* restore = new RestoreWareHouse();
        restore->act(*this);
    }
    else{
        cout << "This input is not legal" << endl;
    }
}

//clone to warehouse
WareHouse* WareHouse:: clone() const{
    WareHouse* copy = new WareHouse();
    for(BaseAction* i : this->actionsLog){
        copy->actionsLog.push_back(i->clone());
    }
    for(Volunteer* i : this->volunteers){
        copy->volunteers.push_back(i->clone());
    }
    for(Order* i : this->pendingOrders){
        copy->pendingOrders.push_back(i->clone());
    }
    for(Order* i : this->inProcessOrders){
        copy->inProcessOrders.push_back(i->clone());
    }
    for(Order* i : this->completedOrders){
        copy->completedOrders.push_back(i->clone());
    }
    for(Customer* i : this->customers){
        copy->customers.push_back(i->clone());
    }
    copy->isOpen=this->isOpen;
    copy->customerCounter=this->customerCounter;
    copy->volunteerCounter=this->volunteerCounter;
    copy->AllorderCounter=this->AllorderCounter;
    return copy;
} 

