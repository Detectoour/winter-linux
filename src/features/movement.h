#pragma once

namespace features::movement
{
    void autobhop(cs_player* local, user_cmd* cmd);
    void airstrafe(cs_player* local, user_cmd* cmd);

    inline float last_yaw;
}