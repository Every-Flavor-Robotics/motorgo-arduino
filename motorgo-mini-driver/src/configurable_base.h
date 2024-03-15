#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <conversion_utils.h>

#include <functional>  // for std::function
#include <string>
#include <vector>

namespace ESPWifiConfig
{

class ConfigurableBase
{
 public:
  virtual void handle_get(AsyncWebServerRequest* request) = 0;
  virtual void handle_post(AsyncWebServerRequest* request, uint8_t* data,
                           size_t len) = 0;
  virtual String get_endpoint() const = 0;
};

}  // namespace ESPWifiConfig
