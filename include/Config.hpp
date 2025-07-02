#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

class Config {
private:
  std::string title;
  int window_width;
  int window_height;

public:
  Config(std::string filename);
  std::string get_title() const;
  int get_window_width() const;
  int get_window_height() const;
};

#endif
