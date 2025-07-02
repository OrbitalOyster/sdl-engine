#include "Config.hpp"
#include <stdexcept>
#include <yaml-cpp/yaml.h>

Config::Config(std::string filename) {
  try {
    YAML::Node config = YAML::LoadFile(filename);
    YAML::Node window = config["window"];
    title = window["title"].as<std::string>();
    window_width = window["width"].as<int>();
    window_height = window["height"].as<int>();
  } catch (const std::runtime_error err) {
    throw std::runtime_error(
        std::string("Unable to load config " + filename + ": ") + err.what());
  }
}

std::string Config::get_title() const { return title; };
int Config::get_window_width() const { return window_width; }
int Config::get_window_height() const { return window_height; }
