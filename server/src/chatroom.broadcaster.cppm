module;
#include <unordered_set>
#include <string>

export module chatroom:broadcaster;

export namespace chatroom {
	class Broadcaster {
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
			void sendListOfConnectedUsers( auto *ws ) {}

			void sendMessageHistoryToUser( auto *ws ) {}
			void sendMessageToAllUsers( auto *ws ) {}
			void sendTypingIndicator( auto *ws ) {}
	};
}