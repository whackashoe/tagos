#ifndef ML_GAME_EVENT_TOGGLE_OFF_HPP
#define ML_GAME_EVENT_TOGGLE_OFF_HPP

#include <cstdint>
#include <json/json_fwd.hpp>

struct game_event_toggle_off
{
    const std::size_t ball_id;
    const std::size_t toggle_id;

    game_event_toggle_off(
        const std::size_t ball_id,
        const std::size_t toggle_id
    );
};

void to_json(nlohmann::json& j, const game_event_toggle_off& p);

#endif

