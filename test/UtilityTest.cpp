#include <gtest/gtest.h>
#include "utility.hpp"
#include <iostream>
#include <numeric>
#include <chrono>


using namespace CadItCoding;

TEST(UtilityTest, WrongURLThrowFetchJSONException)
{
    std::string str;
    EXPECT_THROW(FetchJson("not_real_url",str), FetchJSONError);
}

TEST(UtilityTest, NotJSONFormatThrowParseJSONException)
{
    Json::Value val;
    EXPECT_THROW(ParseJson("not_real_json", val), ParseJSONError);
}

TEST(UtilityTest, ParseFromString)
{
    std::string to_parse = "{\n\"userId\": 1,\n\"id\": 1,\n\"title\": \"my_title\",\n\"completed\": false}";
    Json::Value val;
    ParseJson(to_parse, val);

    EXPECT_EQ(val["userId"], 1);
    EXPECT_EQ(val["id"], 1);
    EXPECT_EQ(val["title"], "my_title");
    EXPECT_EQ(val["completed"], false);

}


TEST(UtilityTest, MergeTest)
{
    Json::Value ob1, ob2, ob_combine;

    ob1["type"] = "A";
    ob1["text"] = "hello";

    ob2["type"] = "A";
    ob2["nr"] = 2;

    ob_combine["type"] = "A";
    ob_combine["text"] = "hello";
    ob_combine["nr"] = 2;

    MergeJson(ob1, ob2);

    EXPECT_EQ(ob_combine.compare(ob1), 0);
}


TEST(UtilityTest, ConvertHourTest)
{
    // 3 August 2022 15:34 GMT --> 1659540840
    double timestamp = 1659540840;
    uint32_t hour = GetHourFromTimeStamp(timestamp);
    std::cout << "hour: " <<hour << std::endl;
    EXPECT_EQ(hour, 15);
    // 3 August 2022 03:54 GMT --> 1659498840
    double timestamp2 = 1659498840;
    uint32_t hour2 = GetHourFromTimeStamp(timestamp2);
    std::cout << "hour: " <<hour2 << std::endl;
    EXPECT_EQ(hour2, 3);
}


class DataAggregationTest : public ::testing::Test
{
    public:
        DataAggregation D;
        std::vector<double> set_data = {2.0, 4.5, 5.0, 3.5, 5.5, 9.0, 3.0, 8.0, 7.5, 2.5, 8.0, 5.0, 4.5, 5.5};
        DataAggregation E;
        std::vector<double> set_data2 = {14.5, 15.0, 13.5, 15.5, 19.0, 13.0, 18.0, 17.5, 12.5, 18.0, 15.0, 14.5, 15.5};
    protected:
        void SetUp() override {
            for (auto& elem : set_data){
                D.update(elem);
            }
            for (auto& elem : set_data2)
            {
                E.update(elem);
            }
        }
};


TEST_F(DataAggregationTest, GetMax)
{
    double supposed_max = *std::max_element(set_data.begin(), set_data.end());
    EXPECT_EQ(D.getMax(), supposed_max);
    double supposed_max2 = *std::max_element(set_data2.begin(), set_data2.end());
    EXPECT_EQ(E.getMax(), supposed_max2);
}


TEST_F(DataAggregationTest, GetMin)
{
    double supposed_min = *std::min_element(set_data.begin(), set_data.end());
    EXPECT_EQ(D.getMin(), supposed_min);
    double supposed_min2 = *std::min_element(set_data2.begin(), set_data2.end());
    EXPECT_EQ(E.getMin(), supposed_min2);    
}


TEST_F(DataAggregationTest, Average)
{
    double supposed_average = std::accumulate(set_data.begin(), set_data.end(), 0.0) / set_data.size();
    EXPECT_EQ(D.getAverage(),  supposed_average);
    double supposed_average2 = std::accumulate(set_data2.begin(), set_data2.end(), 0.0) / set_data2.size();
    EXPECT_EQ(E.getAverage(),  supposed_average2);    
}


TEST_F(DataAggregationTest, Median)
{
    std::vector<double> v(set_data);
    std::vector<double> v2(set_data2);

    // std::sort(v.begin(), v.end());
    // std::sort(v2.begin(), v2.end());

    double supposed_median;
    int n = v.size();
    if (n % 2 == 0)
    {
        // Applying nth_element
        // on n/2th index
        nth_element(v.begin(), v.begin() + n / 2, v.end());
  
        // Applying nth_element
        // on (n-1)/2 th index
        nth_element(v.begin(), v.begin() + (n - 1) / 2, v.end());
        supposed_median = (double)(v[(n - 1) / 2] + v[n / 2]) / 2.0;
    } 
    else
    {
        std::nth_element(v.begin(), v.begin() + n / 2, v.end());
        supposed_median = (double)v[n / 2];
    }
    
    int n2 = v.size();
    if (n2 % 2 == 0)
    {
        // Applying nth_element
        // on n/2th index
        nth_element(v2.begin(), v2.begin() + n2 / 2, v2.end());
  
        // Applying nth_element
        // on (n-1)/2 th index
        nth_element(v2.begin(), v2.begin() + (n2 - 1) / 2, v2.end());
        supposed_median = (double)(v2[(n2 - 1) / 2] + v2[n2 / 2]) / 2.0;
    } 
    else
    {
        std::nth_element(v2.begin(), v2.begin() + n / 2, v2.end());
        supposed_median = (double)v2[n2 / 2];
    }    
}