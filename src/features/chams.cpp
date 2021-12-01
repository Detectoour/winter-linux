#include "../winter.h"
#include "chams.h"
#include "../hooks/hooks.h"
#include <cstring>
#include "../config.h"
#include "../sdk/classes/key_values.h"

using namespace hook_funcs;
material_t* normal_chams;
material_t* flat_material;

void features::chams::initialize()
{
    auto create_material = [](std::string_view material_name, std::string_view shader, std::string_view base = "vgui/white", bool ignorez = false)
    {
        const auto content = fmt::format(
          R"#("{shader}"
	{{
		"$basetexture"		"{texture}"
		"$model"			"1"
		"$ignorez"			"{ignorez}"
		"$selfillum"		"1"
		"$halflambert"		"1"
		"$nofog"			"1"
	}})#",
          fmt::arg("shader", shader),
          fmt::arg("texture", base),
          fmt::arg("ignorez", ignorez ? 1 : 0));

        auto kv = new key_values(shader.data());
        game::addresses.init_kv.cast<void (*)(key_values*, const char*)>()(kv, shader.data());
        game::addresses.load_from_buffer
          .cast<void (*)(key_values*, const char*, const char*, void*, const char*, void*)>()(kv, material_name.data(), content.c_str(), nullptr, nullptr, nullptr);
        return game::interfaces.material_system->create_material(material_name.data(), kv);
    };

    normal_chams  = create_material("winter_chams_normal", "VertexLitGeneric");
    flat_material = game::interfaces.material_system->find_material("debug/debugdrawflat", nullptr);
}

bool features::chams::draw_model_pre(void* ptr, void* result, const draw_model_info_t& info, matrix3x4_t* bone_to_world, float* weight, float* delayed_weight, const vec3_t* origin, int flags)
{
    if (!cheat::config.get<bool>("chams.enabled"))
        return false;
    if (!game::interfaces.engine->in_game() || !ptr || !info.m_renderable)
        return false;
    if (flags & (STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE | STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS))
        return false;

    auto entity = reinterpret_cast<cs_player*>(uintptr_t(info.m_renderable) - 0x8);
    if (!entity || entity == cheat::local || entity->networkable()->dormant())
        return false;

    if (!entity->is_player())
        return false;

    const std::string_view model_name = game::interfaces.model_info->get_model_name(entity->renderable()->model());

    if (model_name.find("player/") != std::string_view::npos)
    {
        game::interfaces.studio_render->set_color(color_t(255, 127, 127));
        game::interfaces.studio_render->set_alpha(0.7f);
        game::interfaces.studio_render->forced_material_override(flat_material);
    }

    return true;
}

bool features::chams::draw_model_post(void* ptr, void* result, const draw_model_info_t& info, matrix3x4_t* bone_to_world, float* weight, float* delayed_weight, const vec3_t* origin, int flags)
{
    if (!game::interfaces.engine->in_game() || !ptr || !info.m_renderable)
        return false;

    auto entity = reinterpret_cast<cs_player*>(uintptr_t(info.m_renderable) - 0x8);

    if (!entity || entity == cheat::local || entity->networkable()->dormant())
        return false;

    if (!entity->is_player())
        return false;

    const std::string_view model_name = game::interfaces.model_info->get_model_name(entity->renderable()->model());

    if (model_name.find("player/") != std::string_view::npos)
    {
        game::interfaces.studio_render->set_color(color_t(255, 127, 127));
        game::interfaces.studio_render->set_alpha(0.7f);
        game::interfaces.studio_render->forced_material_override(flat_material);
    }

    return true;
}