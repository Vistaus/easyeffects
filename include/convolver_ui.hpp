/*
 *  Copyright © 2017-2020 Wellington Wallace
 *
 *  This file is part of PulseEffects.
 *
 *  PulseEffects is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PulseEffects is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PulseEffects.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CONVOLVER_UI_HPP
#define CONVOLVER_UI_HPP

#include <glibmm/i18n.h>
#include <gst/fft/gstfftf32.h>
#include <gtkmm/filechoosernative.h>
#include <boost/filesystem.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <future>
#include <mutex>
#include <sndfile.hh>
#include "glibmm/main.h"
#include "glibmm/miscutils.h"
#include "gtkmm/dialog.h"
#include "plugin_ui_base.hpp"
#include "sigc++/functors/ptr_fun.h"

class ConvolverUi : public Gtk::Grid, public PluginUiBase {
 public:
  ConvolverUi(BaseObjectType* cobject,
              const Glib::RefPtr<Gtk::Builder>& builder,
              const std::string& schema,
              const std::string& schema_path);
  ConvolverUi(const ConvolverUi&) = delete;
  auto operator=(const ConvolverUi&) -> ConvolverUi& = delete;
  ConvolverUi(const ConvolverUi&&) = delete;
  auto operator=(const ConvolverUi&&) -> ConvolverUi& = delete;
  ~ConvolverUi() override;

  void reset() override;

 private:
  std::string log_tag = "convolver_ui: ";

  Glib::RefPtr<Gtk::Adjustment> input_gain, output_gain, ir_width;

  Gtk::ListBox* irs_listbox = nullptr;

  Gtk::MenuButton* irs_menu_button = nullptr;

  Gtk::ScrolledWindow* irs_scrolled_window = nullptr;

  Gtk::Button* import_irs = nullptr;

  Gtk::DrawingArea *left_plot = nullptr, *right_plot = nullptr;

  Gtk::Label *label_file_name = nullptr, *label_sampling_rate = nullptr, *label_samples = nullptr,
             *label_duration = nullptr;

  Gtk::ToggleButton* show_fft = nullptr;

  Pango::FontDescription font;

  boost::filesystem::path irs_dir;

  bool mouse_inside = false, show_fft_spectrum = false;
  unsigned int max_plot_points = 1000U;
  float mouse_intensity = 0.0F, mouse_time = 0.0F, mouse_freq = 0.0F;
  float min_left = 0.0F, max_left = 0.0F, min_right = 0.0F, max_right = 0.0F;
  float max_time = 0.0F;
  float fft_min_left = 0.0F, fft_max_left = 0.0F, fft_min_right = 0.0F, fft_max_right = 0.0F;
  float fft_max_freq = 0.0F, fft_min_freq = 0.0F;
  std::vector<float> left_mag, right_mag, time_axis;
  std::vector<float> left_spectrum, right_spectrum, freq_axis;

  Glib::RefPtr<Gio::Settings> spectrum_settings;

  std::mutex lock_guard_irs_info;

  std::vector<std::future<void>> futures;

  auto get_irs_names() -> std::vector<std::string>;

  void import_irs_file(const std::string& file_path);

  void remove_irs_file(const std::string& name);

  void populate_irs_listbox();

  static auto on_listbox_sort(Gtk::ListBoxRow* row1, Gtk::ListBoxRow* row2) -> int;

  void on_irs_menu_button_clicked();

  void on_import_irs_clicked();

  void get_irs_info();

  void get_irs_spectrum(const int& rate);

  void draw_channel(Gtk::DrawingArea* da,
                    const Cairo::RefPtr<Cairo::Context>& ctx,
                    const std::vector<float>& magnitudes);

  void update_mouse_info_L(GdkEventMotion* event);

  void update_mouse_info_R(GdkEventMotion* event);

  auto on_left_draw(const Cairo::RefPtr<Cairo::Context>& ctx) -> bool;

  auto on_left_motion_notify_event(GdkEventMotion* event) -> bool;

  auto on_right_draw(const Cairo::RefPtr<Cairo::Context>& ctx) -> bool;

  auto on_right_motion_notify_event(GdkEventMotion* event) -> bool;

  auto on_mouse_enter_notify_event(GdkEventCrossing* event) -> bool;

  auto on_mouse_leave_notify_event(GdkEventCrossing* event) -> bool;
};

#endif
