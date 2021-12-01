#pragma once

typedef unsigned short model_instance_handle_t;

struct model_t
{
    char name[255];
};
struct studiohwdata_t;
struct studio_decal_handle_t;

class studio_hdr_t;

struct draw_model_info_t
{
    studio_hdr_t* m_studioHdr;
    studiohwdata_t* m_studio_hardware_data;
    studio_decal_handle_t* m_decals;
    int m_skin;
    int m_body;
    int m_hitbox_set;
    void* m_renderable;
    int m_lod;
    void* m_pColorMeshes;
    bool m_bStaticLighting;
    vec3_t m_vecAmbientCube[6]; // ambient, and lights that aren't in locallight[]
    int m_nLocalLightCount;
    int m_LocalLightDescs[4];
};

struct model_render_info_t
{
    vec3_t origin;
    angle_t angles;
    char _padding[0x4];
    void* renderable_entity;
    const model_t* model;
    const matrix3x4_t* model_to_world;
    const matrix3x4_t* lighting_offset;
    const vec3_t* lighting_origin;
    int flags;
    int entity_index;
    int skin;
    int body;
    int hitbox_set;
    model_instance_handle_t instance;
    model_render_info_t()
    {
        model_to_world  = nullptr;
        lighting_offset = nullptr;
        lighting_origin = nullptr;
    }
};

#define BONE_USED_BY_ANYTHING    0x000FFF00
#define BONE_USED_BY_HITBOX      0x00000100 // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT  0x00000200 // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK 0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0 0x00000400 // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1 0x00000800
#define BONE_USED_BY_VERTEX_LOD2 0x00001000
#define BONE_USED_BY_VERTEX_LOD3 0x00002000
#define BONE_USED_BY_VERTEX_LOD4 0x00004000
#define BONE_USED_BY_VERTEX_LOD5 0x00008000
#define BONE_USED_BY_VERTEX_LOD6 0x00010000
#define BONE_USED_BY_VERTEX_LOD7 0x00020000
#define BONE_USED_BY_BONE_MERGE  0x00040000 // bone is available for bone merge to occur against it
#define BONE_ALWAYS_SETUP        0x00080000

#define MAXSTUDIOBONES 256

struct studio_bone_t
{
    int name_index;
    inline char* name() const
    {
        return ((char*)this) + name_index;
    }
    int parent;             // parent bone
    int bone_controller[6]; // bone controller index, -1 == none

    // default values
    vec3_t pos;
    quaternion quat;
    radian_euler rot;
    // compression scale
    vec3_t pos_scale;
    vec3_t rot_scale;

    matrix3x4_t pose_to_bone;
    quaternion qAlignment;
    int flags;
    int proc_type;
    int proc_index;           // procedural rule
    mutable int physics_bone; // index into physically simulated bone
    inline void* procedure() const
    {
        if (proc_index == 0)
            return 0;
        else
            return (void*)(((char*)this) + proc_index);
    };
    int surface_prop_idx; // index into string tablefor property name
    inline char* surface_prop(void) const
    {
        return ((char*)this) + surface_prop_idx;
    }
    inline int get_surface_prop(void) const
    {
        return surface_prop_lookup;
    }

    int contents;            // See BSPFlags.h for the contents flags
    int surface_prop_lookup; // this index must be cached by the loader, not saved in the file
    int unused[7];           // remove as appropriate
};

struct studio_bbox_t
{
    int bone;
    int group;
    vec3_t min;
    vec3_t max;
    int hitbox_name_index;
    int pad[3];
    float radius;
    int pad2[4];

    char* hitbox_name()
    {
        if (hitbox_name_index == 0)
            return nullptr;

        return ((char*)this) + hitbox_name_index;
    }
};

struct studio_hitbox_set_t
{
    int name_index;

    inline char* name() const
    {
        return ((char*)this) + name_index;
    }

    int num_hitboxes;
    int hitbox_index;

    inline studio_bbox_t* hitbox(int i) const
    {
        return (studio_bbox_t*)(((unsigned char*)this) + hitbox_index) + i;
    };
};

struct studio_hdr_t
{
    int id;

    int version;

    int checksum; // this has to be the same in the phy and vtx files to load!
    char name[64];

    int length;

    vec3_t eye_position;   // ideal eye position
    vec3_t illum_position; // illumination center
    vec3_t hull_min;       // ideal movement hull size
    vec3_t hull_max;

    vec3_t view_bbmin; // clipping bounding box
    vec3_t view_bbmax;

    int flags;

    int numbones; // bones
    int boneindex;

    inline studio_bone_t* bone(int i) const
    {
        return (studio_bone_t*)(((unsigned char*)this) + boneindex) + std::clamp(i, 0, numbones - 1);
    };

    int remap_seq_bone(int iSequence, int iLocalBone) const; // maps local sequence bone to global bone
    int remap_anim_bone(int iAnim, int iLocalBone) const;    // maps local animations bone to global bone
    int num_bone_controllers;                                // bone controllers
    int bone_controller_index;

    int num_hitbox_sets;

    int hitboxset_index;

    // Look up hitbox set by index
    studio_hitbox_set_t* hitbox_set(int i) const
    {
        return (studio_hitbox_set_t*)(((unsigned char*)this) + hitboxset_index) + i;
    };

    // Calls through to hitbox to determine size of specified set
    inline studio_bbox_t* hitbox(int i, int set) const
    {
        studio_hitbox_set_t const* s = hitbox_set(set);
        if (!s)
            return nullptr;

        return s->hitbox(i);
    };

    // Calls through to set to get hitbox count for set
    inline int hitbox_count(int set) const
    {
        studio_hitbox_set_t const* s = hitbox_set(set);
        if (!s)
            return 0;

        return s->num_hitboxes;
    };
};