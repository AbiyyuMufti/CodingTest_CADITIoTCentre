#include "utility.hpp"
#include <sstream>
#include <algorithm>
#include <boost/range/combine.hpp>


int main()
{
    
    // Fetch data from http://jsonplaceholder.typicode.com/users
    std::string from_url;
    CadItCoding::FetchJson("http://jsonplaceholder.typicode.com/users", from_url);
    
    // Parse to JSON Value
    Json::Value from_url_json;
    CadItCoding::ParseJson(from_url, from_url_json);

    // Read Salary Data from "data/salary_data.json"
    std::ifstream file("../data/salary_data.json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    // Parse to JSON Value
    Json::Value from_file_json;
    CadItCoding::ParseJson(buffer.str(), from_file_json);
    
    double RateUSDToIDR = CadItCoding::GetCurrencyRate();

    Json::Value endpoint;

    for (auto tup : boost::combine(from_url_json, from_file_json["array"]))
    {
        Json::Value a, b, res;
        boost::tie(a, b) = tup;
        CadItCoding::MergeJson(a, b);
        a.removeMember("website");
        a.removeMember("company");
        a["salaryInUSD"] = b["salaryInIDR"].asDouble() / RateUSDToIDR;

        endpoint.append(a);
    }
    
    std::cout << endpoint.toStyledString() << std::endl;
    std::cout << " -------------------------------------- " << std::endl;
    
    CadItCoding::WriteJson("../result/salary_converted.json", endpoint);


    return 0;
}