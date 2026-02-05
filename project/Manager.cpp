#include "Customer.h"
#include "Order.h"

// Ham trim khoang trang o dau va cuoi chuoi
void trim(std::string &s)
{
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    {
        s = "";
        return;
    }
    size_t last = s.find_last_not_of(" \t\r\n");
    s = s.substr(first, (last - first + 1));
}

// --- CustomerTable ---
Customer::Customer(std::string id, std::string n, std::string p)
    : customerID(id), name(n), phone(p) {}

std::string Customer::getID() const { return customerID; }
std::string Customer::getName() const { return name; }
std::string Customer::getPhone() const { return phone; }

void Customer::display() const
{
    std::cout << "[Ma KH: " << customerID << " | Ten: " << name << " | SDT: " << phone << "]";
}

// --- HashNode Implementation ---
HashNode::HashNode(Customer c) : data(c), next(nullptr) {}

// --- CustomerTable Implementation ---
CustomerTable::CustomerTable()
{
    for (int i = 0; i < SIZE; i++)
        buckets[i] = nullptr;
}

int CustomerTable::hashFunc(std::string id)
{
    unsigned long hash = 0;
    for (char c : id)
    {
        hash = (hash * 31) + c;
    }
    return hash % SIZE;
}

void CustomerTable::addCustomer(Customer c)
{
    int idx = hashFunc(c.getID());

    // Kiem tra trung ma
    HashNode *curr = buckets[idx];
    while (curr)
    {
        if (curr->data.getID() == c.getID())
        {
            std::cout << "--> Loi: Ma KH da ton tai!" << std::endl;
            return;
        }
        curr = curr->next;
    }

    // Them vao dau danh sach
    HashNode *newNode = new HashNode(c);
    newNode->next = buckets[idx];
    buckets[idx] = newNode;
}

Customer *CustomerTable::findCustomer(std::string id)
{
    int idx = hashFunc(id);
    HashNode *curr = buckets[idx];
    while (curr)
    {
        if (curr->data.getID() == id)
            return &(curr->data);
        curr = curr->next;
    }
    return nullptr;
}

// Giai phong bo nho
CustomerTable::~CustomerTable()
{
    for (int i = 0; i < SIZE; i++)
    {
        HashNode *curr = buckets[i];
        while (curr)
        {
            HashNode *toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }
    }
}

// Luu du lieu ra file
void CustomerTable::saveToFile(std::string fileName)
{
    std::ofstream out(fileName);
    if (!out)
        return;
    // Luu tieu de
    out << "MaKhachHang,TenKhachHang,SoDienThoai\n";
    for (int i = 0; i < SIZE; i++)
    {
        HashNode *curr = buckets[i];
        while (curr)
        {
            out << curr->data.getID() << "," << curr->data.getName() << "," << curr->data.getPhone() << "\n";
            curr = curr->next;
        }
    }
    out.close();
}

// Doc du lieu tu file
void CustomerTable::loadFromFile(std::string fileName)
{
    std::ifstream in(fileName);
    if (!in)
        return;
    std::string line, id, name, phone;
    bool firstLine = true;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        // Bo qua dong tieu de
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        std::stringstream ss(line);
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, phone, ',');

        // Trim khoang trang
        trim(id);
        trim(name);
        trim(phone);

        if (!id.empty())
            addCustomer(Customer(id, name, phone));
    }
    in.close();
}

// --- OrderTree ---

// --- Order Implementation ---
Order::Order(int id, std::string d, std::string cID, double amt)
    : orderID(id), date(d), customerID(cID), amount(amt) {}

int Order::getOrderID() const { return orderID; }
std::string Order::getCustomerID() const { return customerID; }
std::string Order::getDate() const { return date; }
double Order::getAmount() const { return amount; }

// Hien thi thong tin don hang
void Order::display() const
{
    std::cout << "DH: " << orderID << " | Ngay: " << date
              << " | Ma KH: " << customerID << " | Tien: " << amount;
}

// --- BSTNode Implementation ---
BSTNode::BSTNode(Order o) : data(o), left(nullptr), right(nullptr) {}

// --- OrderTree Implementation ---
OrderTree::OrderTree() : root(nullptr) {}

void OrderTree::addOrder(Order o) { root = insert(root, o); }

BSTNode *OrderTree::insert(BSTNode *curr, Order o)
{
    if (!curr)
        return new BSTNode(o);
    // Sap xep theo OrderID
    if (o.getOrderID() < curr->data.getOrderID())
        curr->left = insert(curr->left, o);
    else if (o.getOrderID() > curr->data.getOrderID())
        curr->right = insert(curr->right, o);
    else
        std::cout << "--> Loi: Ma don hang " << o.getOrderID() << " da ton tai!" << std::endl;

    return curr;
}

// Hien thi don hang theo thu tu tang dan
void OrderTree::displayInOrder(BSTNode *curr)
{
    if (!curr)
        return;
    displayInOrder(curr->left);
    curr->data.display();
    std::cout << std::endl;
    displayInOrder(curr->right);
}

// Hien thi tat ca don hang
void OrderTree::showAllOrders()
{
    if (!root)
        std::cout << "(Danh sach trong)" << std::endl;
    else
        displayInOrder(root);
}

// Tinh tong doanh thu
double OrderTree::calculateTotal(BSTNode *node)
{
    if (!node)
        return 0;
    return node->data.getAmount() + calculateTotal(node->left) + calculateTotal(node->right);
}

double OrderTree::getTotalRevenue() { return calculateTotal(root); }

// Tim don hang theo ID
Order *OrderTree::search(int id)
{
    BSTNode *curr = root;
    while (curr)
    {
        if (curr->data.getOrderID() == id)
            return &(curr->data);
        curr = (id < curr->data.getOrderID()) ? curr->left : curr->right;
    }
    return nullptr;
}

OrderTree::~OrderTree() { clear(root); }

// Giai phong bo nho cay nhi phan
void OrderTree::clear(BSTNode *curr)
{
    if (!curr)
        return;
    clear(curr->left);
    clear(curr->right);
    delete curr;
}

// Luu cay don hang ra file
void OrderTree::saveNode(BSTNode *node, std::ostream &out)
{
    if (!node)
        return;

    // Ghi Node hien tai
    out << node->data.getOrderID() << "," << node->data.getDate() << ","
        << node->data.getCustomerID() << "," << node->data.getAmount() << "\n";

    saveNode(node->left, out);
    saveNode(node->right, out);
}

// Luu du lieu don hang ra file
void OrderTree::saveToFile(std::string fileName)
{
    std::ofstream out(fileName);
    if (!out)
        return;
    // Luu tieu de
    out << "MaDonHang,NgayDatHang,MaKhachHang,TongTien\n";
    saveNode(root, out);
    out.close();
}

// Doc du lieu don hang tu file
void OrderTree::loadFromFile(std::string fileName)
{
    std::ifstream in(fileName);
    if (!in)
        return;
    std::string line, date, cID, tempID, tempAmt;
    bool firstLine = true;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        // Bo qua dong tieu de
        if (firstLine)
        {
            firstLine = false;
            continue;
        }
        std::stringstream ss(line);
        std::getline(ss, tempID, ',');
        std::getline(ss, date, ',');
        std::getline(ss, cID, ',');
        std::getline(ss, tempAmt, ',');

        // Trim khoang trang
        trim(tempID);
        trim(date);
        trim(cID);
        trim(tempAmt);

        if (!tempID.empty())
        {
            int oid = std::stoi(tempID);
            double amt = std::stod(tempAmt);
            addOrder(Order(oid, date, cID, amt));
        }
    }
    in.close();
}