#include <string>

export module chatroom:broadcaster;

export namespace chatroom {
	class Broadcaster {
		public:
			Broadcaster() = default;
			void sendMessageToAllUsers(const std::string &message) {}
	};
}