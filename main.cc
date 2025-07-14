#include"hcJson.h"
#include<iostream>
#include<chrono>
#include<fstream>
int main(){
  auto start = std::chrono::steady_clock::now();
  //这是序列化测试案例
  std::string json=R"({
  "meta": {
    "schemaVersion": "4.2.0-RC3",
    "generatedUtc": "2025-07-13T21:08:55.000Z",
    "checksum": "a1b2c3d4e5f6789ab",
    "source": {
      "type": "generator",
      "version": "1.0.0",
      "options": {
        "locale": "en-US",
        "timezone": "UTC",
        "precision": 6
      }
    }
  },
  "data": {
    "users": [
      {
        "id": "usr_7f8a9b0c",
        "identity": {
          "username": "stellar_dev",
          "emails": [
            {
              "address": "dev@stellar.io",
              "verified": true,
              "primary": true
            },
            {
              "address": "backup@stellar.io",
              "verified": false,
              "primary": false
            }
          ],
          "keys": {
            "public": "0x04f2a1b3c4d5e6f7a8b9c0d1e2f3a4b5c6d7e8f9a0b1c2d3e4f5a6b7c8d9e0f1a2",
            "fingerprint": "a1b2c3d4"
          }
        },
        "settings": {
          "ui": {
            "theme": "dark",
            "language": "en-US",
            "density": "compact",
            "animations": false
          },
          "notifications": {
            "push": {
              "general": true,
              "security": true,
              "marketing": false
            },
            "email": {
              "digest": "weekly",
              "frequency": 1,
              "exclude": ["promotions"]
            }
          }
        }
      },
      {
        "id": "usr_42e8f1a9",
        "identity": {
          "username": "quantum_lead",
          "emails": [
            {
              "address": "lead@quantum.io",
              "verified": true,
              "primary": true
            }
          ],
          "keys": {
            "public": "0x09e8a7b6c5d4e3f2a1b0c9d8e7f6a5b4c3d2e1f0",
            "fingerprint": "f0e1d2c3"
          }
        },
        "settings": {
          "ui": {
            "theme": "light",
            "language": "en-GB",
            "density": "comfortable",
            "animations": true
          },
          "notifications": {
            "push": {
              "general": false,
              "security": true,
              "marketing": true
            },
            "email": {
              "digest": "daily",
              "frequency": 3,
              "exclude": []
            }
          }
        }
      }
    ],
    "projects": [
      {
        "id": "proj_42e8f1a9",
        "name": "QuantumLedger",
        "repository": {
          "type": "git",
          "url": "https://github.com/stellar/quantum-ledger.git",
          "commit": "b5c6d7e8f9a0b1c2d3e4f5a6b7c8d9e0f1a2b3c4d",
          "branch": "main"
        },
        "environments": [
          {
            "name": "staging",
            "region": "us-west-2",
            "resources": {
              "compute": [
                {
                  "type": "t3.large",
                  "count": 2,
                  "labels": {
                    "tier": "api",
                    "env": "staging"
                  }
                },
                {
                  "type": "t3.medium",
                  "count": 1,
                  "labels": {
                    "tier": "worker",
                    "env": "staging"
                  }
                }
              ],
              "database": {
                "engine": "postgresql",
                "version": "15.4",
                "tier": "db.t3.micro",
                "backupWindow": "03:00-04:00",
                "encryption": true
              },
              "storage": {
                "buckets": [
                  {
                    "name": "ql-staging-assets",
                    "public": false,
                    "versioning": true,
                    "lifecycle": {
                      "deleteAfterDays": 90
                    }
                  }
                ]
              }
            }
          },
          {
            "name": "production",
            "region": "eu-central-1",
            "resources": {
              "compute": [
                {
                  "type": "c5.2xlarge",
                  "count": 6,
                  "labels": {
                    "tier": "api",
                    "env": "production"
                  }
                },
                {
                  "type": "c5.xlarge",
                  "count": 4,
                  "labels": {
                    "tier": "worker",
                    "env": "production"
                  }
                }
              ],
              "database": {
                "engine": "aurora-postgresql",
                "version": "15.4",
                "tier": "db.r6g.large",
                "replicas": 2,
                "backupRetention": 30,
                "encryption": true
              },
              "storage": {
                "buckets": [
                  {
                    "name": "ql-prod-assets",
                    "public": false,
                    "versioning": true,
                    "lifecycle": {
                      "transitionToIA": 30,
                      "deleteAfterDays": 2555
                    }
                  }
                ]
              }
            }
          }
        ],
        "dependencies": {
          "internal": [
            {
              "name": "auth-service",
              "constraint": "^2.1.0"
            },
            {
              "name": "billing-service",
              "constraint": "^1.5.3"
            }
          ],
          "external": [
            {
              "name": "stripe",
              "endpoint": "https://api.stripe.com/v1",
              "version": "2023-10-16"
            },
            {
              "name": "sendgrid",
              "endpoint": "https://api.sendgrid.com/v3",
              "version": "2023-05-15"
            }
          ]
        }
      }
    ]
  }
})";
    //hcc::Parser a{std::ifstream("C:\\Users\\hcc\\Desktop\\json.txt",std::ios::binary)};
    /*hcc::Parser a{json};
    hcc::JsonRoot root1=a.parse();

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Elapsed: " << elapsed << " ms\n";*/

    //------------------------------------------------------------
    //这是反序列化案例
    /*hcc::JsonObject obj_son1{};
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
    

    hcc::JsonRoot root=std::move(obj_root);*/
    //0x2d66b093f20

    /*std::cout<<"n:"<<hcc::globalMemPool.blocks.size()<<'\n';
    std::cout<<"ddddd:"<<(void*)(hcc::globalMemPool.blocks[0]->buf)<<'\n';*/
    hcc::Parser a{json};
    hcc::JsonRoot root1=a.parse();
    
    //hcc::globalMemPool.reset();
    /*std::cout<<"n:"<<hcc::globalMemPool.blocks.size()<<'\n';
    std::cout<<"qqqqqq:"<<(void*)(hcc::globalMemPool.blocks[0]->buf)<<'\n';*/


    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Elapsed: " << elapsed << " ms\n";

    /*std::string s=root.toJson();
    std::cout<<s<<'\n';*/
    return 0;
}