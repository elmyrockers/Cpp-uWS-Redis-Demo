module;
#include <string>
#include <print>
#include <uwebsockets/App.h>

export module chatroom:server;

export namespace chatroom {
	struct PerSocketData {
		std::string username;
		std::string connectionId;
	};
	class Server {
		uWS::App::WebSocketBehavior<PerSocketData> behavior;
		public:
			Server() = default;
			void start( int port ) {
				// Run the websocket server
					uWS::App()
							.ws<PerSocketData>("/*", std::move(behavior))
							.listen( port, [&port](auto *token) {
								if (token) {
									std::print("Server listening on port {}\n", port);
								} else {
									std::print("Failed to listen on port {}\n", port);
								}
							}).run();
			}
	};
}