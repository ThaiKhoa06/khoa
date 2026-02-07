#include "Customer.h"
#include "Order.h"
#include <limits>

using namespace std;

void menu()
{
    cout << "\n========================================" << endl;
    cout << "       HE THONG QUAN LY DON HANG" << endl;
    cout << "========================================" << endl;
    cout << "1. Them khach hang moi" << endl;
    cout << "2. Them don hang moi" << endl;
    cout << "3. Tra cuu don hang" << endl;
    cout << "4. Danh sach don hang & Doanh thu" << endl;
    cout << "5. Xoa don hang" << endl;
    cout << "0. Luu du lieu va Thoat" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Lua chon cua ban: ";
}

int main()
{
    CustomerTable ct;
    OrderTree ot;

    const string custFile = "customers.csv";
    const string orderFile = "orders.csv";

    ct.loadFromFile(custFile);
    ot.loadFromFile(orderFile);

    int choice;
    do
    {
        menu();
        if (!(cin >> choice))
        {
            cout << "--> Vui long nhap so!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        // Them Khach Hang
        case 1:
        {
            string id, name, phone;
            cout << "Nhap ma KH (VD: KH01): ";
            getline(cin, id);
            
            // Kiem tra ma KH da ton tai chua
            if (ct.findCustomer(id))
            {
                cout << "--> Ma KH nay da ton tai trong he thong!" << endl;
            }
            else
            {
                cout << "Nhap ten KH: ";
                getline(cin, name);
                cout << "Nhap SDT: ";
                getline(cin, phone);
                ct.addCustomer(Customer(id, name, phone));
                cout << "--> Them khach hang thanh cong!" << endl;
            }
            break;
        }
        // Them Don Hang
        case 2:
        {
            int oid;
            string date, cid;
            double amt;

            cout << "Nhap ma DH (So nguyen): ";
            if (!(cin >> oid))
            {
                cout << "--> Ma don hang phai la so!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore();
            if (ot.search(oid))
            {
                cout << "--> Ma don hang da ton tai!" << endl;
                break;
            }

            cout << "Nhap ngay (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Nhap ma KH cua don nay: ";
            getline(cin, cid);

            Customer *c = ct.findCustomer(cid);
            if (c)
            {
                cout << "--> Da tim thay KH: " << c->getName() << endl;
                cout << "Nhap tong tien: ";
                cin >> amt;
                cin.ignore();

                ot.addOrder(Order(oid, date, cid, amt));
                cout << "--> Them don hang thanh cong!" << endl;
            }
            else
            {
                cout << "--> Loi: Ma KH '" << cid << "' khong ton tai! Vui long them KH truoc." << endl;
            }
            break;
        }
        // Tra cuu Don Hang
        case 3:
        {
            int oid;
            cout << "Nhap ma DH can tim: ";
            cin >> oid;
            Order *o = ot.search(oid);
            if (o)
            {
                cout << "\n[KET QUA TIM KIEM]" << endl;
                o->display();

                Customer *c = ct.findCustomer(o->getCustomerID());
                if (c)
                {
                    cout << "\n   -> Thong tin KH: ";
                    c->display();
                }
                else
                {
                    cout << "\n   -> (Thong tin KH khong con ton tai)";
                }
                cout << endl;
            }
            else
                cout << "--> Khong tim thay don hang so " << oid << "!" << endl;
            break;
        }
        // Hien thi tat ca don hang
        case 4:
            cout << "\n--- DANH SACH DON HANG ---" << endl;
            ot.showAllOrders();
            cout << "--------------------------" << endl;
            cout << "TONG DOANH THU: " << ot.getTotalRevenue() << "$" << endl;
            break;

        // Xoa don hang
        case 5:
        {
            int oid;
            cout << "Nhap ma don hang can xoa: ";
            cin >> oid;
            ot.deleteOrder(oid);
            break;
        }

        // Luu du lieu va Thoat
        case 0:
            ct.saveToFile(custFile);
            ot.saveToFile(orderFile);
            cout << "--> Da luu du lieu. Tam biet!" << endl;
            break;
        default:
            cout << "Chuc nang khong ton tai!" << endl;
        }
    } while (choice != 0);

    return 0;
}