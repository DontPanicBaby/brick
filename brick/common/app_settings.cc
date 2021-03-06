#include "app_settings.h"
#include "../platform_util.h"
#include "../helper.h"
#include "third-party/json/json.h"
#include <include/base/cef_logging.h>

AppSettings::AppSettings()
: app_token(""),
  profile_path(""),
  cache_path(""),
  log_file(""),
  proxy_server(""),
  resource_dir(""),
  ignore_certificate_errors(false),
  log_severity(LOGSEVERITY_DEFAULT),
#ifndef NDEBUG
  start_minimized(true),
#else
  start_minimized(false),
#endif
  auto_away(true),
  external_api(true),
  hide_on_delete(true),
  extended_status(true)
{

}

AppSettings::~AppSettings() {

}

AppSettings
AppSettings::InitByJson(std::string json) {
  AppSettings settings;
  settings.UpdateByJson(json);

  return settings;
}

void
AppSettings::UpdateByJson(std::string json) {
  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;

  bool parsingSuccessful = reader.parse(json, root);
  if (!parsingSuccessful) {
#if 0 // Turn off error message while we haven't any configuration file by default
      fprintf(stderr, "Failed to parse configuration: %s", reader.getFormattedErrorMessages().c_str());
#endif
    return;
  }

  if (root.isMember("app_token")
     && root["app_token"].isString()) {
    profile_path = root["app_token"].asString();
  }

  if (root.isMember("profile_path")
     && root["profile_path"].isString()) {
    profile_path = root["profile_path"].asString();
  }

  if (root.isMember("cache_path")
     && root["cache_path"].isString()) {
    cache_path = root["cache_path"].asString();
  }

  if (root.isMember("log_file")
     && root["log_file"].isString()) {
    log_file = root["log_file"].asString();
  }

  if (root.isMember("proxy_server")
     && root["proxy_server"].isString()) {
    proxy_server = root["proxy_server"].asString();
  }

  if (root.isMember("ignore_certificate_errors")
     && root["ignore_certificate_errors"].isBool()) {
    ignore_certificate_errors = root["ignore_certificate_errors"].asBool();
  }

  if (root.isMember("start_minimized")
     && root["start_minimized"].isBool()) {
    start_minimized = root["start_minimized"].asBool();
  }

  if (root.isMember("resource_dir")
     && root["resource_dir"].isString()) {
    resource_dir = root["resource_dir"].asString();
  }

  if (root.isMember("auto_away")
     && root["auto_away"].isBool()) {
    auto_away = root["auto_away"].asBool();
  }

  if (root.isMember("external_api")
     && root["external_api"].isBool()) {
    external_api = root["external_api"].asBool();
  }

  if (root.isMember("hide_on_delete")
     && root["hide_on_delete"].isBool()) {
    hide_on_delete = root["hide_on_delete"].asBool();
  }

  if (root.isMember("extended_status")
     && root["extended_status"].isBool()) {
    extended_status = root["extended_status"].asBool();
  }

  if (root.isMember("client_scripts")
     && root["client_scripts"].isArray()) {
    for(unsigned int i=0; i < root["client_scripts"].size(); ++i) {
      const Json::Value script = root["client_scripts"][i];

      if (!script.isString()) {
        LOG(WARNING) << "Strange client script: " << script << "; Skipping";
        continue;
      }

      std::string script_path = script.asString();
      if (script_path.find('/') != 0) {
        LOG(WARNING) << "Can't load client script, supported only absolute path: " << script_path << "; Skipping";
        continue;
      }

      std::string id;
      id.append(std::to_string(helper::HashString(script_path)));
      id.append(".js");
      client_scripts[id] = script_path;
    }
  }
}

std::string
AppSettings::DumpJson() {
  return "Implement me!";
}