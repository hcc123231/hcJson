#include"hcJson.h"
#include<iostream>
#include<fstream>
int main(){
  //这是序列化测试案例
  std::string json=R"({
  "user": {
    "id": 12345,
    "name": "Alice Johnson",
    "email": "alice.johnson@example.com",
    "phone": "+1234567890",
    "address": {
      "street": "123 Main Street",
      "city": "Anytown",
      "state": "CA",
      "zip": "12345",
      "country": "USA"
    },
    "orders": [
      {
        "orderId": "ORD001",
        "orderDate": "2023-01-15",
        "totalAmount": 150.75,
        "items": [
          {
            "productId": "P001",
            "productName": "Laptop"
          },
          {
            "productId": "P002",
            "productName": "Mouse",
            "quantity": 2,
            "price": 25.00
          }
        ]
      },
      {
        "orderId": "ORD002",
        "orderDate": "2023-02-20",
        "totalAmount": 99.99,
        "items": [
          {
            "productId": "P003",

            "productName": "USB Cable",
            "quantity": 3,
            "price": 9.99
          }
        ]
      }
    ]
  }
})";
    //hcc::Parser a{std::ifstream("C:\\Users\\hcc\\Desktop\\json.txt",std::ios::binary)};
    hcc::Parser a{json};
    hcc::JsonRoot root1=a.parse();
    std::cout<<root1<<'\n';

    //------------------------------------------------------------
    //这是反序列化案例
    hcc::JsonObject obj_son1{};
    hcc::JsonArray arr1{};
    arr1.push_back(hcc::JsonValue());
    arr1.push_back(true);
    arr1.push_back();
    arr1.push_back(4);
    obj_son1["items"]=std::move(arr1);

    hcc::JsonArray arr2{};
    hcc::JsonObject obj_son2{};
    obj_son2["name"]="hcc";
    obj_son2["value"]=12;
    hcc::JsonObject obj_son3{};
    obj_son3["wo"]=hcc::JsonValue();
    obj_son3["age"]=13;
    arr2.push_back(std::move(obj_son2));
    arr2.push_back(std::move(obj_son3));

    hcc::JsonObject obj_root{};
    obj_root["field1"]=std::move(obj_son1);
    obj_root["field2"]=std::move(arr2);
    

    hcc::JsonRoot root=std::move(obj_root);
    std::string s=root.toJson();
    std::cout<<s<<'\n';
    return 0;
}