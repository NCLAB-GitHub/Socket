#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];


    //於Kernel建立Socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    // AF_INET = IPv4 協定
    // SOCK_STREAM = TCP模式
    // 0 = default(auto)

    if (socket_desc == -1)
        printf("Could not create socket\n");

    printf("Socket created\n");

    //設定Socket server細節
    server.sin_family = AF_INET;  //IPv4 協定
    server.sin_addr.s_addr = INADDR_ANY;  //交給Kernel決定Server IP
    server.sin_port = htons( 8888 );  //port

    //進行Socket server配置
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    printf("bind done\n");

    //Listen:決定多少Client可連入此Socket server
    listen(socket_desc , 3);

    //等待Client登入
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

    //同意該Client端登入
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

    if (client_sock < 0){
        perror("accept failed\n");
        return 1;
    }

    printf("Connection accepted\n");

    //讀取Client的Message；buf=2000；0=default
    if( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
        //將Client傳輸的message回傳給Client端
        write(client_sock , client_message , strlen(client_message));

    if(read_size == 0)
        printf("Client is disconnected?\n");

    else if(read_size == -1)
        printf("recv failed");

    return 0;
}
