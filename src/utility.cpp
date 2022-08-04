#include "utility.hpp"
#include <curl/curl.h>
#include <vector>


static std::size_t read_callback(const char* in, std::size_t size, std::size_t num, std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}


void CadItCoding::FetchJson(const std::string& url, std::string& json_string, struct curl_slist *headers)
{
    CURL* curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    
    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    if (headers)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    // Use IPv4, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Response information.
    long httpCode(0);
    std::unique_ptr<std::string> resStr(new std::string());
    
    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, read_callback);

    // Hook up data container
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resStr.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
    {
        json_string = *resStr.get();
    }
    else
    {
        std::string errMsg = "Couldn't GET from " + url + " - exiting!\n";
        throw CadItCoding::FetchJSONError(errMsg);
    }

}


void CadItCoding::ParseJson(const std::string& text, Json::Value&   result)
{
    Json::Reader jsonReader;
    if (!jsonReader.parse(text, result))
    {
        std::string errMsg = "Could not parse following data as json:\n" + text;
        throw CadItCoding::ParseJSONError(errMsg);
    }
}


void CadItCoding::MergeJson(Json::Value& a, Json::Value& b)
{
    if (!a.isObject() || !b.isObject()) return;

    std::vector<std::string> member_name = b.getMemberNames();
    std::string key = "";
    for (unsigned i = 0, len = member_name.size(); i < len; i++) { 
        key = member_name[i];
        if (!a[key].isNull() && a[key].type() == Json::objectValue && b[key].type() == Json::objectValue) 
        {
            MergeJson(a[key], b[key]);
        } else {
            a[key] = b[key];
        }
    }
    member_name.clear();
}


void CadItCoding::WriteJson(const std::string& file, const Json::Value& endpoint)
{
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream(file);
    writer->write(endpoint, &outputFileStream);
}


double CadItCoding::GetCurrencyRate()
{
    std::string url = "https://currency-converter5.p.rapidapi.com/currency/convert?format=json&from=USD&to=IDR&amount=1";
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-RapidAPI-Key: d94bbd256bmshb2b49728f1b72adp1f2b7bjsnee133c74059a");
    headers = curl_slist_append(headers, "X-RapidAPI-Host: currency-converter5.p.rapidapi.com");
    std::string text;

    FetchJson(url, text, headers);

    Json::Value val;
    ParseJson(text, val);

    auto rates = val["rates"];
    auto IDR = rates["IDR"];
    auto rate = IDR["rate"];
    
    return std::stod(rate.asString());
}


uint32_t CadItCoding::GetHourFromTimeStamp(double timestamp)
{
    uint32_t days = (uint32_t)floor(timestamp / 86400);
    uint32_t hours = (uint32_t)floor((timestamp - days * 86400) / 3600) % 24;
    return hours;
}


void CadItCoding::DataAggregation::calculateMin(double input)
{
    if(empty || input < this->minimum)
    {
        this->minimum = input;
    }
}


void CadItCoding::DataAggregation::calculateMax(double input)
{
    if(empty || input > this->maximum)
    {
        this->maximum = input;
    }
}


void CadItCoding::DataAggregation::calculateAverage(double input)
{
    size++;
    sum += input;
    this->average = sum / size;
}


void CadItCoding::DataAggregation::calculateMedian(double input)
{
    if(empty)
    {
        this->median = input;
        s.push(input);
    }

    // case1(left side heap has more elements)
    if (s.size() > g.size())
    {
        if (input < this->median)
        {
            g.push(s.top());
            s.pop();
            s.push(input);
        }
        else
            g.push(input);

        this->median = (s.top() + g.top())/2.0;        
    }

    // case2(both heaps are balanced)
    else if (s.size()==g.size())
    {
        if (input < this->median)
        {
            s.push(input);
            this->median = (double)s.top();
        }
        else
        {
            g.push(input);
            this->median = (double)g.top();
        }
    }

    // case3(right side heap has more elements)
    else
    {
        if (input > this->median)
        {
            s.push(g.top());
            g.pop();
            g.push(input);
        }
        else
            s.push(input);

        this->median = (s.top() + g.top())/2.0;
    }
}


Json::Value CadItCoding::DataAggregation::makeJSON(std::string header)
{
    Json::Value data;
    data["min"] = this->minimum;
    data["max"] = this->maximum;
    data["median"] = this->median;
    data["average"] = this->average;
    Json::Value ret;
    ret[header] = data;
    return ret;
}


