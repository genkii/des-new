#include <cstdint>
#include <string>
#pragma once

struct Hello {
    static constexpr uint8_t ID = 0;
    std::string player_name;
    std::string password;
};
