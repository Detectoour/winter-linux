# Winter-linux
An internal training software base for csgo linux

# Installation
1. Install packages
    
    Arch:
    
    ```
    sudo pacman -S --needed base-devel git cmake gdb sdl2 clang fmt
    ```
2. Clone the repository
    ```
    git clone --recursive https://github.com/NukoOoOoOoO/winter-linux
    ```
3. Type these commands
    ```
    cd winter-linux
    chmod +x tool.sh
    ./tool.sh -b -l #build in release mode, load release binary file
    ```

# Features
- Bunnyhop
- Airstrafe
- Simple esp
- Simple chams
- Color modulation for walls and props
- Detour hooking ( does it count? )
- Datamap dumping
- Parsing WeaponItemIndex, WeaponInfo, Skins dynamically

# Notes
- This is only a **base**, I will update it whenever I want, and there is no warranty
- Pull requests are welcome
- Code reviews are welcome
- Logs, netvar & pred datamap & desc datamap dumps are stored in ``/tmp/``, file names are ``winter.log``, ``netvar.txt``, ``datamap.txt``, ``data_desc_map.txt``

# Know issues
- Sometimes enabling nightmode will crash the game

# Credits
- [XutaxKamay](https://github.com/XutaxKamay) for helping me with detour and other stuff
- [Fuzion](https://github.com/LWSS/Fuzion) & [gamesneeze](https://github.com/seksea/gamesneeze) & [HelvetaCS](https://github.com/cristeigabriel/HelvetaCS) for code & ideas
- [gamesneeze](https://github.com/seksea/gamesneeze) for the `tool.sh` file, thank you guys so much!
- [xemacs](https://github.com/cristeigabriel) for [the static prop color modulation](https://www.unknowncheats.me/forum/counterstrike-global-offensive/450356-fast-path-static-prop-color-alpha-modulation-shader-params.html)
- And some that I may forget