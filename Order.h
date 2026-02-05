#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

class Order
{
private:
    int orderID;
    std::string date;
    std::string customerID;
    double amount;

public:
    Order(int id = 0, std::string d = "", std::string cID = "", double amt = 0);
    int getOrderID() const;
    std::string getCustomerID() const;
    std::string getDate() const;
    double getAmount() const;
    void display() const;
};

struct BSTNode
{
    Order data;
    BSTNode *left, *right;
    BSTNode(Order o);
};

class OrderTree
{
private:
    BSTNode *root;
    BSTNode *insert(BSTNode *curr, Order o);
    void displayInOrder(BSTNode *curr);
    void clear(BSTNode *curr);
    void saveNode(BSTNode *node, std::ostream &out);
    double calculateTotal(BSTNode *node);

public:
    OrderTree();
    ~OrderTree();
    void addOrder(Order o);
    void showAllOrders();
    Order *search(int id);
    double getTotalRevenue();
    void saveToFile(std::string fileName);
    void loadFromFile(std::string fileName);
};

#endif