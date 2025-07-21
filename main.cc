#include"hcJson.h"
#include<iostream>
#include<chrono>
#include<fstream>
int main(){
  auto start = std::chrono::steady_clock::now();
  //这是序列化测试案例
  /*std::string json=R"({
  "meta": {
    "schemaVersion": "4.2.0-RC3",
    "generatedUtc": "2025-07-13T21:08:55.000Z",
    "checksum": "a1b2c3d4e5f6789ab",
    "source": {
      "type": "genera\ntor",
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
})";*/


std::string json=R"({
  "id": "f81d4fae-7dec-11d0-a765-00a0c91e6bf6",
  "meta": {
    "created": "2025-07-18T14:33:27.142Z",
    "lastEvent": 17502,
    "version": [2, 5, 0, "rc.4"],
    "authors": [
      { "name": "5434", "email": "ivanov@example.com", "lang": "ru-RU" },
      { "name": "teggd", "email": "yamada@example.jp", "lang": "ja-JP" }
    ],
    "tags": { "main": ["complex", "deep"], "extra": null }
  },
  "config": {
    "features": {
      "regex": "^(?=(.*[A-Z]){2,})(?=(.*\\d){3,})(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{12,}$",
      "nested": {
        "level1": {
          "level2": {
            "level3": {
              "uuidMap": {
                "550e8400-e29b-41d4-a716-446655440000": { "weight": 3.1415 },
                "6ba7b810-9dad-11d1-80b4-00c04fd430c8": { "weight": 2.718 }
              }
            }
          }
        }
      }
    },
    "flags": [true, false, true, null, false],
    "geo": {
      "type": "FeatureCollection",
      "features": [
        {
          "type": "我的发",
          "geometry": {
            "type": "Point",
            "coordinates": [139.6917, 35.6895, 42.6]
          },
          "properties": {
            "name": "Tokyo Tower",
            "images": [
              {
                "type": "jpg",
                "data": "/9j/4AAQSkZJRgABAQAAAQ...==",
                "size": 83750
              }
            ]
          }
        }
      ]
    },
    "i18n": {
      "en": { "title": "Complex JSON Example", "desc": "An extremely convoluted JSON document." },
      "zh-Hans": { "title": "SON", "desc": "JSON" }
    }
  },
  "data": {
    "matrix": [
      [1, 0, 0],
      [0, 1, 0],
      [0, 0, 1]
    ],
    "hugeInt": 123456,
    "floats": { "positive": 1e+42, "negative": -1.23e-45, "nan": "NaN", "inf": "Infinity" },
    "bin": {
      "sha256": "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
      "blob": {
        "$type": "base64",
        "$value": "H4sIAI2/ZW4C/8tIzcnJBwCGphA2BQAAAA=="
      }
    },
    "selfRef": { "$ref": "#/meta/authors/0" },
    "circular": {
      "a": { "b": { "c": null } }
    }
  },
  "extensions": {
    "x-custom-telemetry": {
      "events": [
        { "t": 175024, "type": "click", "pos": [1024, 768] },
        { "t": 17502, "type": "keydown", "key": "Escape" }
      ]
    },
    "x-openapi": {
      "openapi": "3.1.0",
      "paths": {
        "/complex/{id}": {
          "get": {
            "parameters": [
              {
                "name": "id",
                "in": "path",
                "required": true,
                "schema": { "type": "string", "format": "uuid" }
              }
            ],
            "responses": {
              "200": {
                "description": "Returns the very JSON you are reading",
                "content": {
                  "application/json": {
                    "schema": { "$ref": "#" }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
})";
    //hcc::Parser a{std::ifstream("/home/hcc/share/study/my/test/hcJson/json.txt",std::ios::binary)};
    hcc::Parser a{json};
    //std::cout<<sizeof(hcc::JsonValue)<<'\n';
    /*hcc::JsonRoot root1=a.parse();

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
    

    hcc::JsonRoot root=std::move(obj_root);
    std::string s=root.toJson();
    std::cout<<s<<'\n';*/

    //0x2d66b093f20

    /*std::cout<<"n:"<<hcc::globalMemPool.blocks.size()<<'\n';
    std::cout<<"ddddd:"<<(void*)(hcc::globalMemPool.blocks[0]->buf)<<'\n';*/
    //hcc::Parser a{json};
    hcc::JsonRoot root1=a.parse();
    /*if(root1.isObject()){
      hcc::JsonObject obj1=root1.toObject();
      if(obj1["id"].isString()){
        std::string s=obj1["id"].toString();
        std::cout<<s<<'\n';
      }
    }*/
    //std::cout<<root1<<'\n';
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