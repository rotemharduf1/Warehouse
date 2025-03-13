#pragma once
#include <string>
#include <vector>
using namespace std;
#include <iostream>
#include <fstream>
#include "../include/Customer.h"
#include "../include/Order.h"
#include "../include/Volunteer.h"

class BaseAction;
class Volunteer;


// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        //our functions - rule of 5:
        WareHouse(const WareHouse* other);
        WareHouse& operator=(const WareHouse& other);
        WareHouse& operator=(WareHouse&& other) noexcept;
        WareHouse(WareHouse&& other) noexcept;
        ~WareHouse();


        //our functions
        WareHouse();
        tuple<vector<string>, vector<string>> readFile(const string &configFilePath);
        vector<Volunteer*> createsVolunteer(const vector<string> &vectorVolenteer);
        vector<Customer*> createsCustomer(const vector<string> &vectorCustumer);
        int getCustomerCounter();
        int getVolunteerCounter();
        vector<Volunteer*>& getVolunteers();
        vector<Customer*>& getCustomers();
        vector<Order*>& getPendingOrders();
        vector<Order*>& getInProcessOrders();
        vector<Order*>& getCompletedOrders();
        Order *getPointerOrder(int orderId) const;
        int setCustomerCounter();
        int setVolunteerCounter();
        int increaseVolunteerCounter();
        int decreaseVolunteerCounter();
        void setVolunteers(Volunteer *volunteer);
        void setPendingOrders(Order *order);
        void setInProcessOrders(Order *order);
        void setCompletedOrders(Order *order);
        void setCustomers(Customer *customer);
        Volunteer* getPointerVolunteer(int volunteerId) const;
        Customer* getPointerCustomer(int customerId) const;
        void DecodeInput(string& userAction);
        WareHouse* clone() const;

        //our fields:
        int AllorderCounter;

    protected:
     //our fields for default pointers
        vector<Volunteer*> def_vol;
        vector<Order*> def_or;
        vector<Customer*> def_cus;


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
};