module;
#include <uwebsockets/App.h>

export module chatroom:server;

export namespace chatroom {
    class Server {
        public:
            Server() = default;
            void start( int port ) {}
    };
}