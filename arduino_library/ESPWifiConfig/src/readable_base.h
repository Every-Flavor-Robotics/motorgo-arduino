#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <conversion_utils.h>

#include <functional>  // for std::function
#include <string>
#include <vector>

namespace ESPWifiConfig
{

class ReadableBase
{
 public:
  virtual ~ReadableBase() {}
  virtual void handle_get(AsyncWebServerRequest* request) = 0;
  virtual String get_endpoint() const = 0;
};

}  // namespace ESPWifiConfig
