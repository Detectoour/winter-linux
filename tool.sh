#!/bin/bash

gdb="$(dirname "$0")/gdb"
libname="libgamemodeauto.so.0"
csgo_pid=$(pidof csgo_linux64)

if [[ $EUID -eq 0 ]]; then
    echo "You cannot run this as root."
    exit 1
fi

rm -rf /tmp/dumps
mkdir -p --mode=000 /tmp/dumps

function unload {
    echo "Unloading cheat..."
    echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
    if grep -q "$libname" "/proc/$csgo_pid/maps"; then
        $gdb -n -q -batch -ex "attach $csgo_pid" \
            -ex "set \$dlopen = (void*(*)(char*, int)) dlopen" \
            -ex "set \$dlclose = (int(*)(void*)) dlclose" \
            -ex "set \$library = \$dlopen(\"/usr/lib/$libname\", 6)" \
            -ex "call \$dlclose(\$library)" \
            -ex "call \$dlclose(\$library)" \
            -ex "detach" \
            -ex "quit"
    fi
    echo "Unloaded!"
}

function load {
    echo "Loading cheat..."
    echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope > /dev/null
    sudo cp cmake-build-release/libwinter.so /usr/lib/$libname
    gdbOut=$(
      $gdb -n -q -batch \
      -ex "set auto-load safe-path /usr/lib/" \
      -ex "attach $csgo_pid" \
      -ex "set \$dlopen = (void*(*)(char*, int)) dlopen" \
      -ex "call \$dlopen(\"/usr/lib/$libname\", 1)" \
      -ex "detach" \
      -ex "quit" 2> /dev/null
    )
    lastLine="${gdbOut##*$'\n'}"
    if [[ "$lastLine" != "\$1 = (void *) 0x0" ]]; then
      echo "Successfully injected!"
    else
      echo "Injection failed, make sure you have compiled."
    fi
}

function load_debug {
    echo "Loading cheat..."
    echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
    sudo cp cmake-build-debug/libwinter.so /usr/lib/$libname
    $gdb -n -q -batch \
        -ex "set auto-load safe-path /usr/lib:/usr/lib/" \
        -ex "attach $csgo_pid" \
        -ex "set \$dlopen = (void*(*)(char*, int)) dlopen" \
        -ex "call \$dlopen(\"/usr/lib/$libname\", 1)" \
        -ex "detach" \
        -ex "quit"
    $gdb -p "$csgo_pid"
    echo "Successfully loaded!"
}

function build {
    echo "Building cheat..."
    mkdir -p cmake-build-release
    cd cmake-build-release
    cmake -D CMAKE_BUILD_TYPE=Release ..
    make -j $(nproc --all)
    cd ..
}

function build_debug {
    echo "Building cheat..."
    mkdir -p cmake-build-debug
    cd cmake-build-debug
    cmake -D CMAKE_BUILD_TYPE=Debug ..
    make -j $(nproc --all)
    cd ..
}

while [[ $# -gt 0 ]]
do
keys="$1"
case $keys in
    -u|--unload)
        unload
        shift
        ;;
    -l|--load)
        load
        shift
        ;;
    -ld|--load_debug)
        load_debug
        shift
        ;;
    -b|--build)
        build
        shift
        ;;
    -bd|--build_debug)
        build_debug
        shift
        ;;
    *)
        echo "Unknown arg $1"
        exit
        ;;
esac
done