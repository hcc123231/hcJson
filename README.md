# hcJson
这是基于c++17写的一个json解析器项目，支持序列化与反序列化。

* 凡是涉及到将JsonObject或者JsonArray赋值到另一个对象的操作时，都要使用std::move将原本的对象转为右值

  暂不支持中文以及多字节表示的字符

* 记得使用-std=c+17来编译

* 因为本项目的目的就是提高效率并且受限于g++编译器的版本，所以选择使用unique_ptr作为json值的类型，那么必然导致

  不可拷贝，转而使用移动，所以可能会出现值已被移动但是用户还要使用这个变量的情况，下面两个注意点牢记

* 在任何toObject和toArray函数调用后谨慎使用std::cout<<JsonRoot;

* 在toJson函数调用后谨慎使用JsonRoot

