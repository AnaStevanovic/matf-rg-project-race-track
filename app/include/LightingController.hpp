#ifndef LIGHTING_CONTROLLER_HPP
#define LIGHTING_CONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <array>
#include <glm/glm.hpp>

namespace app {

class LightingController : public engine::core::Controller {
public:
    static const int NUM_LAMPS = 4;
    void poll_events() override;

    void toggle_lamps();
    void toggle_moon();

    void increase_lamp_intensity();
    void decrease_lamp_intensity();

    bool lamps_enabled() const;
    bool moon_enabled() const;
    float lamp_intensity() const;

    const std::array<glm::vec3, NUM_LAMPS>& lamp_positions() const;
    void apply_lighting(float directional_specular, float point_specular);

private:
    bool m_lamps_enabled = true;
    bool m_moon_enabled = true;

    float m_lamp_intensity = 2.0f;
    float m_lamp_light_height = 8.0f;

    std::array<glm::vec3, NUM_LAMPS> m_lamp_positions = {
        glm::vec3(-8.5f, 0.0f, -8.0f),
        glm::vec3(8.5f, 0.0f, -8.0f),
        glm::vec3(-8.5f, 0.0f, 8.0f),
        glm::vec3(8.5f, 0.0f, 8.0f)
    };
};

} // namespace app

#endif