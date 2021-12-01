#pragma once

struct global_vars
{
    float real_time;
    int frame_count;
    float absolute_frametime;
    float absolute_frame_start_time_stddev;
    float cur_time;
    float frametime;
    int max_clients;
    int tick_count;
    float interval_per_tick;
    float interpolation_amount;
    int sim_ticks_this_frame;
    int network_protocol;
    void* save_data;
    bool client;
    bool remote_client;

private:
    int timestamp_networking_base;
    int timestamp_randomize_window;
};