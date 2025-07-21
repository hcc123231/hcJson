#pragma once
#include<vector>
#include<iostream>
#include<unordered_map>
#include<string>
#include <variant>
#include <memory> 

/*
凡是涉及到将JsonObject或者JsonArray赋值到另一个对象的操作时，都要使用std::move将原本的对象转为右值
暂不支持\u
记得使用-std=c+17 -O2来编译
因为本项目的目的就是提高效率并且受限于g++编译器的版本，所以选择使用unique_ptr作为json值的类型，那么必然导致
不可拷贝，转而使用移动，所以可能会出现值已被移动但是用户还要使用这个变量的情况，下面两个注意点牢记
在任何toObject,toArray,toString等等函数调用后谨慎使用std::cout<<JsonRoot;
在toJson函数调用后谨慎使用JsonRoot
*/
namespace hcc{
    
    void callerInfo(int line);
    #define PRINT_CALL_INFO() callerInfo(__LINE__)
    class JsonObject;
    class JsonArray;
    class JsonValue;
    using Array = std::vector<std::unique_ptr<JsonValue>>;
    using Object = std::unordered_map<std::string, std::unique_ptr<JsonValue>>;

    class JsonObject{
        friend class JsonValue;
        friend class JsonArray;
        friend class JsonRoot;
    private:
        Object _obj;
    public:
        
        JsonObject(const Object&)=delete;
        JsonObject()=default;
        JsonObject(Object&& right);
    public:
        inline Object transferAuthority();
        size_t size()const{return _obj.size();}
        JsonValue& operator[](const std::string& str);
        auto begin()  -> decltype(_obj.begin()) {return _obj.begin();}

        auto end()  -> decltype(_obj.end()) {return _obj.end();}
    };

    class JsonArray{
        friend class JsonObject;
        friend class JsonRoot;
        friend class JsonValue;
    private:
        Array _array;
    public:
        inline Array transferAuthority();
        JsonArray(const Array&)=delete;
        JsonArray(Array&& right);
        JsonArray()=default;
        ~JsonArray();
    public:
        inline JsonValue& operator[](size_t index);
        void push_back(JsonValue&& value){
            _array.push_back(std::make_unique<JsonValue>(std::move(value)));
        }
        void push_back(){
            _array.push_back(std::make_unique<JsonValue>());
        }
        void push_back(int value){
            _array.push_back(std::make_unique<JsonValue>(value));
        }
        void push_back(double value){
            _array.push_back(std::make_unique<JsonValue>(value));
        }
        void push_back(bool value){
            _array.push_back(std::make_unique<JsonValue>(value));
        }
        void push_back(std::string value){
            _array.push_back(std::make_unique<JsonValue>(value));
        }
        void push_back(JsonObject&& value){
            _array.push_back(std::make_unique<JsonValue>(std::move(value._obj)));
        }
        void push_back(JsonArray&& value){
            _array.push_back(std::make_unique<JsonValue>(std::move(value._array)));
        }
        //void pop_back();
        inline auto begin() ->decltype(_array.begin()){return _array.begin();}
        inline auto end() ->decltype(_array.end()){return _array.end();}
    };
    enum JsonValueTypeEnum{
        TYPE_NULL=0,
        TYPE_STRING,
        TYPE_BOOL,
        TYPE_INT,
        TYPE_DOUBLE,
        TYPE_ARRAY,
        TYPE_OBJECT,
        TYPE_ERROR
    };
    class JsonValue {
        using ValueType = std::variant<std::monostate,std::string,bool, int, double, Array, Object>;
    private:
        ValueType _value;
    public:
        JsonValue();
        explicit JsonValue(bool value);
        explicit JsonValue(int value);
        explicit JsonValue(double value);
        explicit JsonValue(std::string value);
        explicit JsonValue(const char* s);
        explicit JsonValue(Array value);
        explicit JsonValue(Object value);
        explicit JsonValue(JsonValue&& value);
        ~JsonValue();
    public:
        inline void operator=(const std::string& value){_value=value;}
        inline void operator=(const char* value){_value=std::string{value};}
        inline void operator=(const int& value){_value=value;}
        inline void operator=(const bool& value){_value=value;}
        inline void operator=(const double& value){_value=value;}
        inline void operator=(Array&& value){_value=std::move(value);}
        inline void operator=(Object&& value){_value=std::move(value);}
        inline void operator=(JsonObject&& value){_value=std::move(value._obj);}
        inline void operator=(JsonArray&& value){_value=std::move(value._array);}
        inline void operator=(JsonValue&& value){_value=std::move(value._value);}
        void* operator new(size_t size);
        void operator delete(void* ptr);
    public:
        bool isNull()const{return std::holds_alternative<std::monostate>(_value);}
        bool isBool()const{return std::holds_alternative<bool>(_value);}
        bool isInt()const{return std::holds_alternative<int>(_value);}
        bool isDouble()const{return std::holds_alternative<double>(_value);}
        bool isString()const{return std::holds_alternative<std::string>(_value);}
        bool isArray()const{return std::holds_alternative<Array>(_value);}
        bool isObject()const{return std::holds_alternative<Object>(_value);}
    public:
        //获取到当前_value实际类型
        inline JsonValueTypeEnum theType()const;
        std::string getTypeString()const;
    public:
        std::string toString()const{
            if(!isString()){
                std::string error_str=std::string{"the value's type is not std::string but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            std::string s=std::get<std::string>(_value);
            bool escape=false;
            std::string result="";
            for(size_t i=0;i<s.size();i++){
                char c=s[i];
                if(escape){
                    switch (c)
                    {
                    case '\\':result+='\\';break;
                    case '"':result+='"';break;
                    case '/':result+='/';break;
                    case 'b':result+='\b';break;
                    case 'f':result+='\f';break;
                    case 'n':result+='\n';break;
                    case 'r':result+='\r';break;
                    case 't':result+='\t';break; 
                    case 'u':std::cout<<"has \\u\n";     
                    default:result+="\\";result+=c;break;
                    }
                    escape=false;
                }
                else if(c=='\\'){
                    escape=true;
                }
                else{
                    result+=c;
                }
            }
            return result;
        }
        std::string toNull()const{
            if(!isNull()){
                std::string error_str=std::string{"the value's type is not null but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            return "null";
        }
        bool toBool()const{
            if(!isBool()){
                std::string error_str=std::string{"the value's type is not bool but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            return std::get<bool>(_value);
        }
        int toInt()const{
            if(!isInt()){
                std::string error_str=std::string{"the value's type is not int but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            return std::get<int>(_value);
        }
        double toDouble()const{
            if(!isDouble()){
                std::string error_str=std::string{"the value's type is not double but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            return std::get<double>(_value);
        }
        JsonObject toObject()const{
            if(!isObject()){
                std::string error_str=std::string{"the value's type is not Object but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            return JsonObject{std::move(const_cast<Object&>(std::get<Object>(_value)))};
        }
        JsonArray toArray()const{
            if(!isArray()){
                std::string error_str=std::string{"the value's type is not Array but "}+getTypeString();
                throw std::runtime_error(error_str);
            }
            return JsonArray{std::move(const_cast<Array&>(std::get<Array>(_value)))};
        }
    };
    enum tokenType{
        TOKEN_STRING=0,
        TOKEN_LITERAL,
        TOKNE_INIT,
        TOKEN_SYMBOL
    };

    class Token{
    private:
        tokenType _type;
        std::string_view _value;
    public:
        Token(tokenType type,std::string_view value):_type{type},_value{value}{}
        Token():_type{TOKNE_INIT},_value{""}{};
    public:
        inline bool empty()const {return _value.empty();}
        inline std::string_view getValue()const {return _value;}
        inline tokenType getType()const {return _type;}
    };

    //将json字符串中的有意义词分离开
    class TokenAnalyzer{
    public:
        std::string_view _jsonStr;
        size_t _strSize;
        size_t _pos;//记录当前位置
    public:
        explicit TokenAnalyzer(std::string_view str);
        TokenAnalyzer()=default;
        ~TokenAnalyzer();
    public:
       void operator=(const std::string_view str);
       void operator=(std::string&& str);
       std::string_view getString(){return _jsonStr;}
    public:
        inline Token getNextToken();//得到下一个token
        inline void skipSpace();
        inline void positionAdd();//pos位置+1
        std::string_view extractNumber();//提取json字符串中的数字
        inline bool outRange();
        std::string_view extractString();//提取json中的字符串
    };



    class Parser{
    private:
        std::string _fileContentStr;
        TokenAnalyzer _tokenAnalyzer;//解析器包含一个token提词器完成token的分离提取
        Token _curToken;//还需要一个Token容器记录下当前token内容
    public:
        std::unique_ptr<JsonValue> parse();
        std::unique_ptr<JsonValue> parseValue();
    public:
        explicit Parser(std::string_view jsonStr);
        //文件初始化
        Parser(std::ifstream&& file);

    public:
        inline Token nextToken();
        std::unique_ptr<JsonValue> parseObject();
        std::unique_ptr<JsonValue> parseArray();
        std::unique_ptr<JsonValue> parseNumber();
        
        inline void consume(std::string_view expected);
    };

    class Printer;
    
    class JsonRoot{
    private:
        std::unique_ptr<JsonValue> _root;
    public:
        JsonRoot(std::unique_ptr<JsonValue>&& right);
        JsonRoot(JsonObject&& right);
        ~JsonRoot();
        void operator=(std::unique_ptr<JsonValue>&& right);
    public:
        std::string toJson();
        std::string toJsonFunc(std::unique_ptr<JsonValue> value);
        bool isObject()const{return _root->isObject();}
        JsonObject toObject()const{
            return _root->toObject();
        }
        std::unique_ptr<JsonValue> getValue();
        
    };
    std::ostream& operator<<(std::ostream& os,JsonRoot& jroot);

    class Printer{
    private:
        
    public:
        std::unique_ptr<JsonValue> _value;
        Printer(JsonRoot& root);
        std::unique_ptr<JsonValue> print(std::ostringstream& oss);
        std::unique_ptr<JsonValue> print_value(std::unique_ptr<JsonValue> value,size_t n,std::ostringstream& oss);
        void pspace_count(size_t n,std::ostringstream& oss);
    };
    
}



