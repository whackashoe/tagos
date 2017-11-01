#ifndef ML_GATE_HPP
#define ML_GATE_HPP

#include <string>
#include <iostream>

#include <json.hpp>
#include "polygon.hpp"

#include "gate_type.hpp"
#include "ball.hpp"


struct gate
{
    polygon poly;
    gate_type type;
    gate_type current;
    std::shared_ptr<collision_user_data> col_data;
    int red_minus_blue = 0;
    b2Body* body;

    gate(){}
    gate(
        const polygon poly,
        const gate_type type
    )
    : poly(poly)
    , type(type)
    , current(type)
    , body(nullptr)
    {}

    void mark_on(ball* b);
    void mark_off(ball* b);
    void kill_if_other(ball* b);
    void add_to_world(b2World* world);
};

void to_json(nlohmann::json& j, const gate& p);
void from_json(const nlohmann::json& j, gate& p);

#endif
