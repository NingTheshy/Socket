// TCP 通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    // 1.创建socket(用于监听的套接字)
    int lfd = socket(AF_INET, SOCK_STREAM, 0);

    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // inet_pton(AF_INET, "192.168.216.134", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    saddr.sin_port = htons(9999);//自定义绑定的端口
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3.监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 4.接收客户端连接
    struct sockaddr_in clientaddr;//定义客户端地址
    int len = sizeof(clientaddr);//长度
    int cfd = accept(lfd, (struct sockaddr *)&clientaddr, &len);
    
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }

    // 输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));//字节序转换为主机序
    unsigned short clientPort = ntohs(clientaddr.sin_port);//获取客户端端口
    printf("client ip is %s, port is %d\n", clientIP, clientPort);

    // 5.通信
    char recvBuf[1024] = {0};
    while(1) {
        
        // 获取客户端的数据
        int num = read(cfd, recvBuf, sizeof(recvBuf));
        if(num == -1) {
            perror("read");
            exit(-1);
        } else if(num > 0) {//获取到的数据
            printf("recv client data : %s\n", recvBuf);
        } else if(num == 0) {
            // 表示客户端断开连接
            printf("clinet closed...");
            break;
        }

        char * data = "hello,i am server";
        // 给客户端发送数据
        write(cfd, data, strlen(data));
    }
   
    // 关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}