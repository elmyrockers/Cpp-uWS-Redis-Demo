#include <string>
#include <fstream>
#include <sstream>
#include <print>

#include <uwebsockets/App.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/kazuho-picojson/defaults.h>

struct PerSocketData {
	std::string username;
	std::string connectionId;
};

std::string loadPublicKey(const std::string &path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::print("Failed to open key file: {}\n", path);
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string key = buffer.str();
	std::print("Loaded public key ({} bytes)\n", key.size());
	return key;
}

int main() {
	uWS::App::WebSocketBehavior<PerSocketData> behavior;

	behavior.upgrade = [](auto *response, auto *request, auto *context) {
		// Get token from query
			std::string token(request->getQuery("token"));
			// std::print("Token: {}\n", token);

		try {
			// Verify JWT token
				std::string publicKeyPem = loadPublicKey("keys/public.pem");
				auto decoded  = jwt::decode<jwt::traits::kazuho_picojson>(token);
				auto verifier = jwt::verify<jwt::traits::kazuho_picojson>()
										.allow_algorithm(jwt::algorithm::rs256(publicKeyPem, "", "", ""));
				verifier.verify(decoded);

			// Extract username from claim
				std::string username = decoded.get_payload_claim("username").as_string();
				// std::print("Username from claim: {}\n", username);

			// Upgrade the connection
				response->template upgrade<PerSocketData>(
					{
						.username = username,
						.connectionId = std::to_string(std::rand()),
					},
					request->getHeader("sec-websocket-key"),
					request->getHeader("sec-websocket-protocol"),
					request->getHeader("sec-websocket-extensions"),
					context
				);

		} catch (const std::exception &e) {
			std::print("JWT verification failed: {}\n", e.what());
			response->writeStatus("401 Unauthorized")->end();
		}
	};

	behavior.open = [](auto *ws) {
		std::print("Client connected: {}\nConnection ID: {}\n", ws->getUserData()->username, ws->getUserData()->connectionId);
	};

	behavior.message = [](auto *ws, std::string_view msg, uWS::OpCode) {
		std::print("Received: {}\n", msg);
		ws->send("this is a message from websocket server", uWS::OpCode::TEXT);
	};

	behavior.close = [](auto *ws, int code, std::string_view) {
		std::print("Client disconnected (code {})\n", code);
	};

	uWS::App()
		.ws<PerSocketData>("/*", std::move(behavior))
		.listen(9001, [](auto *token) {
			if (token) {
				std::print("Server listening on port 9001\n");
			} else {
				std::print("Failed to listen on port 9001\n");
			}
		})
		.run();

	return 0;
}