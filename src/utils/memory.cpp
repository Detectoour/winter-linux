#include "../winter.h"
#include <link.h>
#include <chrono>

modules::library_t::library_t()
{
}

modules::library_t::library_t(std::string mod_name, std::vector<region_t> mod_regions)
{
    this->name    = mod_name;
    this->regions = mod_regions;

    for (auto&& region : regions)
    {
        logger::log(fmt::format("{} - {:#09x} - {}", this->name, region.address, region.size));
    }
}

void memory::initialize_modules()
{
    static auto initialized = false;
    if (initialized)
        return;

    dl_iterate_phdr(
      [](struct dl_phdr_info* info, size_t, void*)
      {
          if (strcasestr(info->dlpi_name, "steamapps"))
          {
              std::string library_name(info->dlpi_name);
              library_name = library_name.substr(library_name.find_last_of('/') + 1);

#define INITIALIZE_MODULE(mod_name, library)                                                                                                                                                   \
    if (!strcmp(library_name.c_str(), (mod_name)))                                                                                                                                             \
    {                                                                                                                                                                                          \
        library.name = library_name;                                                                                                                                                           \
        std::vector<modules::library_t::region_t> regions;                                                                                                                                     \
                                                                                                                                                                                               \
        for (int i = 0; i < info->dlpi_phnum; i++)                                                                                                                                             \
        {                                                                                                                                                                                      \
            if (info->dlpi_phdr[i].p_flags & PF_X)                                                                                                                                             \
            {                                                                                                                                                                                  \
                modules::library_t::region_t region;                                                                                                                                           \
                region.address = info->dlpi_addr + info->dlpi_phdr[i].p_vaddr;                                                                                                                 \
                region.size    = info->dlpi_phdr[i].p_memsz;                                                                                                                                   \
                region.data    = reinterpret_cast<uint8_t*>(region.address);                                                                                                                   \
                logger::log(fmt::format("{} - base: {:#09X} - size: {}", library_name, region.address, region.size));                                                                          \
                regions.push_back(region);                                                                                                                                                     \
            }                                                                                                                                                                                  \
        }                                                                                                                                                                                      \
        library.regions = regions;                                                                                                                                                             \
    }                                                                                                                                                                                          \
    else

              INITIALIZE_MODULE("client_client.so", modules::client)
              INITIALIZE_MODULE("engine_client.so", modules::engine)
              INITIALIZE_MODULE("vguimatsurface_client.so", modules::matsurface)
              INITIALIZE_MODULE("studiorender_client.so", modules::studio_render)
              INITIALIZE_MODULE("materialsystem_client.so", modules::material_system)
              INITIALIZE_MODULE("filesystem_stdio_client.so", modules::file_system)
              {
                  // Don't do anything here !!!!!
              }
          }
          return 0;
      },
      nullptr);

    initialized = true;
    logger::log("initialized modules");
}