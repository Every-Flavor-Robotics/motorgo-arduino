
#include "web_server.h"

#include "configurable_base.h"
#include "readable_base.h"
#include "variable_manager.h"

// #include "readable.h"

namespace ESPWifiConfig
{

std::vector<ConfigurableBase*> global_configurables;
// std::vector<ReadableBase*> global_readables;

WebServer::WebServer(int port) : server(port), port(port) {}

WebServer& WebServer::getInstance()
{
  static WebServer instance;
  return instance;
}

void WebServer::connect_to_wifi(String wifi_ssid, String wifi_password)
{
  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Establishing connection to WiFi...");
    // Print Credentials
    // Serial.println(WIFI_SSID);
    // Serial.println(WIFI_PASSWORD);
  }

  Serial.println("Connected to WiFi");
  // Print IP
  Serial.println(WiFi.localIP());
}

void WebServer::setup_session_endpoints()
{
  // Endpoint for POST to set session_id
  server.on(
      "/session", HTTP_POST, [](AsyncWebServerRequest* request) {}, nullptr,
      [this](AsyncWebServerRequest* request, uint8_t* data, size_t len,
             size_t index, size_t total)
      {
        AsyncResponseStream* response =
            request->beginResponseStream("application/json");
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods",
                            "GET, POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers",
                            "Origin, X-Requested-With, Content-Type, Accept");

        // Check content type
        if (request->contentType() != "application/json")
        {
          response->setCode(400);
          response->print("Content-Type must be application/json");
          request->send(response);
          return;
        }

        // Convert received data into a String
        String request_body = String((char*)data);

        // Parse the JSON body
        JSONVar json_object = JSON.parse(request_body);

        // Check if parsing succeeded
        if (JSON.typeof(json_object) == "undefined")
        {
          response->setCode(500);
          response->print("Failed to parse JSON");
          request->send(response);

          return;
        }

        // Check if the JSON contains the session_id field
        if (json_object.hasOwnProperty("session_id"))
        {
          this->session_id = (const char*)json_object["session_id"];

          response->setCode(200);
          response->print("Session ID set");
          request->send(response);
        }
        else
        {
          response->setCode(400);
          response->print("No session_id provided in JSON body");
          request->send(response);
        }
      });

  // Endpoint for GET to check session_id
  server.on(
      "/session", HTTP_GET,
      [this](AsyncWebServerRequest* request)  // Capture 'this' pointer
      {
        if (request->hasHeader("session_id"))
        {
          String received_session_id = request->header("session_id");
          if (received_session_id ==
              this->session_id)  // Use 'this' to access member variable
          {
            request->send(200, "application/json", "{\"status\": \"success\"}");
            // Blink GPIO 8
            digitalWrite(8, HIGH);
            delayMicroseconds(100);
            digitalWrite(8, LOW);
          }
          else
          {
            request->send(403, "text/plain", "Forbidden: Incorrect session ID");
          }
        }
        else
        {
          request->send(400, "text/plain", "No session_id provided");
        }
      });

  server.on("/session", HTTP_OPTIONS,
            [](AsyncWebServerRequest* request)
            {
              AsyncWebServerResponse* response =
                  request->beginResponse(200, "text/plain", "");
              response->addHeader("Access-Control-Allow-Origin", "*");
              response->addHeader("Access-Control-Allow-Methods",
                                  "GET, POST, PUT, DELETE, OPTIONS");
              response->addHeader(
                  "Access-Control-Allow-Headers",
                  "Origin, X-Requested-With, Content-Type, Accept");
              response->addHeader(
                  "Access-Control-Max-Age",
                  "600");  // How long the results of a preflight request can
                           // be cached in a preflight result cache
              request->send(response);
            });
}

void WebServer::start(String wifi_ssid, String wifi_password)
{
  // Setup GPIO pin for Green LED
  pinMode(8, OUTPUT);

  connect_to_wifi(wifi_ssid, wifi_password);

  setup_session_endpoints();

  // Setup all the GET and POST endpoints for the configurables
  std::vector<ConfigurableBase*> configurables =
      VariableManager<ConfigurableBase>::get_instance().get_variables();
  for (auto& conf : configurables)
  {
    Serial.print("Registering configurable: ");
    Serial.println(conf->get_endpoint());
    // Set up a GET endpoint for each configurable
    server.on(conf->get_endpoint().c_str(), HTTP_GET,
              [conf](AsyncWebServerRequest* request)
              { conf->handle_get(request); });

    // Set up a POST endpoint for each configurable
    server.on(
        conf->get_endpoint().c_str(), HTTP_POST,
        [](AsyncWebServerRequest* request) {}, nullptr,
        [conf](AsyncWebServerRequest* request, uint8_t* data, size_t len,
               size_t index, size_t total)
        { conf->handle_post(request, data, len); });

    server.on(conf->get_endpoint().c_str(), HTTP_OPTIONS,
              [](AsyncWebServerRequest* request)
              {
                AsyncWebServerResponse* response =
                    request->beginResponse(200, "text/plain", "");
                response->addHeader("Access-Control-Allow-Origin", "*");
                response->addHeader("Access-Control-Allow-Methods",
                                    "GET, POST, PUT, DELETE, OPTIONS");
                response->addHeader(
                    "Access-Control-Allow-Headers",
                    "Origin, X-Requested-With, Content-Type, Accept");
                response->addHeader(
                    "Access-Control-Max-Age",
                    "600");  // How long the results of a preflight request can
                             // be cached in a preflight result cache
                request->send(response);
              });
  }

  //   Setup all the GET endpoints for the readables
  std::vector<ReadableBase*> readables =
      VariableManager<ReadableBase>::get_instance().get_variables();
  for (auto& read : readables)
  {
    Serial.print("Registering readable: ");
    Serial.println(read->get_endpoint());
    // Set up a GET endpoint for each readable
    server.on(read->get_endpoint().c_str(), HTTP_GET,
              [read](AsyncWebServerRequest* request)
              {
                // Serial.println("Entered post");
                read->handle_get(request);
              });
    server.on(read->get_endpoint().c_str(), HTTP_OPTIONS,
              [](AsyncWebServerRequest* request)
              {
                AsyncWebServerResponse* response =
                    request->beginResponse(200, "text/plain", "");
                response->addHeader("Access-Control-Allow-Origin", "*");
                response->addHeader("Access-Control-Allow-Methods",
                                    "GET, POST, PUT, DELETE, OPTIONS");
                response->addHeader(
                    "Access-Control-Allow-Headers",
                    "Origin, X-Requested-With, Content-Type, Accept");
                response->addHeader(
                    "Access-Control-Max-Age",
                    "600");  // How long the results of a preflight request can
                             // be cached in a preflight result cache
                request->send(response);
              });
  }

  // Finally, start the server
  server.begin();
}

void WebServer::stop() { server.end(); }

String WebServer::get_ip_address() const { return WiFi.localIP().toString(); }

String WebServer::get_port() const { return String(port); }

}  // namespace ESPWifiConfig
