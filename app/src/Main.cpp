#include <engine/core/Engine.hpp>
#include "F1App.hpp"
#include <memory>

int main(int argc, char **argv) {
    return std::make_unique<app::F1App>()->run(argc, argv);
}
