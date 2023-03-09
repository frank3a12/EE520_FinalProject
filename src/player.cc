#include <iostream>
#include "player.h"
#include "parameters.h"

using namespace enviro;

// Put your implementations here
void PlayerController::init() 
{
    // when user press down "w,a,s,d", we set the velocity & omega of the player as default
    watch("keydown", [&](Event &e) {
        auto k = e.value()["key"].get<std::string>();
        if ( k == "w" ) {
            v = v_m;              
        } else if ( k == "s" ) {
            v = -v_m;  
        } else if ( k == "a" ) {
            omega = -omega_m;
        } else if ( k == "d" ) {
            omega = omega_m;
        } 
    });
    // when user press up "w,a,s,d", we set the velocity & omega of the player to 0
    watch("keyup", [&](Event &e) {
        auto k = e.value()["key"].get<std::string>();
        if ( k == "w" || k == "s" ) {
            v = 0;               
        } else if ( k == "a" ) {
            omega = 0;
        } else if ( k == "d" ) {
            omega = 0;
        } 
    });

    // if user hit gold 
    notice_collisions_with("Gold", [&](Event &e) {
        // remove gold
        remove_agent(find_agent(e.value()["id"]).get_id());

        // add point
        g_golds += 5;
        emit(Event("add_golds", {{"golds", g_golds}}));
    });

    // show score accordingly
    watch("show_score", [&](Event &e) {
        int val = e.value()["score"];
        label("I got " + std::to_string(val) + " points!", -30, -30);
    });
}
void PlayerController::start() {}
void PlayerController::update() 
{
    // update speed & omega
    track_velocity(v,omega,10,400);
}
void PlayerController::stop() 
{
    // when player died, we create a new player
    add_agent("Player", 0, 0, 0, PLAYER_STYLE);

    // update lives
    emit(Event("lives", {{"live", --g_lives}}));
}