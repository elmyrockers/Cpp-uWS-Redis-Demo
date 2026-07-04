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

## Security Notes

### JWT Authentication
> **Demo only** — passing JWT as a query parameter is intentional for local development demonstration purposes only.

The WebSocket connection uses JWT for authentication:<br>
`ws://localhost:9001?token=<jwt>`

This approach is **not recommended for production** as the token is exposed in server logs, proxy logs, and browser DevTools network tab.

### Production Approach
In production, this would be implemented using:
>- **HttpOnly cookies** set by the Golang server after login
>- **Nginx reverse proxy** to serve both Golang and C++ servers under the same domain and port
>- Browser automatically sends the cookie in the WebSocket handshake headers — no token in the URL, no JS exposure