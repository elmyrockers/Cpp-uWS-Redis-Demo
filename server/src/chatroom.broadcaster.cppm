module;
#include <unordered_set>
#include <string>
#include <picojson/picojson.h>
#include <uwebsockets/App.h>

export module chatroom:broadcaster;

export namespace chatroom {
	class Broadcaster {
		private:
			uWS::App *app = nullptr;
			std::unordered_set<std::string> connectedUsers;
			std::string getUsersJson() {
				picojson::array users;
				for (const std::string &username : connectedUsers) {
					users.push_back(picojson::value(username));
				}
				picojson::object message;
				message["action"]  = picojson::value(std::string("online_users"));
				message["users"] = picojson::value(users);
				std::string jsonMessage = picojson::value(message).serialize();
				return jsonMessage;
			}
		public:
			Broadcaster() = default;
			void setApp(uWS::App *app) {
				this->app = app;
			}
			void connectUser( auto *ws ) {
				// Subscribe to the "chatroom" topic
					ws->subscribe("chatroom");

				// Add connected user to the list
					connectedUsers.insert(ws->getUserData()->username);

				// Broadcast the list of connected users to all clients
					std::string usersJson = getUsersJson();
					ws->publish("chatroom", usersJson, uWS::OpCode::TEXT);
					ws->send(usersJson, uWS::OpCode::TEXT);
			}
			void disconnectUser( auto *ws ) {
				// Remove disconnected user from the list
					connectedUsers.erase(ws->getUserData()->username);

				// Broadcast the updated list of connected users to all clients
					app->publish("chatroom", getUsersJson(), uWS::OpCode::TEXT);
			}

			void sendMessageHistoryToUser( auto *ws ) {}
			void sendMessageToAllUsers( auto *ws ) {}
			void startTypingIndicator( auto *ws ) {}
			void stopTypingIndicator( auto *ws ) {}
	};
}