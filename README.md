# Cpp-uWS-Redis-Demo
A blazing-fast **C++ realtime chat demo** built with the powerful **uWebSockets library** and **Redis** for **extreme performance and scalability**.

## Technology Stack

![C++23](https://img.shields.io/badge/C++23-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![uWebSockets](https://img.shields.io/badge/uWebSockets-000000?style=for-the-badge&logo=websocket&logoColor=white)
![Redis](https://img.shields.io/badge/Redis-DC382D?style=for-the-badge&logo=redis&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![CI/CD](https://img.shields.io/badge/CI%2FCD-A42E2B?style=for-the-badge&logo=gitlab&logoColor=white)
![GitHub Actions](https://img.shields.io/badge/GitHub%20Actions-2088FF?style=for-the-badge&logo=githubactions&logoColor=white)

## Features

#### WebSocket Server (C++ µWebSockets)
>- **Real-time messaging** - broadcast messages to all connected clients instantly
>- **JWT authentication** - every connection is verified via RS256 JWT token on upgrade
>- **Message history** - last 200 messages fetched from Redis Stream on connect
>- **Online users** - broadcasts connected users list on join/leave
>- **Typing indicators** - real-time start/stop typing broadcast to all clients
>- **Redis Stream storage** - messages stored persistently with auto-trim (last 1000 entries)
>
#### HTTP Server (Golang Fiber)
>- **Web frontend** - serves HTML/JS chat interface
>- **JWT generation** - signs JWT token with RS256 private key on login
>- **Static assets** - serves JS and images
>
#### Browser (WebSocket Client)
>- **Real-time chat UI** - connects directly to C++ WebSocket server
>- **JWT token** - obtained from Golang http server on login, passed to WebSocket connection
>- **Message rendering** - displays chat messages, typing indicators and online users in real-time
>
#### Infrastructure
>- **Docker** - both services containerized and ready to run
>- **Docker Compose** - single command to start all services
>- **Redis** - in-memory message store and stream
>- **RSA key pair** - shared between client and server for JWT signing and verification



## Screenshots

![Index page](/chat1.jpg)
![Chatroom page](/chat2.jpg)





## Security Notes

### JWT Authentication
The WebSocket connection within this demo project uses JWT for authentication:<br>
`ws://localhost:9001?token=<jwt>`

> **Note:** Passing JWT as a query parameter is ***intentional for local development and demo purposes only***.

This approach is **not recommended for production** as the token is exposed in server logs, proxy logs, and browser DevTools network tab,
which will increase security risk.

### Production Approach
In production, authentication would flow as follows:

>1. User logs in - Golang http server verifies credentials
>2. Golang http server sets **HttpOnly cookie** containing the JWT
>3. Browser stores cookie — JavaScript cannot read it
>4. Browser opens WebSocket connection to C++ websocket server
>5. Browser automatically sends cookie in WS handshake headers
>6. C++ websocket server reads and verifies JWT from cookie header directly

The token is passed **server-to-server via HttpOnly cookie** — never exposed to JavaScript, never visible in URLs or logs.