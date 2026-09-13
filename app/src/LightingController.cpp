#include "LightingController.hpp"
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <string>

namespace app {

void LightingController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_L).state() == engine::platform::Key::State::JustPressed) {
        toggle_lamps();
        }
    if (platform->key(engine::platform::KEY_M).state() == engine::platform::Key::State::JustPressed) {
        toggle_moon();
        }
    if (platform->key(engine::platform::KEY_UP).state() == engine::platform::Key::State::Pressed) {
        increase_lamp_intensity();
        }
    if (platform->key(engine::platform::KEY_DOWN).state() == engine::platform::Key::State::Pressed) {
        decrease_lamp_intensity();
        }
}

void LightingController::toggle_lamps() {
    m_lamps_enabled = !m_lamps_enabled;
}

void LightingController::toggle_moon() {
    m_moon_enabled = !m_moon_enabled;
}

void LightingController::increase_lamp_intensity() {
    m_lamp_intensity += 0.02f;
}

void LightingController::decrease_lamp_intensity() {
    m_lamp_intensity -= 0.02f;

    if (m_lamp_intensity < 0.0f) {
        m_lamp_intensity = 0.0f;
    }
}

bool LightingController::lamps_enabled() const {
    return m_lamps_enabled;
}

bool LightingController::moon_enabled() const {
    return m_moon_enabled;
}

float LightingController::lamp_intensity() const {
    return m_lamp_intensity;
}

const std::array<glm::vec3, LightingController::NUM_LAMPS>& LightingController::lamp_positions() const {
    return m_lamp_positions;
}

void LightingController::apply_lighting(float directional_specular,  float point_specular) {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto shader = resources->shader("lighting");

    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_vec3("sceneAmbient", glm::vec3(0.05f));

    shader->set_vec3("dirLight.direction",glm::vec3(-0.42f, -0.70f, -0.66f));
    shader->set_vec3("dirLight.ambient", m_moon_enabled ? glm::vec3(0.10f) : glm::vec3(0.0f));
    shader->set_vec3("dirLight.diffuse", m_moon_enabled ? glm::vec3(0.35f) : glm::vec3(0.0f));
    shader->set_vec3("dirLight.specular", m_moon_enabled ? glm::vec3(directional_specular) : glm::vec3(0.0f));

    for (int i = 0; i < NUM_LAMPS; ++i) {
        const std::string prefix = "pointLights[" + std::to_string(i) + "]";
        const glm::vec3 light_position = m_lamp_positions[i] + glm::vec3(0.0f, m_lamp_light_height, 0.0f);

        shader->set_vec3(prefix + ".position", light_position);
        shader->set_vec3(prefix + ".ambient",glm::vec3(0.0f));
        shader->set_vec3(prefix + ".diffuse", m_lamps_enabled ? glm::vec3(m_lamp_intensity) : glm::vec3(0.0f));
        shader->set_vec3(prefix + ".specular", m_lamps_enabled ? glm::vec3(point_specular) : glm::vec3(0.0f));

        shader->set_float(prefix + ".constant", 1.0f);
        shader->set_float(prefix + ".linear", 0.07f);
        shader->set_float(prefix + ".quadratic", 0.017f);
    }
}

} // namespace app