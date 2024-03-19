#ifndef ESP_WIFI_CONFIG_JSON_CONVERTER_H
#define ESP_WIFI_CONFIG_JSON_CONVERTER_H

namespace ESPWifiConfig
{

#include <Arduino_JSON.h>

// Utility to parse and convert JSON data to specific types
template <typename T>
struct JsonConverter
{
};

// Utility to serialize JSON data from primitive types
// Default implementation is to just return the object
template <typename T>
struct JsonSerializer
{
  static JSONVar serialize(const T& obj) { return JSONVar(obj); }
};
// Specialization for float
template <>
struct JsonConverter<float>
{
  static float convert(const JSONVar& obj)
  {
    if (JSON.typeof(obj) == "number")
    {
      return static_cast<float>((double)obj);
    }
    throw std::runtime_error("Invalid JSON format or type for float.");
  }
};

template <>
struct JsonConverter<double>
{
  static double convert(const JSONVar& obj)
  {
    if (JSON.typeof(obj) == "number")
    {
      return static_cast<double>(obj);
    }
    throw std::runtime_error("Invalid JSON format or type for float.");
  }
};

// // Specialization for std::string (assuming that we're using std::string)
// template <>
// struct JsonConverter<std::string>
// {
//   static std::string convert(const JSONVar& obj)
//   {
//     if (obj.hasOwnProperty("value") && JSON.typeof(obj["value"]) ==
//     "string")
//     {
//       return static_cast<const char*>(obj["value"]);
//     }
//     throw std::runtime_error("Invalid JSON format or type for string.");
//   }
// };

// Specialization for int
template <>
struct JsonConverter<int>
{
  static int convert(const JSONVar& obj)
  {
    if (JSON.typeof(obj) == "number")
    {
      return static_cast<int>(obj);
    }
    throw std::runtime_error("Invalid JSON format or type for int.");
  }
};

// Specialization for bool
template <>
struct JsonConverter<bool>
{
  static bool convert(const JSONVar& obj)
  {
    if (JSON.typeof(obj) == "boolean")
    {
      return static_cast<bool>(obj);
    }
    throw std::runtime_error("Invalid JSON format or type for bool.");
  }
};

}  // namespace ESPWifiConfig

#endif  // ESP_WIFI_CONFIG_JSON_CONVERTER_H