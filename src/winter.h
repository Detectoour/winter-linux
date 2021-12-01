#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <dlfcn.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

/*#include "imgui/GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"*/

#include "utils/address.h"
#include "utils/bitflag.h"
#include "utils/color.h"
#include "utils/logger.h"
#include "utils/memory.h"
#include "utils/stb.hh"
#include "utils/hash.h"

#include "sdk/interfaces.h"

namespace game
{
    inline interfaces_t interfaces;
}

namespace cheat
{
    inline vec2_t screen_size {};
    inline matrix4x4_t matrix;
    inline cs_player* local;
} // namespace cheat
