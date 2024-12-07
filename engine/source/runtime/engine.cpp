#include <iostream>
#include <json11.hpp>

using namespace json11;

int main() {
    // 一个用于测试的 JSON 字符串
    std::string json_str = R"({
        "name": "John Doe",
        "age": 30,
        "is_student": false
    })";

    // 解析 JSON 字符串
    std::string err;
    Json json = Json::parse(json_str, err);

    // 检查是否解析错误
    if (!err.empty()) {
        std::cerr << "解析 JSON 时出错: " << err << std::endl;
        return 1;
    }

    // 访问解析后的 JSON 数据
    std::cout << "Name: " << json["name"].string_value() << std::endl;
    std::cout << "Age: " << json["age"].int_value() << std::endl;
    std::cout << "Is Student: " << json["is_student"].bool_value() << std::endl;

    return 0;
}