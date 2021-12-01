#pragma once

namespace features::glow
{
    inline std::vector<cs_player*> ents;
    inline bool drawing;
    void initialize();
    void render();
} // namespace features::glow