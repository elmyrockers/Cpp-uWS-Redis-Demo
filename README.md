# Cpp-uWS-Redis-Demo
A blazing-fast **C++ realtime chatroom demo** built with the powerful **uWebSockets library** and **Redis** for **extreme performance and scalability**.

## Technology Stack

![C++23](https://img.shields.io/badge/C++23-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![uWebSockets](https://img.shields.io/badge/uWebSockets-000000?style=for-the-badge&logo=websocket&logoColor=white)
![Redis](https://img.shields.io/badge/Redis-DC382D?style=for-the-badge&logo=redis&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![CI/CD](https://img.shields.io/badge/CI%2FCD-A42E2B?style=for-the-badge&logo=gitlab&logoColor=white)
![GitHub Actions](https://img.shields.io/badge/GitHub%20Actions-2088FF?style=for-the-badge&logo=githubactions&logoColor=white)

## Screenshots

![Index page](/chat1.jpg)
![Chatroom page](/chat2.jpg)



***

## Security Notes

### JWT Authentication
The WebSocket connection uses JWT for authentication:<br>
`ws://localhost:9001?token=<jwt>`

> **Demo only** — passing JWT as a query parameter is intentional for local development and demo purposes only.

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