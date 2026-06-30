import chatroom;

int main(){
    chatroom::Server server;
    server.start( 9001 );
    return 0;
}