// Header file for the MotorGo Mini driver class.

#ifndef PID_MANAGER_H
#define PID_MANAGER_H

#include "Arduino_JSON.h"
#include "configurable.h"
#include "motorgo_mini.h"
#include "readable.h"

template <>
struct ESPWifiConfig::JsonConverter<MotorGo::PIDParameters>
{
  static MotorGo::PIDParameters convert(JSONVar obj)
  {
    MotorGo::PIDParameters params;

    JSONVar p_var = JSONVar("p");
    if (JSON.typeof(obj[p_var]) == "number")
    {
      params.p = static_cast<float>((double)obj[p_var]);
    }
    JSONVar i_var = JSONVar("i");
    if (JSON.typeof(obj[i_var]) == "number")
    {
      params.i = static_cast<float>((double)obj[i_var]);
    }
    JSONVar d_var = JSONVar("d");
    if (JSON.typeof(obj[d_var]) == "number")
    {
      params.d = static_cast<float>((double)obj[d_var]);
    }
    JSONVar output_ramp_var = JSONVar("output_ramp");
    if (JSON.typeof(obj[output_ramp_var]) == "number")
    {
      params.output_ramp = static_cast<float>((double)obj[output_ramp_var]);
    }
    JSONVar lpf_time_constant_var = JSONVar("lpf_time_constant");
    if (JSON.typeof(obj[lpf_time_constant_var]) == "number")
    {
      params.lpf_time_constant =
          static_cast<float>((double)obj[lpf_time_constant_var]);
    }
    JSONVar limit_var = JSONVar("limit");
    if (JSON.typeof(obj[limit_var]) == "number")
    {
      params.limit = static_cast<float>((double)obj[limit_var]);
    }

    return params;
  }
};

template <>
struct ESPWifiConfig::JsonSerializer<MotorGo::PIDParameters>
{
  static JSONVar serialize(const MotorGo::PIDParameters& obj)
  {
    JSONVar params;
    params["p"] = obj.p;
    params["i"] = obj.i;
    params["d"] = obj.d;
    params["output_ramp"] = obj.output_ramp;
    params["lpf_time_constant"] = obj.lpf_time_constant;
    params["limit"] = obj.limit;

    return params;
  }
};

namespace MotorGo
{

struct PIDManagerConfig
{
  String name;
  PIDParameters& params;
  std::function<void()> update_controller_callback;
  ESPWifiConfig::Configurable<PIDParameters> configurable;

  PIDManagerConfig(const String& name, PIDParameters& params_old,
                   std::function<void()> callback);

  PIDManagerConfig(const PIDManagerConfig& other);
};

class PIDManager
{
 public:
  PIDManager();

  // Adds a controller to the PIDManager
  // add_controller takes a PIDParameters struct and a callback function to
  // update the controller
  void add_controller(String name, PIDParameters& params,
                      std::function<void()> update_controller_callback);
  void init(String wifi_ssid, String wifi_password);

 private:
  //  Vector of params
  std::vector<PIDManagerConfig> configs;
  JSONVar schema;

  ESPWifiConfig::Readable<JSONVar> schema_readable;
  void generate_schema();
};
}  // namespace MotorGo

#endif  // MOTORGO_MINI_H
