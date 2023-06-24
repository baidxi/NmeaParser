//
// Created by juno on 23-6-24.
//

#include <chrono>
#include <thread>
#include <cstring>

#include "ui.h"
#include "lvgl/lvgl.h"

ui::ui(lv_coord_t w, lv_coord_t h) {
  lv_init();
  lv_group_set_default(lv_group_create());
  display = lv_sdl_window_create(w, h);
  lv_indev_t  *mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_create());
  lv_indev_set_disp(mouse, display);

  LV_IMG_DECLARE(mouse_cursor_icon);
  lv_obj_t  *cursor_obj = lv_img_create(lv_scr_act());
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);
  lv_indev_set_cursor(mouse, cursor_obj);

  lv_indev_t  *mousewheel = lv_sdl_mouse_create();
  lv_indev_set_disp(mousewheel, display);
  lv_indev_set_group(mousewheel, lv_group_create());

  lv_indev_t  *keyboard = lv_sdl_keyboard_create();
  lv_indev_set_disp(keyboard, display);
  lv_indev_set_group(keyboard, lv_group_create());
}

void ui::loop_run(GPS *gps) {
  auto delay = std::chrono::duration<int, std::milli>(60);
  zda *tm = (zda *)gps->get("ZDA");
  gsv *info_gsv = (gsv *)gps->get("GSV");
  rmc *rmc_info = (rmc *)gps->get("RMC");

  gsv_group *group;
  gsv_info *info;

  tv = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 140);

  lv_obj_t *tab = lv_tabview_get_tab_btns(tv);
  lv_obj_t *time_lab = lv_label_create(tab);
  lv_obj_set_width(time_lab, LV_SIZE_CONTENT);   /// 1
  lv_obj_set_height(time_lab, LV_SIZE_CONTENT);    /// 1
  lv_obj_set_x(time_lab, 10);
  lv_obj_set_y(time_lab, 10);
  lv_obj_set_style_text_font(time_lab, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *tab1 = lv_tabview_get_tab_btns(tv);
  lv_obj_t *rmc_lab = lv_label_create(tab1);
  lv_obj_set_width(rmc_lab, LV_SIZE_CONTENT);   /// 1
  lv_obj_set_height(rmc_lab, LV_SIZE_CONTENT);    /// 1
  lv_obj_set_x(rmc_lab, 10);
  lv_obj_set_y(rmc_lab, 80);
  lv_obj_set_style_text_font(rmc_lab, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *ui_gsv_info = lv_chart_create(lv_scr_act());
  lv_obj_set_width(ui_gsv_info, 700);
  lv_obj_set_height(ui_gsv_info, 200);
  lv_obj_set_x(ui_gsv_info, 50);
  lv_obj_set_y(ui_gsv_info, 200);
//  lv_obj_set_align(ui_gsv_info, LV_ALIGN_CENTER);
  lv_chart_set_type(ui_gsv_info, LV_CHART_TYPE_BAR);
  lv_chart_set_point_count(ui_gsv_info, 16);
  lv_chart_set_axis_tick(ui_gsv_info, LV_CHART_AXIS_PRIMARY_X, 10, 5, 16, 2, true, 50);
  lv_chart_set_axis_tick(ui_gsv_info, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
  lv_chart_set_axis_tick(ui_gsv_info, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, true, 25);

  lv_chart_series_t * ui_gsv_info_series1 = lv_chart_add_series(ui_gsv_info, lv_color_hex(0x008080), LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t ui_gsv_info_series1_elevation[4][4] = {0};
  lv_chart_series_t * ui_gsv_info_series2 = lv_chart_add_series(ui_gsv_info, lv_color_hex(0xaa8080), LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t ui_gsv_info_series2_elevation[4][4] = {0};
  lv_chart_series_t * ui_gsv_info_series3 = lv_chart_add_series(ui_gsv_info, lv_color_hex(0x8080ff), LV_CHART_AXIS_PRIMARY_Y);
  static lv_coord_t ui_gsv_info_series3_elevation[4][4] = {0};

  while(gps->is_active()) {
	lv_timer_handler();
	uint32_t time = strtoul(tm->time, nullptr, 10);
	int h, m, s;
	sscanf(tm->time, "%02d%02d%02d", &h, &m, &s);

	for (int i = 0; i < info_gsv->group_count; ++i) {
	  for (int j = 0; j < info_gsv->group[i].num_info; ++j) {
		ui_gsv_info_series1_elevation[i][j] = info_gsv->group[i].infos[j].elevation;
		ui_gsv_info_series2_elevation[i][j] = info_gsv->group[i].infos[j].azimuth;
		ui_gsv_info_series3_elevation[i][j] = info_gsv->group[i].infos[j].snr;
	  }
	}

	lv_label_set_text_fmt(time_lab, "%d-%d-%d:%02d:%02d:%02d", tm->year, tm->mon, tm->day, h + 8, m, s);
	lv_label_set_text_fmt(rmc_lab, "status:%c Latitude:%s %c Longitude:%s %c",
						  rmc_info->status, std::to_string(rmc_info->Latitude).c_str(),
						  rmc_info->ns,std::to_string(rmc_info->Longitude).c_str(), rmc_info->ew);

	lv_chart_set_ext_y_array(ui_gsv_info, ui_gsv_info_series1, (lv_coord_t *)ui_gsv_info_series1_elevation);
	lv_chart_set_ext_y_array(ui_gsv_info, ui_gsv_info_series2, (lv_coord_t *)ui_gsv_info_series3_elevation);
	lv_chart_set_ext_y_array(ui_gsv_info, ui_gsv_info_series3, (lv_coord_t *)ui_gsv_info_series3_elevation);
	std::this_thread::sleep_for(delay);
  }
}

void ui::style_init() {
  font = &lv_font_montserrat_24;

  #if LV_USE_THEME_DEFAULT
  lv_theme_default_init(nullptr,
						lv_palette_main(LV_PALETTE_BLUE),
						lv_palette_main(LV_PALETTE_RED),
						LV_THEME_DEFAULT_DARK, font);
  #endif

  lv_style_init(&text_muted);
  lv_style_set_text_opa(&text_muted, LV_OPA_50);

  lv_style_init(&title);
  lv_style_set_text_font(&title, font);

  lv_style_init(&icon);
  lv_style_set_text_color(&icon, lv_theme_get_color_primary(nullptr));
  lv_style_set_text_font(&icon, font);

  lv_style_init(&bullet);
  lv_style_set_border_width(&bullet, 0);
  lv_style_set_radius(&bullet, LV_RADIUS_CIRCLE);
}
