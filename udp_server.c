#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
//192.168.X.X 8080

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage %s [ip] [port]\n",argv[0]);
        return 1;
    }

    int sock = socket(AF_INET,SOCK_DGRAM,0);//协议IPV4 udp
    if(sock < 0)
    {
        perror("socket");
        return 2;
    }

    printf("sock: %d\n",sock);

    //填充本地信息
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));  //把主机序列的字符串端口号转换成整型  然后转换为二子皆网络端口号 因为网络端口号是两个字节所以用short
    local.sin_addr.s_addr = inet_addr(argv[1]);  //把点分十式十进制字符串IP地址转换为网络字节序的四字节整型IP地址
    
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)    //把用户栈上的local绑定到操作系统内部
    {
        perror("bind");
        return 3;
    }

    char buf[1024];
    while(1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        //收到的东西放到buf里
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1,0,(struct sockaddr*)&client,&len);//通过client带回的信息知道是谁连接了我，我后面要发送给谁
        if(s > 0)
        {
            buf[s] = 0;                                                                  
            //打印出客户端的IP地址和网络端口号，并用这两个函数将其转换成我们想看到的样子，并打印出client发过来的内容
            printf("[%s:%d]: %s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client,sizeof(client));//把buf中的内容发回客户端
        }
    }

}
