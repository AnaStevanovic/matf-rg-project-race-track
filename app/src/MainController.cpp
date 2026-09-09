//
// Created by ana on 9/7/26.
//
#include "MainController.hpp"

#include "engine/graphics/GraphicsController.hpp"
#include <string>
#include <engine/core/Engine.hpp>

namespace app {

void MainController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();

    auto camera =  engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->Position = glm::vec3(0.0f, 3.0f, 10.0f);
    camera->rotate_camera(0.0f, -180.0f);
}

bool MainController::loop() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
        return false;
    }

    return true;
}

void MainController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_F1).state() == engine::platform::Key::State::JustPressed) {
        m_cursor_enabled = !m_cursor_enabled;
        platform->set_enable_cursor(m_cursor_enabled);
    }
}

void MainController::update() {
    update_camera();
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    draw_track();
    draw_track_lines();
    draw_lamps();
    draw_ferrari();
}

void MainController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void MainController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float dt = platform->dt();

    if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }

    auto mouse = platform->mouse();

    camera->rotate_camera(mouse.dx, mouse.dy);
    camera->zoom(mouse.scroll);
}

void MainController::draw_ferrari() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    auto shader = resources->shader("lighting");
    auto ferrari = resources->model("ferrari");

    shader->use();
    shader->set_bool("useTexture", true);
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    auto model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f, 0.1f, 0.0f));
    model = glm::scale(model,glm::vec3(1.5f));
    shader->set_mat4("model", model);

    shader->set_vec3("viewPos", graphics->camera()->Position);

    shader->set_vec3("dirLight.direction",glm::vec3(-0.2f, -1.0f, -0.3f));
    shader->set_vec3("dirLight.ambient",glm::vec3(0.10f));
    shader->set_vec3("dirLight.diffuse",glm::vec3(0.35f));
    shader->set_vec3("dirLight.specular",glm::vec3(0.4f));



    for (int i = 0; i < NUM_LAMPS; ++i) {
        const std::string prefix = "pointLights[" + std::to_string(i) + "]";
        const glm::vec3 light_position = m_lamp_positions[i] + glm::vec3(0.0f, m_lamp_light_height, 0.0f);

        shader->set_vec3(prefix + ".position",light_position);
        shader->set_vec3(prefix + ".ambient",glm::vec3(0.0f));
        shader->set_vec3(prefix + ".diffuse",glm::vec3(1.2f));
        shader->set_vec3(prefix + ".specular",glm::vec3(1.2f));

        shader->set_float(prefix + ".constant",1.0f);
        shader->set_float(prefix + ".linear",0.09f);
        shader->set_float(prefix + ".quadratic",0.032f);
    }

    ferrari->draw(shader);
}

void MainController::draw_track() {
    auto graphics =engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources =engine::core::Controller::get<engine::resources::ResourcesController>();

    auto shader = resources->shader("lighting");
    auto track = resources->model("track");

    shader->use();

    shader->set_bool("useTexture", false);
    shader->set_vec3("objectColor",glm::vec3(0.15f, 0.15f, 0.15f));

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());
    shader->set_mat4("model",glm::mat4(1.0f));

    shader->set_vec3("viewPos",graphics->camera()->Position);

    shader->set_vec3("dirLight.direction",glm::vec3(-0.2f, -1.0f, -0.3f));
    shader->set_vec3("dirLight.ambient",glm::vec3(0.10f));
    shader->set_vec3("dirLight.diffuse",glm::vec3(0.35f));
    shader->set_vec3("dirLight.specular",glm::vec3(0.1f));

    for (int i = 0; i < NUM_LAMPS; ++i) {
        const std::string prefix = "pointLights[" + std::to_string(i) + "]";
        const glm::vec3 light_position = m_lamp_positions[i] + glm::vec3(0.0f, m_lamp_light_height, 0.0f);

        shader->set_vec3(prefix + ".position",light_position);
        shader->set_vec3(prefix + ".ambient",glm::vec3(0.0f));
        shader->set_vec3(prefix + ".diffuse",glm::vec3(1.2f));
        shader->set_vec3(prefix + ".specular",glm::vec3(0.2f));

        shader->set_float(prefix + ".constant",1.0f);
        shader->set_float(prefix + ".linear",0.09f);
        shader->set_float(prefix + ".quadratic",0.032f);
    }

    track->draw(shader);
}

void MainController::draw_track_lines() {
    auto graphics =engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources =engine::core::Controller::get<engine::resources::ResourcesController>();

    auto shader = resources->shader("uniform_color");
    auto strip = resources->model("strip");

    shader->use();

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());

    shader->set_vec4("color",glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Leva ivica staze
    auto left = glm::mat4(1.0f);
    left = glm::translate(left,glm::vec3(-7.2f, 0.01f, 0.0f));
    left = glm::scale(left,glm::vec3(0.15f, 1.0f, 40.0f));

    shader->set_mat4("model", left);
    strip->draw(shader);

    // Desna ivica staze
    auto right = glm::mat4(1.0f);
    right = glm::translate(right,glm::vec3(7.2f, 0.01f, 0.0f));
    right = glm::scale(right,glm::vec3(0.15f, 1.0f, 40.0f));

    shader->set_mat4("model", right);
    strip->draw(shader);

    // Startna linija
    auto start = glm::mat4(1.0f);
    start = glm::translate(start,glm::vec3(0.0f, 0.01f, 4.0f));
    start = glm::scale(start,glm::vec3(14.4f, 1.0f, 0.25f));

    shader->set_mat4("model", start);
    strip->draw(shader);
}

void MainController::draw_lamps() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    auto shader = resources->shader("basic");
    auto lamp = resources->model("lamp");

    shader->use();

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    for (const auto& position : m_lamp_positions) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(1.0f));
        shader->set_mat4("model", model);

        lamp->draw(shader);
    }
}

}// namespace app