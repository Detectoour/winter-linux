#include "../winter.h"
#include "world.h"
#include "../config.h"

void features::world::prop_color(bool clear)
{
    auto props = game::interfaces.static_prop_mgr->get_static_props();
    if (!old_prop_info.count())
        old_prop_info = props;

    if (props.count() > 0)
    {
        auto enabled = cheat::config.get<bool>("esp.nightmode") && !clear;

        for (auto i = 0; i < props.count(); i++)
        {
            // This one doesn't work, I have no idea why
            // props[i].alpha = 32;
            enabled ? props[i].color *= 0.5f : props[i].color = old_prop_info[i].color;
        };
    }
}

void features::world::clear()
{
    if (old_prop_info.count() > 0)
        old_prop_info.remove_all();
}
