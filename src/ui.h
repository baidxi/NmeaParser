//
// Created by juno on 23-6-24.
//

#ifndef GPS_UTILS_SRC_UI_H
#define GPS_UTILS_SRC_UI_H

#include "lvgl/lvgl.h"
#include "GPS.h"

class ui {
  lv_disp_t *display;
  lv_obj_t *tv;
  const lv_font_t  *font;
  lv_style_t	text_muted;
  lv_style_t 	title;
  lv_style_t 	icon;
  lv_style_t 	bullet;

public:
  explicit ui(lv_coord_t w, lv_coord_t h);
  void loop_run(GPS *);
  void style_init();
};

#endif //GPS_UTILS_SRC_UI_H
