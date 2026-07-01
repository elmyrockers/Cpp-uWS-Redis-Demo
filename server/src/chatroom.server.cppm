module;
#include <string>
#include <print>
#include <chrono>
#include <uwebsockets/App.h>

export module chatroom:server;
import :auth;
import :broadcaster;

export namespace chatroom {
	struct PerSocketData {
		std::string username;
		std::string connectionId;
		std::chrono::system_clock::time_point exp;
	};
	class Server {
		uWS::App::WebSocketBehavior<PerSocketData> behavior;
		public:
			Server() {
				chatroom::Broadcaster broadcaster;
				behavior.upgrade = [](auto *response, auto *request, auto *context) {
					// Get token from query
						std::string token(request->getQuery("token"));
						std::print("Token: {}\n", token);

					// Verify JWT token
						chatroom::Auth auth;
						chatroom::Payload payload;
						bool verified = auth.verifyToken(token, payload);
						if (!verified) {
							response->writeStatus("401 Unauthorized")->end();
							return;
						}
						std::print("JWT verification successful for user:\nUsername: {}\nExpiration: {}\n", payload.username, payload.exp);

					// Upgrade the connection
						auth.upgradeConnection<PerSocketData>(response, request, context, payload);
				};
				behavior.open = [&broadcaster](auto *ws) {
					// Connect the user and broadcast the list of connected users
						broadcaster.connectUser(ws);
						std::print("WebSocket connection opened for user: {}\n\n", ws->getUserData()->username);
				};
			}
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