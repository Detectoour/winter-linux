#include "winter.h"
#include "config.h"
#include "features/world.h"
#include <utility>

void config_t::add(const std::string& name, types default_value)
{
    config_list[name] = default_value;
}

void config_t::add_callback(const std::string& name, config_t::callback callback)
{
    callback_list[name] = std::move(callback);
}

void config_t::initialize()
{
    cheat::config.add("movement.bunnyhop", true);
    cheat::config.add("movement.autostrafe", true);
    cheat::config.add("chams.enabled", false);
    cheat::config.add("esp.enabled", false);
    cheat::config.add("aimbot.fov", 180.f);
    cheat::config.add("esp.nightmode", false);
    cheat::config.add_callback("esp.nightmode",
                               []()
                               {
                                   features::world::prop_color();
                                   return true;
                               });
    cheat::config.add("esp.player.box", false);
    cheat::config.add("esp.player.name", false);
    cheat::config.add("esp.player.health", false);
    cheat::config.add("esp.player.box.clr", color_t(255, 255, 255));
}
