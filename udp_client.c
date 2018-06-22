#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
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
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));  //把主机序列的字符串端口号转换成整型  然后转换为二子皆网络端口号 因为网络端口号是两个字节所以用short
    server.sin_addr.s_addr = inet_addr(argv[1]);  //把点分十式十进制字符串IP地址转换为网络字节序的四字节整型IP地址
    
    //客户端的端口号不应该绑定，这样不合适！
    /*if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)    //把用户栈上的local绑定到操作系统内部
    {
        perror("bind");
        return 3;
    }*/

    char buf[1024];
    while(1)  //先读数据，再收数据
    {
        printf("Please Enter:");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf)-1);  //从标准输入读
        if(s > 0)
        {
            buf[s-1] = 0;   //处理\n                                                               
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));//把buf中的内容发送到服务端
            //发送完后再接受服务器发回来的消息
            ssize_t _s = recvfrom(sock, buf, sizeof(buf)-1,0,NULL,NULL);//因为我们的客户端收到的数据一定从服务器发过来，不接受发送者也知道是谁发来的
            if(_s > 0)
            {
                buf[_s] = 0;                                                                  
                printf("server echo: %s\n",buf);
            }
        }
    }

}
