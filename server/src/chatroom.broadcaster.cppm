module;
#include <unordered_set>
#include <string>
#include <picojson/picojson.h>
#include <uwebsockets/App.h>

export module chatroom:broadcaster;

export namespace chatroom {
	class Broadcaster {
		private:
			std::unordered_set<std::string> connectedUsers;
		public:
			Broadcaster() = default;
			void connectUser( auto *ws ) {
				// Subscribe to the "chatroom" topic
					ws->subscribe("chatroom");

				// Add connected user to the list
					connectedUsers.insert(ws->getUserData()->username);

				// Broadcast the list of connected users to all clients
					sendListOfConnectedUsers(ws);
			}
			void disconnectUser( auto *ws ) {}
			void sendListOfConnectedUsers( auto *ws ) {
				// Convert the list of connected users to picojson::array
					picojson::array users;
					for (const auto &username : connectedUsers) {
						users.push_back(picojson::value(username));
					}
				
				// Create a JSON message
					picojson::object message;
					message["action"]  = picojson::value(std::string("online_users"));
					message["users"] = picojson::value(users);
					std::string jsonMessage = picojson::value(message).serialize();

				// Broadcast the message to all connected clients
					ws->publish("chatroom", jsonMessage, uWS::OpCode::TEXT);
					ws->send(jsonMessage, uWS::OpCode::TEXT);
			}

			void sendMessageHistoryToUser( auto *ws ) {}
			void sendMessageToAllUsers( auto *ws ) {}
			void sendTypingIndicator( auto *ws ) {}
	};
}