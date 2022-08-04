#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <stdexcept>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <bits/stdc++.h>

namespace CadItCoding
{
    void FetchJson(const std::string& url, std::string& json_string, struct curl_slist *headers=NULL);

    void ParseJson(const std::string& text, Json::Value& result);

    void MergeJson(Json::Value& a, Json::Value& b);

    void WriteJson(const std::string& file, const Json::Value& endpoint);

    double GetCurrencyRate();

    uint32_t GetHourFromTimeStamp(double timestamp);

    class DataAggregation
    {
        public:
            DataAggregation(){ empty=true;};
            ~DataAggregation(){};
            void update(double input)
            {
                calculateMin(input);
                calculateMax(input);
                calculateMedian(input);
                calculateAverage(input);
                empty = false;
            }
            double getMin() { return minimum; }
            double getMax() { return maximum; }
            double getMedian() { return median; }
            double getAverage() { return average; }
            Json::Value makeJSON(std::string header);

        private: 
            double minimum;
            double maximum;
            double median;
            double average;
            bool empty;         
            
            double sum;
            double size;

            // max heap to store the smaller half elements
            std::priority_queue<double> s;

            // min heap to store the greater half elements
            std::priority_queue<double, std::vector<double>, std::greater<double> > g;            

            void calculateMin(double input);
            void calculateMax(double input);
            void calculateMedian(double input);
            void calculateAverage(double input);
    };

    class CustomError : std::exception
    {
        protected:
            std::string text;
        public:
            CustomError(const std::string & msg) : text(msg) {}
            const char * what() { return text.c_str(); }
    };

    class FetchJSONError : public CustomError 
    {public: FetchJSONError(const std::string & msg): CustomError(msg){}};

    class ParseJSONError : public CustomError
    {public: ParseJSONError(const std::string & msg) : CustomError(msg) {}};    


};


#endif // __UTILITY_HPP__