#ifndef ESP_WIFI_CONFIG_READABLE_H
#define ESP_WIFI_CONFIG_READABLE_H

#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <conversion_utils.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "readable_base.h"
#include "variable_manager.h"

namespace ESPWifiConfig
{

template <typename T>
class Readable : public ReadableBase
{
 private:
  // Callback for GET which retrieves the value
  std::function<T()> get_value_callback;

  String endpoint;
  String description;

 public:
  Readable() {}
  Readable(std::function<T()> value_func, const String endpoint_path,
           const String desc)
      : get_value_callback(value_func),
        endpoint(endpoint_path),
        description(desc)

  {
    VariableManager<ReadableBase>::get_instance().register_variable(this);
  }

  Readable(const Readable& other)
      : get_value_callback(other.get_value_callback),
        endpoint(other.endpoint),
        description(other.description)
  {
    VariableManager<ReadableBase>::get_instance().register_variable(this);

    Serial.println("Readable copy constructor");
  }

  ~Readable()
  {
    Serial.println("Readable destructor");
    VariableManager<ReadableBase>::get_instance().deregister_variable(this);
  }

  Readable& operator=(const Readable& other)
  {
    if (this != &other)
    {
      // Release own resources (if any)

      // Copy resources from 'other'
      get_value_callback = other.get_value_callback;
      endpoint = other.endpoint;
      description = other.description;

      VariableManager<ReadableBase>::get_instance().register_variable(this);
    }
    return *this;
  }

  String get_endpoint() const override { return endpoint; }

  void handle_get(AsyncWebServerRequest* request) override
  {
    if (get_value_callback)
    {
      T value = get_value_callback();
      AsyncResponseStream* response =
          request->beginResponseStream("application/json");

      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      response->addHeader("Access-Control-Allow-Headers",
                          "Origin, X-Requested-With, Content-Type, Accept");

      response->setCode(200);
      //   Add the value to the JSON object
      JSONVar json_object;
      json_object["value"] = value;
      response->print(JSON.stringify(json_object));
      request->send(response);
    }
    else
    {
      AsyncResponseStream* response =
          request->beginResponseStream("text/plain");

      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      response->addHeader("Access-Control-Allow-Headers",
                          "Origin, X-Requested-With, Content-Type, Accept");

      response->setCode(400);
      request->send(response);
    }
  }
};

}  // namespace ESPWifiConfig

#endif  // ESP_WIFI_CONFIG_CONFIGURABLE_H
