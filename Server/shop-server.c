#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

struct client_details {
    char *item;
    int qty;
    char* IP;
    int port;
};

void timestamp(char arr[])
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    strcpy(arr, asctime(localtime(&ltime)));
}

int main(void)
{
    int listenfd = 0,connfd = 0,n=0,num=0,trans=0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in serverStorage;
    struct client_details transactions[20];
    socklen_t addr_size;

    char sendBuff[1025], recvBuff[1024];

    int numrv, mango=30, orange=30, guava=30, banana=30 ,sav;
    int mclientno=0, oclientno=0, gclientno=0, bclientno=0, transno=0;
    char mtime[30],otime[30],gtime[30],btime[30];
    char new1[50];
    int new,i,j;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1){
        printf("Failed to listen\n");
        return -1;
    }
    printf("\nAvailable items");
    printf("\nProduct\t\tQty_left\tLast Sold");
    printf("\nMango\t\t%d\t\t--", mango);
    printf("\nOrange\t\t%d\t\t--", orange);
    printf("\nGuava\t\t%d\t\t--", guava);
    printf("\nBanana\t\t%d\t\t--", banana);

    while(1)
    {
        printf("\n-----------------------------------------------------\n");
        addr_size = sizeof(serverStorage);
        connfd = accept(listenfd, (struct sockaddr*) &serverStorage , &addr_size);        /* accept awaiting request */

        /* get client details */
        struct sockaddr_in* cliIP = (struct sockaddr_in*)&serverStorage;
        struct in_addr ipAddr = cliIP->sin_addr;
        char str[INET_ADDRSTRLEN];
        char* ID = cliIP->sin_zero;
        char str2[8];
        inet_ntop(AF_INET, &ID, str2, 8);

        /* read the data send by client */
        num = recv(connfd, recvBuff, sizeof(recvBuff),0);
        if ( num <= 0 )
        {
            printf("Either Connection Closed or Error\n");

        }

        recvBuff[num] = '\0';
        for(i=1;i<strlen(recvBuff);i++)
        {
            new1[i-1]=recvBuff[i];
        }
        new1[i-1]='\0';
        new=atoi(new1);
        if(recvBuff[0]=='m')
        {
            printf("\nOrder for %d mangos recieved\n",new);
            sav=mango;
            mango=mango-new;
            if(mango<0)
            {
                mango=sav;
                printf("Transaction failed as requested quantity of Mango is not available\n");
            } else {
                transactions[trans].IP = inet_ntoa(serverStorage.sin_addr);
                transactions[trans].port = serverStorage.sin_port;
                transactions[trans].item = "Mango";
                transactions[trans].qty = new;
                timestamp(mtime);
                trans++;                                                                        /* to calculate total no. of transactions done */
                transno = mclientno++;                                          /* to send the no. of clients who have previously ordered this item */
            }
        }
        if(recvBuff[0]=='o')
        {
            printf("\nOrder for %d oranges recieved\n",new);
            sav=orange;
            orange=orange-new;
            if(orange<0)
            {
                orange=sav;
                printf("Transaction failed as requested quantity of Orange is not available\n");
            } else {
                transactions[trans].IP = inet_ntoa(serverStorage.sin_addr);
                transactions[trans].port = serverStorage.sin_port;
                transactions[trans].item = "Orange";
                transactions[trans].qty = new;
                timestamp(otime);
                trans++;                                                        /* to calculate total no. of transactions done */
                transno = oclientno++;                                          /* to send the no. of clients who have previously ordered this item */
            }

        }
        if(recvBuff[0]=='g')
        {
            printf("\nOrder for %d guavas recieved\n",new);
            sav=guava;
            guava=guava-new;
            if(guava<0)
            {
                guava=sav;
                printf("Transacation failed as requested quantity of Guava is not available\n");
            }    else {
                transactions[trans].IP = inet_ntoa(serverStorage.sin_addr);
                transactions[trans].port = serverStorage.sin_port;
                transactions[trans].item = "Guava";
                transactions[trans].qty = new;
                timestamp(gtime);
                trans++;                                                        /* to calculate total no. of transactions done */
                transno = gclientno++;                                          /* to send the no. of clients who have previously ordered this item */
            }

        }
        if(recvBuff[0]=='b')
        {
            printf("\nOrder for %d bananas recieved\n",new);
            sav=banana;
            banana=banana-new;
            if(banana<0)
            {
                banana=sav;
                printf("Transaction failed as requested quantity of Banana is not available\n");
                transno = -1;
            } else {
                transactions[trans].IP = inet_ntoa(serverStorage.sin_addr);
                transactions[trans].port = serverStorage.sin_port;
                transactions[trans].item = "Banana";
                transactions[trans].qty = new;
                timestamp(btime);
                trans++;                                                        /* to calculate total no. of transactions done */
                transno = bclientno++;                                          /* to send the no. of clients who have previously ordered this item */
            }

        }
        /* write the data for client */
        printf("\nAvailable items");
        printf("\nProduct\t\tQty_left\tLast Sold");
        printf("\nMango\t\t%d\t\t%s",mango,mtime);
        printf("\nOrange\t\t%d\t\t%s",orange,otime);
        printf("\nGuava\t\t%d\t\t%s",guava,gtime);
        printf("\nBanana\t\t%d\t\t%s",banana,btime);
        if ((write(connfd,&transno,sizeof(transno)))== -1)
        {
            fprintf(stderr, "Failure Sending Message\n");
            close(connfd);
            break;
        }
        printf("\n\n***TRANSACTION DETAILS***\n");
        printf("Item\t\tQty_Purchased\tIP Address\tPORT\n");
        for(j = 0; j < trans; j++){
            printf("%s\t\t%d\t\t%s\t%d\n",transactions[j].item,transactions[j].qty,transactions[j].IP,transactions[j].port);
        }
        close(connfd); /* Close Connection Socket */
        sleep(1);
    } /* End of Inner While... */

    return 0;
}

