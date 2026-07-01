module;
#include <string>
#include <print>
#include <chrono>
#include <fstream>
#include <jwt-cpp/jwt.h>

export module chatroom:auth;

export namespace chatroom {
	struct Payload {
        std::string username;
        std::chrono::system_clock::time_point exp;
    };
	class Auth {
		private:
			std::string publicKeyPem;

			// Load public key from file: /keys/public.pem
			std::string loadPublicKey(const std::string &path) {
				std::ifstream file(path);
				if (!file.is_open()) {
					std::print("Failed to open key file: {}\n", path);
					return "";
				}
				std::stringstream buffer;
				buffer << file.rdbuf();
				std::string key = buffer.str();
				return key;
			}
		public:
			Auth() {
				publicKeyPem = loadPublicKey("keys/public.pem");
				if (publicKeyPem.empty()) {
					std::print("Public key is empty. JWT verification will fail.\n");
				}
			}
			// Verify JWT token and extract payload claim
			bool verifyToken(const std::string &token, Payload &payload ) {
				try {
					// Verify JWT token
						auto decoded  = jwt::decode<jwt::traits::kazuho_picojson>(token);
						auto verifier = jwt::verify<jwt::traits::kazuho_picojson>()
												.allow_algorithm(jwt::algorithm::rs256(publicKeyPem, "", "", ""));
						verifier.verify(decoded);

					// Extract username & exp from claim
						payload.username = decoded.get_payload_claim("username").as_string();
						payload.exp = decoded.get_expires_at();
				} catch (const std::exception &e) {
					std::print("JWT verification failed: {}\n", e.what());
					return false;
				}
				return true;
			}
			template <typename UserData>
			void upgradeConnection(auto *response, auto *request, auto *context, Payload &payload) {
				response->template upgrade<UserData>(
					{
						.username = payload.username,
						.connectionId = std::to_string(std::rand()),
						.exp = payload.exp,
					},
					request->getHeader("sec-websocket-key"),
					request->getHeader("sec-websocket-protocol"),
					request->getHeader("sec-websocket-extensions"),
					context
				);
			}
	};
}