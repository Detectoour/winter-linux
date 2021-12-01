#pragma once

#include "netvars.h"

struct client_renderable
{
    model_t* model()
    {
        return memory::get_vfunc<model_t* (*)(void*)>(this, 8)(this);
    }
};

struct client_networkable
{
    client_class* get_client_class()
    {
        return memory::get_vfunc<client_class* (*)(void*)>(this, 2)(this);
    }

    bool dormant()
    {
        return memory::get_vfunc<bool (*)(void*)>(this, 9)(this);
    }

    int index()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 10)(this);
    }
};

struct base_entity
{
    client_renderable* renderable()
    {
        return reinterpret_cast<client_renderable*>(reinterpret_cast<uintptr_t>(this) + 0x8);
    }

    client_networkable* networkable()
    {
        return reinterpret_cast<client_networkable*>(reinterpret_cast<uintptr_t>(this) + 0x10);
    }

    // Maybe we don't need this, since we already have m_vecAbsOrigin
    /*    const vec3_t &get_abs_origin()
    {
        return memory::get_vfunc< const vec3_t &( * )( void * ) >( this,
                                                                   12 )( this );
    }*/

    datamap_t* data_desc_map()
    {
        return memory::get_vfunc<datamap_t* (*)(void*)>(this, 18)(this);
    }

    datamap_t* pred_datamap()
    {
        return memory::get_vfunc<datamap_t* (*)(void*)>(this, 21)(this);
    }

    void draw_model(int flags, uint8_t alpha)
    {
        return memory::get_vfunc<void (*)(void*, int, uint8_t)>(this, 64)(this, flags, alpha);
    }

    bool should_draw()
    {
        return game::addresses.should_draw.cast<bool (*)(void*)>()(this);
    }

    bool is_player()
    {
        return memory::get_vfunc<bool (*)(void*)>(this, 210)(this);
    }

    NETVAR(vec3_t, mins, "DT_BaseEntity->m_vecMins")
    NETVAR(vec3_t, maxs, "DT_BaseEntity->m_vecMaxs")
    NETVAR(vec3_t, origin, "DT_BaseEntity->m_vecOrigin")
    NETVAR(int, team, "DT_BaseEntity->m_iTeamNum")
    NETVAR(bool, spotted, "DT_BaseEntity->m_bSpotted")
    NETVAR(int, model_index, "DT_BaseEntity->m_nModelIndex")
    NETVAR(float, anim_time, "DT_BaseEntity->m_flAnimTime")
    NETVAR(int, collision_group, "DT_BaseEntity->m_CollisionGroup")

    NETVAR(move_type_t, move_type, "DT_BaseEntity->m_MoveType")
    NETVAR(char, move_collide, "DT_BaseEntity->m_MoveCollide")
    NETVAR(vec3_t, abs_velocity, "DT_BaseEntity->m_vecAbsVelocity")
    NETVAR(vec3_t, velocity, "DT_BaseEntity->m_vecVelocity")
    NETVAR(bitflag_t, flags, "DT_BaseEntity->m_fFlags")
    NETVAR(vec3_t, view_offset, "DT_BaseEntity->m_vecViewOffset")
    NETVAR(float, friction, "DT_BaseEntity->m_flFriction")
    NETVAR(int, health, "DT_BaseEntity->m_iHealth")
    NETVAR(size_t, network_move_parent_handle, "DT_BaseEntity->m_hNetworkMoveParent")
    NETVAR(vec3_t, network_origin, "DT_BaseEntity->m_vecNetworkOrigin")
    NETVAR(vec3_t, network_angles, "DT_BaseEntity->m_angNetworkAngles")
    NETVAR(vec3_t, abs_origin, "DT_BaseEntity->m_vecAbsOrigin")
    NETVAR(vec3_t, ang_abs_rotation, "DT_BaseEntity->m_angAbsRotation")
    NETVAR(size_t, ground_entity_handle, "DT_BaseEntity->m_hGroundEntity")
    NETVAR(water_level_t, water_level, "DT_BaseEntity->m_nWaterLevel")
    NETVAR(char, water_type, "DT_BaseEntity->m_nWaterType")
    NETVAR(vec3_t, ang_velocity, "DT_BaseEntity->m_vecAngVelocity")
    NETVAR(vec3_t, base_velocity, "DT_BaseEntity->m_vecBaseVelocity")
    NETVAR(bitflag_t, effect_flags, "DT_BaseEntity->m_iEFlags")
    NETVAR(float, gravity, "DT_BaseEntity->m_flGravity")
    NETVAR(float, proxy_random_value, "DT_BaseEntity->m_flProxyRandomValue")
    NETVAR(bool, ever_had_prediction_errors_for_this_command, "DT_BaseEntity->m_bEverHadPredictionErrorsForThisCommand")

    NETVAR(const matrix3x4_t, coordinate_frame, "DT_BaseEntity->m_rgflCoordinateFrame")
    NETVAR(const char*, m_ModelName, "DT_BaseEntity->m_ModelName")
};

// To find the offsets you need
// just search this signature:
// 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 89 FB 48 83 EC 08 4C 8B 67 18
// the signature itself is the weapon_xxx.txt parser
// Take armor_ratio for example:
// the offset you get from ida is:
// 368(dec) or 0x170(hex)
// then the actually offset should be:
// ida_offset + 0x4 ==> 372(dec) or 0x174(hex)
// because it somehow doesn't count the destructor function, at least in my case

struct weapon_info_t
{
    OFFSET(char*, console_name, 0x8)
    OFFSET(int, damage, 0x16C)
    OFFSET(float, armor_ratio, 0x174)
    OFFSET(float, penetration, 0x17C)
    OFFSET(float, range, 0x188)
    OFFSET(float, range_modifier, 0x18C)
    OFFSET(float, max_speed, 0x1B8)
};

struct weapon : base_entity
{
    NETVAR(int, ammo, "DT_BaseCombatWeapon->m_iClip1")
    NETVAR(int, primary_reserve_ammo_count, "DT_BaseCombatWeapon->m_iPrimaryReserveAmmoCount")
    NETVAR(int, secondary_reserve_ammo_count, "DT_BaseCombatWeapon->m_iSecondaryReserveAmmoCount")
    NETVAR(int, fallback_paint_kit, "DT_BaseCombatWeapon->m_nFallbackPaintKit")
    NETVAR(int, fallback_seed, "DT_BaseCombatWeapon->m_nFallbackSeed")
    NETVAR(float, fallback_wear, "DT_BaseCombatWeapon->m_flFallbackWear")
    NETVAR(int, fallback_stat_trak, "DT_BaseCombatWeapon->m_nFallbackStatTrak")
    NETVAR(int, entity_quality, "DT_BaseCombatWeapon->m_iEntityQuality")
    NETVAR(int, account_id, "DT_BaseCombatWeapon->m_iAccountID")
    NETVAR(int, item_id_high, "DT_BaseCombatWeapon->m_iItemIDHigh")
    NETVAR(int, item_id_low, "DT_BaseCombatWeapon->m_iItemIDLow")
    NETVAR(const char*, custom_name, "DT_BaseCombatWeapon->m_szCustomName")
    NETVAR(int16_t, item_definition_index, "DT_BaseCombatWeapon->m_iItemDefinitionIndex")

    void update_accuracy_penalty()
    {
        return game::addresses.weapon_update_accuracy_penalty.cast<void (*)(void*)>()(this);
    }

    weapon_info_t* weapon_info()
    {
        return game::addresses.weapon_info.cast<weapon_info_t* (*)(void*)>()(this);
    }
};

struct planted_c4 : weapon
{
    NETVAR(bool, bomb_ticking, "DT_PlantedC4->m_bBombTicking")
    NETVAR(int, bomb_site, "DT_PlantedC4->m_nBombSite")
    NETVAR(float, c4_blow, "DT_PlantedC4->m_flC4Blow")
    NETVAR(float, timer_length, "DT_PlantedC4->m_flTimerLength")
    NETVAR(float, defuse_length, "DT_PlantedC4->m_flDefuseLength")
    NETVAR(float, defuse_count_down, "DT_PlantedC4->m_flDefuseCountDown")
    NETVAR(int, bomb_defused, "DT_PlantedC4->m_bBombDefused")
    NETVAR(std::size_t, bomb_defuser_handle, "DT_PlantedC4->m_hBombDefuser")
};

struct grenade : weapon
{
    NETVAR(float, damage, "DT_BaseGrenade->m_flDamage")
    NETVAR(float, damage_radius, "DT_BaseGrenade->m_DmgRadius")
    NETVAR(bool, live, "DT_BaseGrenade->m_bIsLive")
    NETVAR(std::size_t, thrower_handle, "DT_BaseGrenade->m_hThrower")

    NETVAR(bool, held_by_player, "DT_BaseCSGrenade->m_bIsHeldByPlayer")
    NETVAR(bool, pin_pulled, "DT_BaseCSGrenade->m_bPinPulled")
    NETVAR(float, throw_time, "DT_BaseCSGrenade->m_fThrowTime")
};

struct base_player : base_entity
{
    NETVAR(vec3_t, velocity, "DT_BasePlayer->m_vecVelocity[0]")
    NETVAR(int, tick_base, "DT_BasePlayer->m_nTickBase")
    NETVAR(int, health, "DT_BasePlayer->m_iHealth")
    NETVAR(float, max_speed, "DT_BasePlayer->m_flMaxspeed")
    NETVAR(vec3_t, view_offset, "DT_BasePlayer->m_vecViewOffset[0]")
    NETVAR(float, simulation_time, "DT_BasePlayer->m_flSimulationTime")

    NETVAR(int, stepside, "DT_BasePlayer->m_nStepside")
    NETVAR(int, old_buttons, "DT_BasePlayer->m_nOldButtons")
    NETVAR(vec3_t, old_origin, "DT_BasePlayer->m_oldOrigin")
    NETVAR(bool, touched_phys_object, "DT_BasePlayer->m_bTouchedPhysObject")
    NETVAR(bool, physics_was_frozen, "DT_BasePlayer->m_bPhysicsWasFrozen")
    NETVAR(vec3_t, new_vphysics_position, "DT_BasePlayer->m_vNewVPhysicsPosition")
    NETVAR(vec3_t, new_vphysics_velocity, "DT_BasePlayer->m_vNewVPhysicsVelocity")
    NETVAR(int, buttons, "DT_BasePlayer->m_nButtons")
    NETVAR(float, water_jump_time, "DT_BasePlayer->m_flWaterJumpTime")
    NETVAR(int, impulse, "DT_BasePlayer->m_nImpulse")
    NETVAR(float, step_sound_time, "DT_BasePlayer->m_flStepSoundTime")
    NETVAR(float, swim_sound_time, "DT_BasePlayer->m_flSwimSoundTime")
    NETVAR(float, ignore_ladder_jump_time, "DT_BasePlayer->m_ignoreLadderJumpTime")
    NETVAR(bool, has_walk_moved_since_last_jump, "DT_BasePlayer->m_bHasWalkMovedSinceLastJump")
    NETVAR(int, physics, "DT_BasePlayer->m_flPhysics")
    NETVAR(int, af_button_last, "DT_BasePlayer->m_afButtonLast")
    NETVAR(int, af_button_pressed, "DT_BasePlayer->m_afButtonPressed")
    NETVAR(int, af_button_released, "DT_BasePlayer->m_afButtonReleased")
    NETVAR(size_t, view_model_handle, "DT_BasePlayer->m_hViewModel")
    NETVAR(float, surface_friction, "DT_BasePlayer->m_surfaceFriction")
    NETVAR(vec3_t, previously_predicted_origin, "DT_BasePlayer->m_vecPreviouslyPredictedOrigin")

    vec3_t eye_pos()
    {
        return origin() + view_offset();
    }

    base_entity* ground_entity()
    {
        return game::addresses.player_ground_entity.cast<base_entity* (*)(void*)>()(this);
    }
};

struct cs_player : base_player
{
    NETVAR(bool, has_heavy_armor, "DT_CSPlayer->m_bHasHeavyArmor")
    NETVAR(bool, has_helmet, "DT_CSPlayer->m_bHasHelmet")
    NETVAR(int, money, "DT_CSPlayer->m_iAccount")
    NETVAR(int, armor, "DT_CSPlayer->m_ArmorValue")
    NETVAR(bool, has_defuser, "DT_CSPlayer->m_bHasDefuser")
    NETVAR(int, life_state, "DT_CSPlayer->m_lifeState")
    NETVAR(float, flash_max_alpha, "DT_CSPlayer->m_flFlashMaxAlpha")
    NETVAR(float, flash_duration, "DT_CSPlayer->m_flFlashDuration")
    NETVAR(std::size_t, weapon_handle, "DT_CSPlayer->m_hActiveWeapon")
    NETVAR(bool, defusing, "DT_CSPlayer->m_bIsDefusing")
    NETVAR(int, survival_team, "DT_CSPlayer->m_nSurvivalTeam")
    NETVAR(int, shots_fired, "DT_CSPlayer->m_iShotsFired")
    NETVAR(std::size_t, observer_target_handle, "DT_CSPlayer->m_hObserverTarget")
    NETVAR(int, bbserver_mode, "DT_CSPlayer->m_iObserverMode")
    NETVAR(float, max_speed, "DT_CSPlayer->m_flMaxspeed")
    NETVAR(int, tick_base, "DT_CSPlayer->m_nTickBase")
    NETVAR(vec3_t, aim_punch_angle_vel, "DT_CSPlayer->m_aimPunchAngleVel")
    NETVAR(vec3_t, aim_punch_angle, "DT_CSPlayer->m_aimPunchAngle")
    NETVAR(vec3_t, view_punch_angle, "DT_CSPlayer->m_viewPunchAngle")
    NETVAR(float, molotov_use_time, "DT_CSPlayer->m_fMolotovUseTime")
    NETVAR(float, molotov_damage_time, "DT_CSPlayer->m_fMolotovDamageTime")
    NETVAR(bool, is_scoped, "DT_CSPlayer->m_bIsScoped")

    bool has_c4()
    {
        return game::addresses.player_has_c4.cast<bool (*)(void*)>()(this);
    }

    weapon* active_weapon()
    {
        return game::addresses.player_active_weapon.cast<weapon* (*)(void*)>()(this);
    }

    // basically life_state == 0, but I prefer this one
    bool alive()
    {
        return game::addresses.player_alive.cast<bool (*)(void*)>()(this);
    }
};
