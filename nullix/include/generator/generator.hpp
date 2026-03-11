#pragma once
#include <expected>
#include <string>
#include <toml++/toml.hpp>
#include <vector>

namespace nullix {

    class Generator{
        public:
            auto getModules() -> std::expected<std::vector<std::string>, std::string>;
            auto computeNextGeneration() -> std::expected<int, std::string>;
            auto generate() -> std::expected<bool, std::string>;
    };
}
