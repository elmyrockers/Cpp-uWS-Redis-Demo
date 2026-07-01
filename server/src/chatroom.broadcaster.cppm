module;
#include <string>

export module chatroom:broadcaster;

export namespace chatroom {
	class Broadcaster {
		public:
			Broadcaster() = default;
			void connectUser( auto *ws ) {
				// Subscribe to the "chatroom" topic
					ws->subscribe("chatroom");
			}
			void disconnectUser( auto *ws ) {}
			void sendListOfConnectedUsers( auto *ws ) {}

			void sendMessageHistoryToUser( auto *ws ) {}
			void sendMessageToAllUsers( auto *ws ) {}
			void sendTypingIndicator( auto *ws ) {}
	};
}