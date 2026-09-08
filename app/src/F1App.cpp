//
// Created by ana on 9/7/26.
//
#include "F1App.hpp"
#include <MainController.hpp>

namespace app {

void F1App::app_setup() {
    auto main_controller = register_controller<MainController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}

}