#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Engine.hpp>

namespace app {

class MainController : public engine::core::Controller {
public:
    void initialize() override;
    bool loop() override;
    void poll_events() override;
    void update() override;
    void begin_draw() override;
    void draw() override;
    void end_draw() override;

private:
    void update_camera();
    void draw_track();
    void draw_track_lines();
    void draw_ferrari();
    void draw_lamps();
    void draw_starting_lights();
    void draw_active_start_lights();
    void draw_gantry();
    void draw_skybox();
    void draw_moon();

    bool  m_cursor_enabled = false;
};

}

#endif