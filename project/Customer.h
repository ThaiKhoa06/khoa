#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// Dinh nghia lop khach hang
class Customer
{
private:
    std::string customerID;
    std::string name;
    std::string phone;

public:
    Customer(std::string id = "", std::string n = "", std::string p = "");
    std::string getID() const;
    std::string getName() const;
    std::string getPhone() const;
    void display() const;
};

struct HashNode
{
    Customer data;
    HashNode *next;
    HashNode(Customer c);
};

class CustomerTable
{
private:
    static const int SIZE = 101;
    HashNode *buckets[SIZE];
    int hashFunc(std::string id);

public:
    CustomerTable();
    ~CustomerTable();
    void addCustomer(Customer c);
    Customer *findCustomer(std::string id);
    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
};

#endif