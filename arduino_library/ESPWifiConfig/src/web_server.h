#ifndef ESP_WIFI_CONFIG_WEBSERVER_H
#define ESP_WIFI_CONFIG_WEBSERVER_H

#include <Arduino_JSON.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#include <vector>

namespace ESPWifiConfig
{

// Forward declaration to avoid circular dependency
class ConfigurableBase;

class WebServer
{
 private:
  AsyncWebServer server;
  String session_id;
  int port;

  // Private constructor
  WebServer(int port = 8080);

  void connect_to_wifi(String wifi_ssid, String wifi_password);
  void setup_session_endpoints();

 public:
  // Deleted copy constructors to ensure single instance
  WebServer(WebServer& other) = delete;
  void operator=(const WebServer&) = delete;

  static WebServer& getInstance();

  //   void register_configurable(ConfigurableBase* configurable);

  void start(String wifi_ssid, String wifi_password);
  void stop();

  //   Get IP and port
  String get_ip_address() const;
  String get_port() const;
};

}  // namespace ESPWifiConfig

#endif  // ESP_WIFI_CONFIG_WEBSERVER_H
