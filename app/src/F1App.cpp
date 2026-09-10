//
// Created by ana on 9/7/26.
//
#include "F1App.hpp"
#include <MainController.hpp>
#include <LightingController.hpp>

namespace app {

void F1App::app_setup() {
    auto lighting_controller = register_controller<LightingController>();
    lighting_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());

    auto main_controller = register_controller<MainController>();
    main_controller->after(lighting_controller);
}

}