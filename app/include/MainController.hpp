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
    void  draw() override;
    void end_draw() override;

private:
    void update_camera();
    void draw_track();
    void draw_track_lines();
    void draw_ferrari();
    void draw_lamps();

    bool  m_cursor_enabled = false;

    static constexpr int NUM_LAMPS = 4;
    const glm::vec3 m_lamp_positions[NUM_LAMPS] = {
        {-8.5f, 0.0f, -8.0f},
        { 8.5f, 0.0f, -8.0f},
        {-8.5f, 0.0f,  8.0f},
        { 8.5f, 0.0f,  8.0f}
    };

    float m_lamp_light_height = 8.0f;
};

}

#endif