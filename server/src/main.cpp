#include <uwebsockets/App.h>
#include <print>

struct PerSocketData {};

int main() {
    uWS::App().ws<PerSocketData>("/*", {
        .open = [](auto *ws) {
            std::print("Client connected\n");
        },
        .message = [](auto *ws, std::string_view msg, uWS::OpCode) {
            std::print("Received: {}\n", msg);
            ws->send(msg, uWS::OpCode::TEXT);
        },
        .close = [](auto *ws, int code, std::string_view msg) {
            std::print("Client disconnected (code {})\n", code);
        }
    }).listen(9001, [](auto *token) {
        if (token) {
            std::print("Server listening on port 9001\n");
        } else {
            std::print("Failed to listen on port 9001\n");
        }
    }).run();

    return 0;
}