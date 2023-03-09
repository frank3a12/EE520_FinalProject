#ifndef __PLAYER_AGENT__H
#define __PLAYER_AGENT__H 

#include "enviro.h"
#include "styles.h"

using namespace enviro;

class PlayerController : public Process, public AgentInterface 
{
public:
    PlayerController() : Process(), AgentInterface() {}

    void init();
    void start();
    void update();
    void stop();

private:
    double v, omega;
    double const v_m = 30, omega_m = 1;
};

class Player : public Agent 
{
public:
    Player(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }

private:
    PlayerController c;
};

DECLARE_INTERFACE(Player)

#endif