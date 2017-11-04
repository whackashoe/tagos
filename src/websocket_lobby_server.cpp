#include "websocket_server.hpp"
#include "websocket_lobby_server.hpp"

void on_lobby_request_games(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
) {
    const lobby_server& lobby = lobby_server::get_instance();

    std::vector<lobby_event_games_game> games;
    games.reserve(lobby.games.size());

    for(auto && o : lobby.games) {
        games.emplace_back(
            o->port,
            o->max_points,
            o->max_length,
            o->timestep,
            o->m->name,
            o->m->author,
            o->m->balls.size()
        );
    }

    try_send(srv, hdl, websocketpp::frame::opcode::TEXT,
        lobby_event(lobby_event_games(games))
    );

}

void handle_lobby_message(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());
        // std::cout << j.dump() << std::endl;

        if(j.at("request").get<std::string>() == "games") {
            return on_lobby_request_games(srv, hdl, msg);
        }

    } catch(...) {
        try_send(srv, hdl, websocketpp::frame::opcode::TEXT, {
            {"error", "json_parse_error"}
        });
    }
}

int start_lobby_server(
    lobby_server& lobby,
    const std::uint16_t port
) {
    spdlog::get("game")->info("starting tagos lobby server on port: {0:d}", port);
    websocketpp::server<websocketpp::config::asio> srv;

    try {
        srv.clear_access_channels(websocketpp::log::alevel::all);
        srv.set_access_channels(websocketpp::log::elevel::info);
        srv.init_asio();
        srv.set_message_handler(bind(&handle_lobby_message,&srv,::_1,::_2));
        srv.listen(port);

        srv.start_accept();
        srv.run();
    } catch (websocketpp::exception const & e) {
        spdlog::get("game")->error("server exception", e.what());
        lobby.is_alive = false;
        return 1;
    } catch (...) {
        spdlog::get("game")->error("unknown server exception");
        lobby.is_alive = false;
        return 1;
    }

    return 0;
}


