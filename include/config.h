#ifndef CONFIG_H
#define CONFIG_H

struct Config {
  int window_width;
  int window_height;
  char *window_title;
};

struct Config *load_config(char *filename);
void destroy_config(struct Config *config);

#endif /* CONFIG_H */
