module;
#include <unordered_set>
#include <string>
#include <print>
#include <chrono>
#include <picojson/picojson.h>
#include <uwebsockets/App.h>
#include <sw/redis++/redis++.h>


export module chatroom:broadcaster;

export namespace chatroom {
	class Broadcaster {
		private:
			uWS::App *app = nullptr;
			sw::redis::Redis redis;
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
			Broadcaster() : redis("tcp://localhost:6379") {

			}
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
			void sendMessageToAllUsers( auto *ws, const std::string &messageContent ) {
				// Get the current timestamp
					auto now = std::chrono::system_clock::now().time_since_epoch();
					auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now).count();
				
				// Store the message into redis
					std::vector<std::pair<std::string, std::string>> attrs = {
						{"connection_id", ws->getUserData()->connectionId},
						{"username",      ws->getUserData()->username},
						{"content",       messageContent},
						{"timestamp",     std::to_string(timestamp)}
					};
					redis.xadd("chatroom:messages", "*", attrs.begin(), attrs.end(), 1000);

				// Build the message JSON
					picojson::object message;
					message["action"] = picojson::value(std::string("message"));
					message["connection_id"] = picojson::value(ws->getUserData()->connectionId);
					message["username"] = picojson::value(ws->getUserData()->username);
					message["content"] = picojson::value(messageContent);
					message["timestamp"] = picojson::value((double)timestamp);

					std::string jsonMessage = picojson::value(message).serialize();

				// Broadcast the message to all clients
					ws->publish("chatroom", jsonMessage, uWS::OpCode::TEXT);
					ws->send(jsonMessage, uWS::OpCode::TEXT);

				// Mark the user as not typing - because it's done sending the message
					ws->getUserData()->isTyping = false;
			}
			void startTypingIndicator( auto *ws ) {
				// Mark the user as typing
					ws->getUserData()->isTyping = true;

				// Broadcast start typing indicator to all clients except the sender
					picojson::object message;
					message["action"] = picojson::value(std::string("start_typing"));
					message["username"] = picojson::value(ws->getUserData()->username);
					std::string jsonMessage = picojson::value(message).serialize();

					ws->publish("chatroom", jsonMessage, uWS::OpCode::TEXT);
			}
			void stopTypingIndicator( auto *ws ) {
				// Mark the user as not typing
					ws->getUserData()->isTyping = false;

				// Broadcast stop typing indicator to all clients except the sender
					picojson::object message;
					message["action"] = picojson::value(std::string("stop_typing"));
					message["username"] = picojson::value(ws->getUserData()->username);
					std::string jsonMessage = picojson::value(message).serialize();

					ws->publish("chatroom", jsonMessage, uWS::OpCode::TEXT);
			}
			void stopTypingIndicatorOnClose( auto *ws ) {
				// Allow broadcasting only if the user is currently typing
					if (!ws->getUserData()->isTyping) return;

				// Mark the user as not typing
					ws->getUserData()->isTyping = false;

				// Broadcast stop typing indicator to all clients
					picojson::object message;
					message["action"] = picojson::value(std::string("stop_typing"));
					message["username"] = picojson::value(ws->getUserData()->username);
					std::string jsonMessage = picojson::value(message).serialize();

					app->publish("chatroom", jsonMessage, uWS::OpCode::TEXT);
			}
	};
}