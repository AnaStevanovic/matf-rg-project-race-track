#include "RaceController.hpp"
#include <engine/core/Engine.hpp>

namespace app {

void RaceController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_SPACE).state() == engine::platform::Key::State::JustPressed && !m_sequence_running
        && !m_car_moving && !m_race_finished) {
        m_sequence_running = true;
        m_active_start_lights = 0;
        m_elapsed_time = 0.0f;
    }
}

void RaceController::update() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    const float dt = platform->dt();
    if (m_sequence_running) {
        m_elapsed_time += dt;
        
        if (m_elapsed_time < 1.0f) {
            m_active_start_lights = 0;
        }
        else if (m_elapsed_time < 2.0f) {
            m_active_start_lights = 1;
        }
        else if (m_elapsed_time < 3.0f) {
            m_active_start_lights = 2;
        }
        else if (m_elapsed_time < 4.0f) {
            m_active_start_lights = 3;
        }
        else if (m_elapsed_time < 5.0f) {
            m_active_start_lights = 4;
        }
        else if (m_elapsed_time < 7.0f) {
            m_active_start_lights = 5;
        }
        else {
            m_active_start_lights = 0;
            m_sequence_running = false;
            m_car_moving = true;
        }
    }

    if (m_car_moving) {
        m_car_z_offset += m_car_speed * dt;

        if (m_car_z_offset >= m_car_stop_z) {
            m_car_z_offset = m_car_stop_z;
            m_car_moving = false;
            m_race_finished = true;
        }
    }
}

int RaceController::active_start_lights() const {
    return m_active_start_lights;
}

float RaceController::car_z_offset() const {
    return m_car_z_offset;
}

} // namespace app