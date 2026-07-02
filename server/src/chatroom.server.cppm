module;
#include <string>
#include <print>
#include <chrono>
#include <picojson/picojson.h>
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
		uWS::App app;
		uWS::App::WebSocketBehavior<PerSocketData> behavior;
		chatroom::Auth auth;
		chatroom::Broadcaster broadcaster;
		public:
			Server() {
				broadcaster.setApp(&app);
				behavior.upgrade = [this](auto *response, auto *request, auto *context) {
					// Get token from query
						std::string token(request->getQuery("token"));
						std::print("Token: {}\n", token);

					// Verify JWT token
						chatroom::Payload payload;
						bool verified = auth.verifyToken(token, payload);
						if (!verified) {
							response->writeStatus("401 Unauthorized")->end();
							return;
						}
						std::print("JWT verification successful for user:\nUsername: {}\nExpiration: {}\n", payload.username, payload.exp);

					// Upgrade the connection
						std::print("Upgrading connection for user: {}\n", payload.username);
						auth.upgradeConnection<PerSocketData>(response, request, context, payload);
				};
				behavior.open = [this](auto *ws) {
					// Connect the user and broadcast the list of connected users
						broadcaster.connectUser(ws);
						std::print("WebSocket connection opened for user: {}\n\n", ws->getUserData()->username);
				};
				behavior.message = [this](auto *ws, std::string_view msg, uWS::OpCode) {
					std::print("Received message from user {}: {}\n", ws->getUserData()->username, msg);
					// Parse JSON message
						std::string err;
						picojson::value parsedMessage;
						err = picojson::parse(parsedMessage, std::string(msg));
						if (!err.empty() || !parsedMessage.is<picojson::object>()) {
							std::print("Invalid JSON: {}\n", err);
							return;
						}
					// Get the action from the message
						picojson::object obj = parsedMessage.get<picojson::object>();
						std::string action = obj["action"].get<std::string>();
						std::print("Action: {}\n", action);

					// Broadcast based on the action
						// Typing indicator actions
							if(action == "start_typing") {
								broadcaster.startTypingIndicator(ws);
							} else if (action == "stop_typing") {
								broadcaster.stopTypingIndicator(ws);
							}
							
				};
				behavior.close = [this](auto *ws, int code, std::string_view message) {
					// Disconnect the user
						broadcaster.disconnectUser(ws);
						std::print("WebSocket connection closed for user: {}\nCode: {}\nMessage: {}\n\n", ws->getUserData()->username, code, message);
				};
			}
			void start( int port ) {
				// Run the websocket server
					app.ws<PerSocketData>("/*", std::move(behavior))
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