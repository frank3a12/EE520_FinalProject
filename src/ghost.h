#ifndef __GHOST_AGENT__H
#define __GHOST_AGENT__H 

#include "enviro.h"
#include "styles.h"
#include <unordered_set>
#include <math.h>

using namespace enviro;

#define SENSOR_TURN_DEPTH 150
#define SENSOR_HITWALL_DEPTH 50

struct Direction {
    static constexpr double FRONT = 0;
    static constexpr double TURN_LEFT = -1.57;
    static constexpr double TURN_RIGHT = 1.57;
};

class GhostController : public Process, public AgentInterface
{
public:
    GhostController() : Process(), AgentInterface() {}

    void init() {
        // when ghost catch the player
        notice_collisions_with("Player", [&](Event &e) {
            // kill player
            remove_agent(find_agent(e.value()["id"]).get_id());
        });
    }
    void start() {}
    void update() 
    {
        // the center sensor doesn't found the player
        if(sensor_reflection_type(2) != "Player") 
        {
            // if the ghost is too close to the wall
            if(sensor_value(2) < SENSOR_HITWALL_DEPTH)
            {
                // turn left when left side is wider, turn right when right side is wider
                if(sensor_value(0) > sensor_value(1)) track_velocity(-m_v, m_direction + Direction::TURN_LEFT);
                else track_velocity(-m_v, m_direction + Direction::TURN_RIGHT);
            }
            // if the ghost see the wall from a certain distance
            else if(sensor_value(2) < SENSOR_TURN_DEPTH)
            {
                // turn left when left side is wider, turn right when right side is wider
                if(sensor_value(0) > sensor_value(1)) track_velocity(m_v, Direction::TURN_LEFT);
                else track_velocity(m_v, Direction::TURN_RIGHT);
            }
            // if the ghost is very far from the wall
            else
                track_velocity(m_v, m_direction);
        }
        // the center sensor found the player
        else 
        {
            // speed up and try to catch the user
            track_velocity(m_v * 2, sin(angle()));
        }

        // update ghost ids
        emit(Event("ghost_ids", {{"ids", id()}}));
    }
    void stop() {}

private:
    double m_v = 10;
    double m_direction = Direction::FRONT;
};

class Ghost : public Agent 
{
    public:
    Ghost(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    GhostController c;
};

DECLARE_INTERFACE(Ghost)

#endif