export module chatroom:auth;

export namespace chatroom {
	class Auth {
		private:
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
			Auth() = default;
	};
}