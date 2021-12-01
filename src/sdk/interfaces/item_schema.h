#pragma once

struct item_schema_t
{
private:
    // Credit: namazso
    template <typename k, typename v>
    struct node_t
    {
        int previous_id;    // 0x0000
        int next_id;        // 0x0004
        void* _unknown_ptr; // 0x0008
        int _unknown;       // 0x0010
        k key;              // 0x0014
        v value;            // 0x0018
    };

    template <typename k, typename v>
    struct head_t
    {
        node_t<k, v>* memory; // 0x0000
        int allocation_count; // 0x0008
        int grow_size;        // 0x000C
        int start_element;    // 0x0010
        int next_available;   // 0x0014
        int _unknown;         // 0x0018
        int last_element;     // 0x0020
    };                        // Size=0x0024

    struct string_t
    {
        char* buffer;  // 0x0000
        int capacity;  // 0x0008
        int grow_size; // 0x000C
        int length;    // 0x0010
    };                 // Size=0x0014

private:
    struct attribute_string_t
    {
    public:
        virtual ~attribute_string_t();

        int32_t field_set;
        char** value;

    private:
        int32_t pad0[2];
    };
    // ItemDefinition part
    union attribute_data_union_t
    {
        bool v_bool;
        int v_int;
        float v_float;
        vec3_t* v_vec;
        attribute_string_t* v_string;
    };

    struct static_attrib_t
    {
        uint16_t id;
        attribute_data_union_t value;
        bool force_gc_to_generate;
    };

public:
    struct econ_item_definition_t
    {
        virtual int get_definition_index()        = 0;
        virtual const char* get_prefab_name()     = 0;
        virtual const char* get_item_base_name()  = 0;
        virtual const char* get_item_type_name()  = 0;
        virtual const char* get_item_desc()       = 0;
        virtual const char* get_inventory_image() = 0;
        auto get_item_attributes()
        {
            // Search any weapon attribute name from item_games.txt, such as "is revolver", "cannot shoot underwater" etc.
            // xref to the function that only call the string once and decompile it
            // scroll down to the bottom, there should be something like this:
            // *(_BYTE *)(a1 + 778) = (unsigned int)sub_A96810(a1, v408, 0) != 0;
            // Decompile sub_A96810 and you will see a function with 2 parameters is called in a if statement, that is CEconItemDefinition::IterateAttributes, decompile it
            // you will see something like: v5 = (unsigned __int16 *)(*(_QWORD *)(a1 + 0x50) + v3); at the very beginning of a do-while loop, 0x50 is the offset we are looking for
            // select that line, go to IDA-View and sync with the pseudocode and sig it
            static auto offset = game::addresses.offset_item_attribute.add(3).get().cast<std::int8_t>();
            return *reinterpret_cast<utl_vector<static_attrib_t>*>(reinterpret_cast<uintptr_t>(this) + offset);
        }
    };

private:
    struct econ_item_definition_hashmap_entry_t
    {
        econ_item_definition_t* definition;
        int size;
        int unk;
        int index;
        int unk2;
    };

    struct econ_item_definition_hashmap_t
    {
        void* pad;
        econ_item_definition_hashmap_entry_t entries[2048];
    };

    auto get_item_definition_map()
    {
        // function sig: 55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 83 EC 18 80 BF ? ? ? ? ?
        // vtable index: 41 from vtable CCStrike15ItemSchema
        // .text:0000000000CF0868 49 03 84 24 20 01 00 00                add     rax, [r12+120h] ; <-- 0x120 is the offset we need
        // .text:0000000000CF0870 83 78 10 FF                            cmp     dword ptr [rax+10h], 0FFFFFFFFh
        // .text:0000000000CF0874 7C 1D                                  jl      short loc_CF0893
        static auto offset = game::addresses.offset_item_definition.add(4).get().cast<std::int32_t>();
        return *reinterpret_cast<econ_item_definition_hashmap_t**>(reinterpret_cast<uintptr_t>(this) + offset);
    }

public:
    auto get_item_definition_count()
    {
        return memory::get_vfunc<int (*)(void*)>(this, 3)(this);
    }

    econ_item_definition_t* get_item_definition_by_index(int index)
    {
        if (index < 0 || index >= get_item_definition_count())
            return nullptr;

        return this->get_item_definition_map()->entries[index].definition;
    }

    // PaintKit and Sticker
private:
    struct paintkit_t
    {
        int id;
        string_t name;
        string_t description;
        string_t item_name;

    private:
        string_t material_name;
        string_t image_inventory;
        string_t pad_1;
        string_t pad_2;
        bool pad_3;

    public:
        int rarity;
    };

public:
    auto get_paintkit()
    {
        //.text:0000000000CF99A0                                                 ; __int64 __fastcall GetPaintKitDefinition(__int64 a1, int PaintKitID)
        //.text:0000000000CF99A0 55                                                              push    rbp
        //.text:0000000000CF99A1 48 89 E5                                                        mov     rbp, rsp
        //.text:0000000000CF99A4 53                                                              push    rbx
        //.text:0000000000CF99A5 48 89 FB                                                        mov     rbx, rdi
        //.text:0000000000CF99A8 48 8D BF 70 03 00 00                                            lea     rdi, [rdi+370h] ; 0x370 is the offset we need
        //.text:0000000000CF99AF 48 83 EC 18                                                     sub     rsp, 18h
        //.text:0000000000CF99B3 89 75 E0                                                        mov     [rbp+var_20], esi
        //.text:0000000000CF99B6 48 8D 75 E0                                                     lea     rsi, [rbp+var_20]
        //.text:0000000000CF99BA E8 D1 91 02 00                                                  call    UtlMap_Find
        //.text:0000000000CF99BF 48 63 C8                                                        movsxd  rcx, eax
        //.text:0000000000CF99C2 89 C2                                                           mov     edx, eax
        //.text:0000000000CF99C4 31 C0                                                           xor     eax, eax
        //.text:0000000000CF99C6 48 85 C9                                                        test    rcx, rcx
        //.text:0000000000CF99C9 78 2C                                                           js      short loc_CF99F7
        //.text:0000000000CF99CB 48 63 B3 80 03 00 00                                            movsxd  rsi, dword ptr [rbx+380h]
        //.text:0000000000CF99D2 48 39 F1                                                        cmp     rcx, rsi
        //.text:0000000000CF99D5 7D 20                                                           jge     short loc_CF99F7
        //.text:0000000000CF99D7 3B 93 94 03 00 00                                               cmp     edx, [rbx+394h]
        //.text:0000000000CF99DD 7F 18                                                           jg      short loc_CF99F7
        //.text:0000000000CF99DF 83 FA FF                                                        cmp     edx, 0FFFFFFFFh
        //.text:0000000000CF99E2 74 13                                                           jz      short loc_CF99F7
        //.text:0000000000CF99E4 48 C1 E1 05                                                     shl     rcx, 5
        //.text:0000000000CF99E8 48 03 8B 78 03 00 00                                            add     rcx, [rbx+378h] ; You can also get the offset from here too

        // lea     rdi, [rdi+370h]
        // + 0x8 in order to skip a void pointer, which is lessfunc in UtlMap
        static auto offset = game::addresses.paintkit_definition.add(8).add(3).get().cast<std::int32_t>() + 0x8;
        return reinterpret_cast<head_t<int, paintkit_t*>*>(reinterpret_cast<uintptr_t>(this) + offset);
    }

public:
    struct econ_item_attribute_definition_t
    {
        auto get_index()
        {
            return memory::get_vfunc<int16_t (*)(void*)>(this, 0)(this);
        }

        auto get_name()
        {
            return memory::get_vfunc<const char* (*)(void*)>(this, 1)(this);
        }
    };

    auto get_attributes()
    {
        // Search string "CEconItemSchema::GetAttributeDefinitionByName"
        //.text:0000000000CF33E0                                                 ; __int64 __fastcall CEconItemSchema::GetAttributeDefinitionByName(__int64 a1, const char* name)
        //.text:0000000000CF33E0 55                                                              push    rbp
        //.text:0000000000CF33E1 48 89 E5                                                        mov     rbp, rsp
        //.text:0000000000CF33E4 41 57                                                           push    r15
        //.text:0000000000CF33E6 41 56                                                           push    r14
        //.text:0000000000CF33E8 49 89 F6                                                        mov     r14, rsi
        //.text:0000000000CF33EB 41 55                                                           push    r13
        //.text:0000000000CF33ED 41 54                                                           push    r12
        //.text:0000000000CF33EF 49 89 FC                                                        mov     r12, rdi
        //.text:0000000000CF33F2 53                                                              push    rbx
        //.text:0000000000CF33F3 48 83 EC 18                                                     sub     rsp, 18h
        //.text:0000000000CF33F7 4C 8B 2D 9A 50 52 01                                            mov     r13, cs:g_VProfCurrentProfile_ptr
        //.text:0000000000CF33FE 45 8B 95 0C 10 00 00                                            mov     r10d, [r13+100Ch]
        //.text:0000000000CF3405 45 85 D2                                                        test    r10d, r10d
        //.text:0000000000CF3408 0F 95 45 CF                                                     setnz   [rbp+var_31]
        //.text:0000000000CF340C 0F 85 E6 00 00 00                                               jnz     loc_CF34F8
        //.text:0000000000CF3412
        //.text:0000000000CF3412                                                 loc_CF3412:                             ; CODE XREF: CEconItemSchema__GetAttributeDefinitionByName+127↓j
        //.text:0000000000CF3412                                                                                         ; CEconItemSchema__GetAttributeDefinitionByName+188↓j
        //.text:0000000000CF3412 45 8B 8C 24 90 01 00 00                                         mov     r9d, [r12+190h]
        //.text:0000000000CF341A 31 DB                                                           xor     ebx, ebx
        //.text:0000000000CF341C 45 85 C9                                                        test    r9d, r9d
        //.text:0000000000CF341F 7E 43                                                           jle     short loc_CF3464
        //.text:0000000000CF3421
        //.text:0000000000CF3421                                                 loc_CF3421:                             ; CODE XREF: CEconItemSchema__GetAttributeDefinitionByName+82↓j
        //.text:0000000000CF3421 49 8B 84 24 80 01 00 00                                         mov     rax, [r12+180h] ; <-- 0x180 is the offset we need

        static auto offset = game::addresses.offset_schema_attribute.add(4).get().cast<std::int32_t>();
        return *reinterpret_cast<utl_vector<econ_item_attribute_definition_t*>*>(reinterpret_cast<uintptr_t>(this) + offset);
    }

    econ_item_attribute_definition_t* get_attribute_by_name(const char* target_name)
    {
        static auto attributes = get_attributes();
        for (auto i = 0; i < attributes.count(); i++)
        {
            auto attribute = attributes[i];
            if (!memory::is_valid_address(attribute))
                continue;

            const auto name = attribute->get_name();
            if (!strcmp(target_name, name))
                return attribute;
        }
        return nullptr;
    }

    int16_t get_attribute_id_by_name(const char* target_name)
    {
        static auto attributes = get_attributes();
        for (auto i = 0; i < attributes.count(); i++)
        {
            auto attribute = attributes[i];
            if (!memory::is_valid_address(attribute))
                continue;

            const auto name = attribute->get_name();
            if (!strcmp(target_name, name))
            {
                // Could have used the vfunc but sometimes the address of (this) is 0x0...
                const auto id = *reinterpret_cast<int16_t*>(reinterpret_cast<uintptr_t>(attribute) + 0x10);
                return id;
            }
        }
        return -1;
    }
};