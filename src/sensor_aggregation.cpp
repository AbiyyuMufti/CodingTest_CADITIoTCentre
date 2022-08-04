#include "utility.hpp"
#include <unordered_map>

int main()
{
    // Read Sensor Data from "data/sensor_data.json"
    std::ifstream file("../data/sensor_data.json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    // Parse to JSON Value
    Json::Value sensor_data_json;
    CadItCoding::ParseJson(buffer.str(), sensor_data_json);

    std::unordered_map<std::string, CadItCoding::DataAggregation> dataDayTimeTemperature;
    std::unordered_map<std::string, CadItCoding::DataAggregation> dataNightTimeTemperature;
    std::unordered_map<std::string, CadItCoding::DataAggregation> dataDayTimeHumidity;
    std::unordered_map<std::string, CadItCoding::DataAggregation> dataNightTimeHumidity;

    // Group JSON
    for (auto& elem : sensor_data_json["array"])
    {
        // Group by daytime
        uint32_t hour = CadItCoding::GetHourFromTimeStamp(elem["timestamp"].asDouble());
        // Group by room        
        std::string area = elem["roomArea"].asString();

        // Daytime is between 6 to 18
        if ((hour >= 6) && (hour < 18))
        {
            dataDayTimeTemperature[area].update(elem["temperature"].asDouble());
            dataDayTimeHumidity[area].update(elem["humidity"].asDouble());
        } 
        else //other than that is night time
        {
            dataNightTimeTemperature[area].update(elem["temperature"].asDouble());
            dataNightTimeHumidity[area].update(elem["humidity"].asDouble());
        }
    }

    Json::Value daytime;
    Json::Value nighttime;
    for (auto key : dataDayTimeHumidity)
    {
        daytime[key.first].append(dataDayTimeHumidity[key.first].makeJSON("Humidity"));
        daytime[key.first].append(dataDayTimeTemperature[key.first].makeJSON("Temperature"));
        nighttime[key.first].append(dataNightTimeHumidity[key.first].makeJSON("Humidity"));
        nighttime[key.first].append(dataNightTimeTemperature[key.first].makeJSON("Temperature"));        
    }

    Json::Value endpoint;
    endpoint["day"] = daytime;
    endpoint["night"] = nighttime;
    
    std::cout << endpoint.toStyledString() << std::endl;
    std::cout << " -------------------------------------- " << std::endl;

    CadItCoding::WriteJson("../result/sensor_aggregation.json", endpoint);

    return 0;
}