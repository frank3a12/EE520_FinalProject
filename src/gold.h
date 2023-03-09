#ifndef __GOLD_AGENT__H
#define __GOLD_AGENT__H 

#include <random>
#include "enviro.h"
#include "parameters.h"
#include "styles.h"

using namespace enviro;

class GoldController : public Process, public AgentInterface {

    public:
    GoldController() : Process(), AgentInterface() {}

    void init() {}
    void start() {}
    void update() 
    {
        // update gold ids
        emit(Event("gold_id", {{"id", id()}}));
    }
    void stop() 
    {
        // find new spot for new gold
        int x = rand() % (g_width) + (g_width / 2) * -1;
        int y = rand() % (g_height) + (g_height / 2) * -1;
        add_agent("Gold", x, y, 0, GOLD_STYLE);

        // also add a new ghost
        x = rand() % (g_width) + (g_width / 2) * -1;
        y = rand() % (g_height) + (g_height / 2) * -1;
        add_agent("Ghost", x, y, 0, GHOST_STYLE);
    }

};

class Gold : public Agent {
    public:
    Gold(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    GoldController c;
};

DECLARE_INTERFACE(Gold)

#endif