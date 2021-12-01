#include "winter.h"
#include "hooks/hooks.h"
#include "menu/menu.h"
#include "sdk/signatures.h"
#include "config.h"
#include "sdk/game_data.h"

void entry_point()
{
    while (!dlopen("./bin/linux64/serverbrowser_client.so", RTLD_NOLOAD | RTLD_NOW))
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    try
    {
        logger::m_file.open("/tmp/winter.log", std::ios::out | std::ios::trunc);
        memory::initialize_modules();
        game::addresses.setup_patterns();

        if (!game::addresses.setup())
        {
            throw std::runtime_error("Address");
        }

        if (!game::interfaces.setup())
        {
            logger::log("Failed to setup interfaces", log_level::error);
            throw std::runtime_error("Interfaces");
        }

        if (!game::netvar.dump())
        {
            logger::log("Failed to dump netvars", log_level::error);
            throw std::runtime_error("Netvar");
        }

        game::netvar.dump_datamap();
        game::game_data.initialize();

        config_t::initialize();
        cheat::menu.hook();
        if (!hooks::hook())
        {
            logger::log("Failed to setup hooks", log_level::error);
            throw std::runtime_error("Hook");
        }
        logger::log("Finished hooking", log_level::critical);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        logger::log(fmt::format("{}", ex.what()), log_level::error);
        logger::m_file.close();
        return;
    }
}

int __attribute__((constructor)) main()
{
    std::thread main_thread(entry_point);

    main_thread.detach();
    return 0;
}

void __attribute__((destructor)) destroy()
{
    cheat::menu.unhook();
    hooks::unhook();
    logger::log("Unloaded");
    logger::m_file.close();
}