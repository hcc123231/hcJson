#include"hcJson.h"
#include<iostream>  
#include<cctype>
#include<stdexcept>
#include <sstream>
#include<fstream>
#include <charconv>
#include"memPool.h"
using namespace hcc;
void* JsonValue::operator new(size_t size){return gMemPool().alloc();}
void JsonValue::operator delete(void* ptr){gMemPool().dealloc(ptr);}
JsonValue::JsonValue():_value{std::monostate{}}{}
JsonValue::JsonValue(bool value):_value{value}{}
JsonValue::JsonValue(int value):_value{value}{}
JsonValue::JsonValue(double value):_value{value}{}
JsonValue::JsonValue(std::string value):_value{value}{}
JsonValue::JsonValue(const char* s):_value{std::string{s}}{}
JsonValue::JsonValue(Array value):_value{std::move(value)}{}
JsonValue::JsonValue(Object value):_value{std::move(value)}{}
JsonValue::JsonValue(JsonValue&& value):_value{std::move(value._value)}{}
JsonValue::~JsonValue()=default;
TokenAnalyzer::TokenAnalyzer(std::string_view str):_jsonStr{str},_pos{0},_strSize{str.size()}{}
void TokenAnalyzer::operator=(const std::string_view str){
    _jsonStr=str;
    _strSize=_jsonStr.size();
    _pos=0;
}
void TokenAnalyzer::operator=(std::string&& str){
    if(str!=_jsonStr){
        std::string_view sv{str};
        _jsonStr=std::move(sv);
        _strSize=_jsonStr.size();
        _pos=0;
    }
}
TokenAnalyzer::~TokenAnalyzer()=default;

JsonObject::JsonObject(Object&& right):_obj{std::move(right)}{}

JsonArray::JsonArray(Array&& right):_array{std::move(right)}{}
Array JsonArray::transferAuthority(){
    return std::move(_array);
}
JsonArray::~JsonArray()=default;
JsonRoot::JsonRoot(std::unique_ptr<JsonValue>&& right):_root{std::move(right)}{}
JsonRoot::JsonRoot(JsonObject&& right):_root{std::move(std::make_unique<JsonValue>(std::move(right._obj)))}{

}
JsonRoot::~JsonRoot()=default;
JsonValueTypeEnum JsonValue::theType()const{
    if(std::holds_alternative<std::monostate>(_value))return TYPE_NULL;
    if(std::holds_alternative<std::string>(_value))return TYPE_STRING;
    if(std::holds_alternative<bool>(_value))return TYPE_BOOL;
    if(std::holds_alternative<int>(_value))return TYPE_INT;
    if(std::holds_alternative<double>(_value))return TYPE_DOUBLE;
    if(std::holds_alternative<Array>(_value))return TYPE_ARRAY;
    if(std::holds_alternative<Object>(_value))return TYPE_OBJECT;
    return TYPE_ERROR;
}
std::string JsonValue::getTypeString()const{
    JsonValueTypeEnum res=theType();
    switch(res){
        case 0:return "null";
        case 1:return "std::string";
        case 2:return "bool";
        case 3:return "int";
        case 4:return "double";
        case 5:return "std::vector<std::unique_ptr<JsonValue>>";
        case 6:return "std::unordered_map<std::string, std::unique_ptr<JsonValue>>";
    }
    return "type error";
}

Token TokenAnalyzer::getNextToken(){
    skipSpace();//先跳过所有的空白字符
    //再判断此时的_pos是否已经出界
    if(_pos>=_jsonStr.size())return Token{TOKEN_STRING,std::string_view{""}};
    char c=_jsonStr[_pos];
    //当token是单字符
    if(c=='{'||c=='}'||c=='['||c==']'||c==','||c==':'){
        std::string_view s{&_jsonStr[_pos],1};
        positionAdd();
        return Token{TOKEN_SYMBOL,s};
    }
    //当token是字面值，null,false,true
    if(c=='n'||c=='f'||c=='t'){
        size_t start=_pos;
        while(isalpha(_jsonStr[_pos]))positionAdd();
        
        return Token{TOKEN_LITERAL,_jsonStr.substr(start,_pos-start)};
    }
    //当token是数字时,那么一定是以数字或者'-'号或者'.'开头的
    if(isdigit(c)||c=='-'||c=='.'){
        
        return Token{TOKEN_LITERAL,extractNumber()};
    }
    //最后是解析字符串,字符串以"开头
    if(c=='"'){
        //std::string oo=extractString();
        std::string_view s=extractString();
        positionAdd();
        return Token{TOKEN_STRING,s};
    }else{
        std::cout<<"type error:"<<c<<'\n';
        printf("c:%d\n",c);
        throw std::runtime_error("the type is error");
    }
}
void TokenAnalyzer::skipSpace(){
    while(!outRange()&&isspace(_jsonStr[_pos])){
        positionAdd();
    }
}
void TokenAnalyzer::positionAdd(){
    if(_pos>=_jsonStr.size()){
        throw std::out_of_range("The pos of _jsonStr moved out of range!");
    }
    ++_pos;
}
bool TokenAnalyzer::outRange(){
    if(_pos>=_strSize)return true;
    return false;
}
//数字有[123,-456,0.123,.456,1.23e10,-4.56E-10]这些表达方式，但是暂时不考虑.456这种形式
/*
    那么可见数字分为7个部分，第一部分是开头的'-'号，第二部分是整数部分，
    第三部分是小数点'.',第四部分是小数部分,第五部分是指数e,E，第六部分
    是指数后面的'-'号，第七部分是指数或者'-'号后面的数字部分
*/
std::string_view TokenAnalyzer::extractNumber(){
    size_t start_pos=_pos;//记录下一开始的pos
    //如果说数字的第一个位置上放的是'-'，那么可以直接跳过
    if(!outRange()&&_jsonStr[_pos]=='-')positionAdd();
    //接下来是整数部分
    while(!outRange()&&isdigit(_jsonStr[_pos]))positionAdd();
    //小数部分
    if(!outRange()&&_jsonStr[_pos]=='.'){
        positionAdd();
        while(!outRange()&&isdigit(_jsonStr[_pos]))positionAdd();
    }
    //指数部分
    if(!outRange()&&(_jsonStr[_pos]=='e'||_jsonStr[_pos]=='E')){
        positionAdd();
        if(!outRange()&&(_jsonStr[_pos]=='-'||_jsonStr[_pos]=='+')){
            positionAdd();
        }
        while(!outRange()&&isdigit(_jsonStr[_pos]))positionAdd();
    }
    return _jsonStr.substr(start_pos,_pos-start_pos);
}
/*
提取字符串有一个限制，当字符串中含有'\'这个转义符时，可能会发生转义，所以我们需要对这些转义符进行额外处理
*/
std::string_view TokenAnalyzer::extractString(){
    /*positionAdd();
    std::string result;
    bool escape=false;//转义状态标识符
    while(!outRange()){
        char c=_jsonStr[_pos];
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
        else if(c=='"'){
            return result;
        }
        else if(c=='\\'){
            escape=true;
        }
        else{
            result+=c;
        }
        positionAdd();
    }
    return "";*/
    positionAdd();
    //std::string result;
    bool escape=false;//转义状态标识符
    size_t start=_pos;
    while(!outRange()){
        char c=_jsonStr[_pos];
        if(escape){
            escape=false;
        }
        else if(c=='"'){
            return _jsonStr.substr(start,_pos-start);
        }
        else if(c=='\\'){
            escape=true;
        }
        positionAdd();
    }
    return "";
}
//将下一个token赋值给_curToken
Token Parser::nextToken(){
    _curToken=_tokenAnalyzer.getNextToken();
    return _curToken;
}

Parser::Parser(std::string_view jsonStr):_tokenAnalyzer{jsonStr}{
    //初始化完成后立即获取第一个token内容
    nextToken();
    if(_curToken.getValue()!="{"){
        throw std::runtime_error("expected \"{\"");
    }
}

Parser::Parser(std::ifstream&& file){
    if(!file.is_open()){
        throw std::runtime_error("file can not be opened");
    }
    file.seekg(0,std::ios::end);
    size_t len=file.tellg();
    file.seekg(0,std::ios::beg);
    
    _fileContentStr.reserve(len);
    _fileContentStr.assign(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
    _tokenAnalyzer=std::move(_fileContentStr);
    file.close();
    nextToken();
    if(_curToken.getValue()!="{"){
        throw std::runtime_error("expected \"{\"");
    }
}
std::unique_ptr<JsonValue> Parser::parse(){
    std::unique_ptr<JsonValue> ptr=parseValue();
    return std::move(ptr);
}
//解析json获取值部分
std::unique_ptr<JsonValue> Parser::parseValue(){
    if(_curToken.empty()){
        throw std::invalid_argument("The param is empty");
    }
    //根据token类型进行不同的解析操作
    if(_curToken.getValue()=="{"){
        //这是对象
        return parseObject();
    }
    else if(_curToken.getValue()=="["){
        //解析数组
        return parseArray();
    }
    else if(_curToken.getValue()=="true"&&_curToken.getType()==TOKEN_LITERAL){
        nextToken();
        return std::make_unique<JsonValue>(true);
    }
    else if(_curToken.getValue()=="false"&&_curToken.getType()==TOKEN_LITERAL){
        nextToken();
        return std::make_unique<JsonValue>(false);
    }
    else if(_curToken.getValue()=="null"&&_curToken.getType()==TOKEN_LITERAL){
        nextToken();
        return std::make_unique<JsonValue>();
    }
    //数字
    else if((_curToken.getValue()[0]=='-'||isdigit(_curToken.getValue()[0]))&&_curToken.getType()==TOKEN_LITERAL){
        return parseNumber();
    }
    //默认为字符串
    else if(_curToken.getType()==TOKEN_STRING){
        std::string s{_curToken.getValue()};
        nextToken();

        return std::make_unique<JsonValue>(s);
    }
    else{
        throw std::runtime_error("the char is not correct");
    }
}
//消费掉当前expectedToken，并且将下一个token赋值给_curToken，那么能起到一个字符串检查和_curToken更新的作用
void Parser::consume(std::string_view expectedToken){
    
    if(expectedToken!=_curToken.getValue()){
        std::string error=std::string("expectedToken is not match _curToken: ")+std::string("expected \"")+std::string(expectedToken)+std::string("\" but the _curToken is \"")+std::string(_curToken.getValue())+std::string("\"");
        throw std::runtime_error(error);
    }
    nextToken();
}

std::unique_ptr<JsonValue> Parser::parseNumber(){
    std::string num_str{_curToken.getValue()};
    nextToken();
    //先解析为int类型，然后如果不符合就解析为double类型
    size_t size=0;
    int num=0;
    auto [ptr,ec]=std::from_chars(num_str.data(),num_str.data()+num_str.size(),num);
    if (ec == std::errc::invalid_argument) {
        std::cout << "invalid_argument" << std::endl;
    } else if (ec == std::errc::result_out_of_range) {
        std::cout << "result_out_of_range" << std::endl;
    }
    
    if(size==num_str.size()&&num>=std::numeric_limits<int>::min()&&num<=std::numeric_limits<int>::max()){
        return std::make_unique<JsonValue>(num);
    }
    double db_num=std::stod(num_str);
    
    return std::make_unique<JsonValue>(db_num);
}
std::unique_ptr<JsonValue> Parser::parseObject(){
    consume("{");
    Object obj;
    //只有当_curToken不等于'}'时才能继续对这个对象操作
    if(_curToken.getValue()!="}"){

        //一进来必须是键，键的类型就只能是字符串类型
        //如果_curToken为空，说明键也找不到，那么就是出问题了
        do{
            if(_curToken.empty()){
                throw std::runtime_error("json object expecte a key");
            }
            if(_curToken.getType()!=TOKEN_STRING){
                //必须要求键是string类型的
                throw std::runtime_error("the key is not a string");
            }
            std::string key{_curToken.getValue()};
            nextToken();
            consume(":");
            std::unique_ptr<JsonValue> value=parseValue();
            obj[key]=std::move(value);
        }while(_curToken.getValue()!="}"&&(consume(","),true));
    }
    consume("}");
    return std::make_unique<JsonValue>(std::move(obj));
}
std::unique_ptr<JsonValue> Parser::parseArray(){
    Array array;
    consume("[");
    if(_curToken.getValue()!="]"){
        do{
            array.push_back(parseValue());
        }while(_curToken.getValue()==","&&(consume(","),true));
    }
    consume("]");
    return std::make_unique<JsonValue>(std::move(array));
}
JsonValue& JsonObject::operator[](const std::string& str){
    auto it = _obj.find(str);
    if (it == _obj.end()) {
        //键不存在就插入一个新的空JsonValue
        _obj[str] = std::make_unique<JsonValue>();
        return *_obj[str];
    }
    return *it->second; 
}

JsonValue& JsonArray::operator[](size_t index){
    return *(_array[index]);
}
void hcc::callerInfo(int line){
    std::cout<<"call line:"<<line<<'\n';
}
Printer::Printer(JsonRoot& root){
    _value=root.getValue();
}

std::unique_ptr<JsonValue> JsonRoot::getValue(){
    return std::move(_root);
}
void JsonRoot::operator=(std::unique_ptr<JsonValue>&& right){
    _root=std::move(right);
}
std::string JsonRoot::toJson(){
    std::string s=toJsonFunc(std::move(_root));
    return s;
}
std::string JsonRoot::toJsonFunc(std::unique_ptr<JsonValue> value){
    std::string s;
    if(value->isObject()){
        JsonObject root_obj=value->toObject();
        s.push_back('{');
        
        for(auto it=root_obj.begin();it!=root_obj.end();it++){
            s.push_back('\"');
            s.append(it->first);
            s.append("\":");
            if(it->second->isObject()){
                s.append(toJsonFunc(std::move(it->second)));
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }
            else if(it->second->isArray()){
                s.append(toJsonFunc(std::move(it->second)));
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }else if(it->second->isInt()){
                s.append(std::to_string(it->second->toInt()));
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }else if(it->second->isDouble()){
                s.append(std::to_string(it->second->toDouble()));
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }
            else if(it->second->isBool()){
                s.append((it->second->toBool()==1?"true":"false"));
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }
            else if(it->second->isString()){
                s.push_back('\"');
                s.append(it->second->toString());
                s.push_back('\"');
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }else if(it->second->isNull()){
                s.append("null");
                if(std::next(it)!=root_obj.end()){
                    s.push_back(',');
                }
            }
        }
        s.push_back('}');
    }
    else if(value->isArray()){
        JsonArray array=value->toArray();
        s.push_back('[');
        for(auto i=array.begin();i!=array.end();i++){
            if(i->get()->isObject()){
                s.append(toJsonFunc(std::move(*i)));
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
            else if(i->get()->isArray()){
                s.append(toJsonFunc(std::move(*i)));
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
            else if(i->get()->isInt()){
                s.append(std::to_string(i->get()->toInt()));
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
            else if(i->get()->isDouble()){
                s.append(std::to_string(i->get()->toDouble()));
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
            else if(i->get()->isBool()){
                s.append((i->get()->toBool()==1?"true":"false"));
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
            else if(i->get()->isString()){
                s.push_back('\"');
                s.append(i->get()->toString());
                s.push_back('\"');
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
            else if(i->get()->isNull()){
                s.append("null");
                if(std::next(i)!=array.end()){
                    s.push_back(',');
                }
            }
        }
        s.push_back(']');
    }
    return s;
}
#define SPACE_COUNT 4
std::unique_ptr<JsonValue> Printer::print(std::ostringstream& oss){
    return print_value(std::move(_value),SPACE_COUNT,oss);
}
std::ostream& hcc::operator<<(std::ostream& os,JsonRoot& jroot){
    hcc::Printer printer=jroot;
    std::ostringstream oss;
    jroot=printer.print(oss);
    os<<oss.str();
    return os;
}

void Printer::pspace_count(size_t n,std::ostringstream& oss){
    for(int i=0;i<n;i++)oss<<' ';
}
#define OBJECT_SELECT() \
    do{\
        if(std::next(it)!=root_obj.end()){\
            oss<<",\n";\
            pspace_count(n,oss);\
        }\
        else{\
            oss<<"\n";\
            pspace_count(n-SPACE_COUNT,oss);\
        }\
    }while(0);
#define ARRAY_SELECT() \
    do{\
        if(std::next(i)!=array.end()){\
            oss<<",\n";\
            pspace_count(n,oss);\
        }\
        else{\
            oss<<"\n";\
            pspace_count(n-SPACE_COUNT,oss);\
        }\
    }while(0);

Object JsonObject::transferAuthority(){
    return std::move(_obj);
}
std::unique_ptr<JsonValue> Printer::print_value(std::unique_ptr<JsonValue> value,size_t n,std::ostringstream& oss){
    if(value->isObject()){
        JsonObject root_obj=value->toObject();
        oss<<"{\n";
        pspace_count(n,oss);
        for(auto it=root_obj.begin();it!=root_obj.end();it++){
            oss<<"\""<<it->first<<"\""<<": ";
            if(it->second->isObject()){
                it->second=print_value(std::move(it->second),n+SPACE_COUNT,oss);
                OBJECT_SELECT();
            }
            else if(it->second->isArray()){
                it->second=print_value(std::move(it->second),n+SPACE_COUNT,oss);
                OBJECT_SELECT();
            }else if(it->second->isInt()){
                oss<<it->second->toInt();
                OBJECT_SELECT();
            }else if(it->second->isDouble()){
                oss<<it->second->toDouble();
                OBJECT_SELECT();
            }
            else if(it->second->isBool()){
                oss<<(it->second->toBool()==1?"true":"false");
                OBJECT_SELECT();
            }
            else if(it->second->isString()){
                oss<<"\""<<it->second->toString()<<"\"";
                OBJECT_SELECT();
            }else if(it->second->isNull()){
                oss<<it->second->toNull();
                OBJECT_SELECT();
            }
        }
        *value=std::move(root_obj.transferAuthority());
        oss<<"}";
    }
    else if(value->isArray()){
        JsonArray array=value->toArray();
        oss<<"[\n";
        pspace_count(n,oss);
        for(auto i=array.begin();i!=array.end();i++){
            if(i->get()->isObject()){
                *i=print_value(std::move(*i),n+SPACE_COUNT,oss);
                ARRAY_SELECT();
            }
            else if(i->get()->isArray()){
                *i=print_value(std::move(*i),n+SPACE_COUNT,oss);
                ARRAY_SELECT();
            }
            else if(i->get()->isInt()){
                oss<<i->get()->toInt();
                ARRAY_SELECT();
            }
            else if(i->get()->isDouble()){
                oss<<i->get()->toDouble();
                ARRAY_SELECT();
            }
            else if(i->get()->isBool()){
                oss<<(i->get()->toBool()==1?"true":"false");
                ARRAY_SELECT();
            }
            else if(i->get()->isString()){
                oss<<"\""<<i->get()->toString()<<"\"";
                ARRAY_SELECT();
            }
            else if(i->get()->isNull()){
                oss<<i->get()->toNull();
                ARRAY_SELECT();
            }
        }
        *value=std::move(array.transferAuthority());
        oss<<"]";
    }
    return value;
}
