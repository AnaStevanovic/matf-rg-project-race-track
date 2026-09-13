#ifndef F1_APP_HPP
#define F1_APP_HPP

#include <engine/core/Engine.hpp>

namespace app {

class F1App : public engine::core::App {
protected:
    void app_setup() override;
};

}

#endif