#ifndef RACECONTROLLER_HPP
#define RACECONTROLLER_HPP

#include <engine/core/Controller.hpp>

namespace app {

class RaceController : public engine::core::Controller {
public:
    void poll_events() override;
    void update() override;

    int active_start_lights() const;
    float car_z_offset() const;

private:
    bool m_sequence_running = false;
    bool m_car_moving = false;

    int m_active_start_lights = 0;

    float m_car_z_offset = 0.0f;
    float m_elapsed_time = 0.0f;
    float m_car_speed = 4.0f;
    float m_car_stop_z = 16.0f;
    bool m_race_finished = false;
};

} // namespace app
#endif