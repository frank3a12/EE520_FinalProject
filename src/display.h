#ifndef __DISPLAY_AGENT__H
#define __DISPLAY_AGENT__H 

#include <random>
#include "enviro.h"
#include "parameters.h"
#include "styles.h"

using namespace enviro;

class displayController : public Process, public AgentInterface 
{
public:
    displayController() : Process(), AgentInterface() {}

    void init() 
    {
        // set label as 3 lives & 0 golds
        label("Current lives: " + std::to_string(m_lives) + ", points: " + std::to_string(m_golds), 0, 0);
        
        // init member vars for time
        time_t t = std::time(0);
        m_begin_time = static_cast<long int>(t);
        m_prev_add_ghost_time = 0;
    }
    void start() {}
    void update() 
    {
        // update lives
        watch("lives", [&](Event& e) {
            m_lives = e.value()["live"];
        });

        // update golds
        watch("add_golds", [&](Event& e) {
            m_golds = e.value()["golds"];
        });

        // display messages
        label("Current lives: " + std::to_string(m_lives) + ", points: " + std::to_string(m_golds), 0, 0);

        // automatically add ghost after each 5 seconds
        if(m_lives != 0)
        {
            // get current time
            time_t t = std::time(0);
            long int current_time = static_cast<long int>(t);

            // add a ghost after 5 seconds
            if((current_time - m_begin_time) != m_prev_add_ghost_time && (current_time - m_begin_time) % 5 == 0) {
                
                // generate random place in the scope
                int x = rand() % (g_width) + (g_width / 2) * -1;
                int y = rand() % (g_height) + (g_height / 2) * -1;
                add_agent("Ghost", x, y, 0, GHOST_STYLE);
                
                // update prev time
                m_prev_add_ghost_time = current_time - m_begin_time;
            }
        }
        else
        {
            // remove ghosts
            watch("ghost_ids", [&](Event& e) {
                auto id = e.value()["ids"];
                if(agent_exists(id)) remove_agent(id);
            });

            // remove gold (set as invisible)
            watch("gold_id", [&](Event& e) {
                auto id = e.value()["id"];
                Agent &gold = find_agent(id);
                gold.set_style(EMPTY_STYLE);
            });

            // update scores to player
            emit(Event("show_score", {{"score", m_golds}}));
        }
    }
    void stop() {}

private:
    int m_lives = g_lives;
    int m_golds = g_golds;
    long int m_begin_time;
    int m_prev_add_ghost_time;
};

class display : public Agent {
    public:
    display(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    displayController c;
};

DECLARE_INTERFACE(display)

#endif