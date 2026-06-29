module;
#include <string>
#include <unordered_set>
#include <picojson/picojson.h>

export module chatroom;

export class Chatroom {
    std::unordered_set<std::string> connectedUsers;

public:
    void addUser(const std::string &username) {
        connectedUsers.insert(username);
    }

    void removeUser(const std::string &username) {
        auto it = connectedUsers.find(username);
        if (it != connectedUsers.end()) {
            connectedUsers.erase(it);
        }
    }

    std::string getUsersJson() const {
        picojson::array arr;
        for (const auto &username : connectedUsers) {
            arr.push_back(picojson::value(username));
        }
        return picojson::value(arr).serialize();
    }
};