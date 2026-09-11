//
// Created by ana on 9/7/26.
//
#include "MainController.hpp"
#include "LightingController.hpp"
#include "RaceController.hpp"
#include "engine/graphics/GraphicsController.hpp"
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
    draw_gantry();
    draw_starting_lights();
    draw_active_start_lights();
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
    auto lighting = engine::core::Controller::get<LightingController>();
    auto race = engine::core::Controller::get<RaceController>();

    auto shader = resources->shader("lighting");
    auto ferrari = resources->model("ferrari");

    shader->use();
    shader->set_bool("useTexture", true);

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    
    auto model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f,0.1f,race->car_z_offset()));
    model = glm::scale(model,glm::vec3(1.5f));
    shader->set_mat4("model", model);

    lighting->apply_lighting(0.4f, 1.2f);

    ferrari->draw(shader);
}

void MainController::draw_track() {
    auto graphics =engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources =engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lighting = engine::core::Controller::get<LightingController>();

    auto shader = resources->shader("lighting");
    auto track = resources->model("track");

    shader->use();

    shader->set_bool("useTexture", false);
    shader->set_vec3("objectColor",glm::vec3(0.22f, 0.22f, 0.22f));

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());
    glm::mat4 model(1.0f);
    model = glm::scale(model,glm::vec3(1.1f, 1.0f, 1.0f));
    shader->set_mat4("model", model);

    lighting->apply_lighting(0.1f, 0.2f);

    track->draw(shader);
}

void MainController::draw_track_lines() {
    auto graphics =engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources =engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lighting = engine::core::Controller::get<LightingController>();

    auto shader = resources->shader("lighting");
    auto strip = resources->model("strip");

    shader->use();

    shader->set_bool("useTexture", false);
    shader->set_vec3("objectColor", glm::vec3(0.8f));

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());

    lighting->apply_lighting(0.0f, 0.0f);

    // Leva ivica staze
    auto left = glm::mat4(1.0f);
    left = glm::translate(left,glm::vec3(-7.8f, 0.01f, 0.0f));
    left = glm::scale(left,glm::vec3(0.15f, 1.0f, 40.0f));

    shader->set_mat4("model", left);
    strip->draw(shader);

    // Desna ivica staze
    auto right = glm::mat4(1.0f);
    right = glm::translate(right,glm::vec3(7.8f, 0.01f, 0.0f));
    right = glm::scale(right,glm::vec3(0.15f, 1.0f, 40.0f));

    shader->set_mat4("model", right);
    strip->draw(shader);

    // Startna linija
    auto start = glm::mat4(1.0f);
    start = glm::translate(start,glm::vec3(0.0f, 0.01f, 4.0f));
    start = glm::scale(start,glm::vec3(15.6f, 1.0f, 0.25f));

    shader->set_mat4("model", start);
    strip->draw(shader);
}

void MainController::draw_lamps() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lighting = engine::core::Controller::get<LightingController>();

    auto shader = resources->shader("lighting");
    auto lamp = resources->model("lamp");

    shader->use();
    shader->set_bool("useTexture", true);

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());

    lighting->apply_lighting(0.2f, 0.3f);

    for (const auto& position : lighting->lamp_positions()) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(1.0f));
        shader->set_mat4("model", model);

        lamp->draw(shader);
    }
}

void MainController::draw_starting_lights() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lighting = engine::core::Controller::get<LightingController>();

    auto shader = resources->shader("lighting");
    auto starting_lights = resources->model("starting_lights");

    shader->use();
    shader->set_bool("useTexture", false);
    shader->set_vec3("objectColor", glm::vec3(0.2f));

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());

    lighting->apply_lighting(0.2f, 0.3f);

    glm::mat4 model(1.0f);
    model = glm::translate(model,glm::vec3(1.05f, 9.62f, 11.5f));
    model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model,glm::vec3(0.75f));
    model = glm::translate(model,glm::vec3(-35.53f, -3.10f, -85.29f));
    shader->set_mat4("model", model);

    starting_lights->draw(shader);
}

void MainController::draw_gantry() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto lighting = engine::core::Controller::get<LightingController>();

    auto shader = resources->shader("lighting");
    auto gantry = resources->model("gantry");

    shader->use();

    shader->set_bool("useTexture", false);
    shader->set_vec3("objectColor", glm::vec3(0.2f));

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());

    lighting->apply_lighting(0.2f, 0.3f);

    // Gornja greda
    glm::mat4 top(1.0f);
    top = glm::translate(top,glm::vec3(0.0f, 10.3f, 12.2f));
    top = glm::scale(top,glm::vec3(0.147f, 0.04f, 0.04f));
    shader->set_mat4("model", top);
    gantry->draw(shader);

    // Levi stub
    glm::mat4 left(1.0f);
    left = glm::translate(left,glm::vec3(-8.9f, 5.375f, 12.2f));
    left = glm::rotate(left,glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    left = glm::scale(left,glm::vec3(0.086f, 0.04f, 0.04f));
    shader->set_mat4("model", left);
    gantry->draw(shader);

    // Desni stub
    glm::mat4 right(1.0f);
    right = glm::translate(right,glm::vec3(8.9f, 5.375f, 12.2f));
    right = glm::rotate(right,glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    right = glm::scale(right,glm::vec3(0.086f, 0.04f, 0.04f));
    shader->set_mat4("model", right);
    gantry->draw(shader);
}

void MainController::draw_active_start_lights() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto race = engine::core::Controller::get<RaceController>();
    auto lighting = engine::core::Controller::get<LightingController>();

    const int active_lights = race->active_start_lights();
    if (active_lights == 0) return;

    auto shader = resources->shader("lighting");
    auto light = resources->model("start_light");

    shader->use();

    shader->set_bool("useTexture", false);
    shader->set_vec3("objectColor",glm::vec3(1.0f, 0.0f, 0.0f));

    shader->set_mat4("projection",graphics->projection_matrix());
    shader->set_mat4("view",graphics->camera()->view_matrix());

    lighting->apply_lighting(0.0f, 0.0f);

    const float first_light_x = -5.55f;
    const float light_y = 7.94f;
    const float light_z = 10.1f;

    const float light_spacing = 2.23f;
    const float light_scale = 0.44f;

    for (int i = 0; i < active_lights; ++i) {
        const float x = first_light_x + i * light_spacing;

        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(x, light_y, light_z));
        model = glm::scale(model,glm::vec3(light_scale));
        shader->set_mat4("model", model);

        light->draw(shader);
    }
}

}// namespace app