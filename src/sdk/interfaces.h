#pragma once

#include "definitions/angles.h"
#include "definitions/vector.h"
#include "definitions/matrix.h"
#include "definitions/game.h"

#include "classes/utl_vector.h"
#include "classes/client_class.h"
#include "classes/user_cmd.h"
#include "classes/models_info.h"
#include "classes/material.h"
#include "classes/texture.h"

#include "signatures.h"

#include "interfaces/base_client.h"
#include "interfaces/cvar.h"
#include "interfaces/engine_client.h"
#include "interfaces/entity_list.h"
#include "interfaces/global_vars.h"
#include "interfaces/surface.h"
#include "interfaces/studio_render.h"
#include "interfaces/model_info.h"
#include "interfaces/model_render.h"
#include "interfaces/material_system.h"
#include "interfaces/render_view.h"
#include "interfaces/static_prop_manager.h"
#include "interfaces/debug_overlay.h"
#include "interfaces/localize.h"
#include "interfaces/hud_chat.h"
#include "interfaces/item_schema.h"

#include "entities.h"

class interfaces_t
{
public:
    cvar_t* convar                         = nullptr;
    surface_t* surface                     = nullptr;
    base_client_t* base_client             = nullptr;
    engine_client_t* engine                = nullptr;
    entity_list_t* entity_list             = nullptr;
    global_vars* global                    = nullptr;
    studio_render_t* studio_render         = nullptr;
    model_info_t* model_info               = nullptr;
    model_render_t* model_render           = nullptr;
    material_system_t* material_system     = nullptr;
    render_view_t* render_view             = nullptr;
    static_prop_manager_t* static_prop_mgr = nullptr;
    debug_overlay_t* debug_overlay         = nullptr;
    localize_t* localize                   = nullptr;
    hud_chat_t* hud_chat                   = nullptr;
    item_schema_t* item_schema             = nullptr;

    bool setup();

private:
    typedef void* (*instantiate_interface_fn)();
    // https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/tier1/interface.h#L72
    class interface_reg
    {
    public:
        instantiate_interface_fn create_fn;
        const char* name;
        interface_reg* next;
    };
    template <typename T>
    T* get(const char* file, const char* name, bool include_version = false)
    {
        auto lib = dlopen(file, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);
        if (lib)
        {
            // https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/tier1/interface.cpp#L46
            auto interface_reg = *reinterpret_cast<class interface_reg**>(dlsym(lib, "s_pInterfaceRegs"));
            dlclose(lib);

            // loop through each interface in interface_reg linked list
            for (auto cur = interface_reg; cur; cur = cur->next)
            {
                // If current interface equals input name without the 3
                // version numbers so if an interface version changes we
                // don't have to care
                if ((strstr(cur->name, name) && strlen(cur->name) - 3 == strlen(name)) || (include_version && (strstr(cur->name, name) && strlen(cur->name) == strlen(name))))
                {
                    T* interface_pointer = reinterpret_cast<T*>(cur->create_fn());
                    logger::log(fmt::format("{} ({}) => {:#09x}", name, cur->name, reinterpret_cast<uintptr_t>(interface_pointer)), log_level::critical);
                    return interface_pointer;
                }
            }
        }
        logger::log(fmt::format("Failed to find interface {} in {}.\n", name, file), log_level::error);

        dlclose(lib);
        return nullptr;
    }
};