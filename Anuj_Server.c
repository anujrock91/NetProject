#include "network.h"

struct AddressInformation
{
    int count;
    char details[1000];
    int discriptorVar;
};

void MainServerFunction(char *port)
{
    struct AddressInformation addInfo[10];
    int ServerListnerSocket;
    struct sockaddr_in ServerAddress;
    int BindVar;
    int portNumber;
    fd_set socketfd;
    int SocketDescriptor[10];
    int MaxSocketDescriptor;
    int SocketDescriptorLoop;
    int SocketActivity;
    int NewSocketDescriptor;
    struct sockaddr_in ClientAddress;
    int ClientAddressLength;
    char buffer[100];
    int AddressInformationLoop;
    int AddressInformationCount;
    char a[100];
    AddressInformationCount = 1;
    char ClientReadBuffer[2000];
    int ClientReadValue;
    char NullifyAddInfoDetails[] = " ";

    //Getting the IP of Server
    char getIpTimberlake[]="timberlake.cse.buffalo.edu";
    char *ReceiveIP;
    ReceiveIP = GetIP(getIpTimberlake);
    //Getting the port number
    portNumber = atoi(port);

    //Getting the server address
    ServerAddress.sin_family = AF_INET;
    inet_pton(AF_INET, ReceiveIP, &(ServerAddress.sin_addr));
    ServerAddress.sin_port = htons(portNumber);
    bzero(ServerAddress.sin_zero,8);

    //Creating a server socket
    ServerListnerSocket = socket(AF_INET , SOCK_STREAM , 0);
    if(ServerListnerSocket < 0)
    {
        puts("Error creating Socket...");
    }

    //Binding the Server socket
    BindVar = bind(ServerListnerSocket, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress));
    if(BindVar < 0)
    {
        puts("Error Binding...");
    }

    //Server Listener
    if((listen(ServerListnerSocket, 5)) < 0)
    {
        puts("Error in Listening Method...");
    }

    for(SocketDescriptorLoop = 2; SocketDescriptorLoop <10; SocketDescriptorLoop++)
    {
        SocketDescriptor[SocketDescriptorLoop] = 0;
    }
    SocketDescriptor[0] = stdin;
    SocketDescriptor[1] = ServerListnerSocket;

    //working with Address Information structure
    char *StringAddressGenVar;
    addInfo[0].count = 1;
    StringAddressGenVar = StringAddressGen(getIpTimberlake,ReceiveIP,port);
    strcpy(addInfo[0].details,StringAddressGenVar);
    addInfo[0].discriptorVar = ServerListnerSocket;

    for(AddressInformationLoop=1; AddressInformationLoop<10; AddressInformationLoop++)
    {
        addInfo[AddressInformationLoop].discriptorVar = 0;
        addInfo[AddressInformationLoop].count = 0;
        strcpy(addInfo[AddressInformationLoop].details,NullifyAddInfoDetails);
    }

    puts("Success1");

    //Initiating While Loop
    while(1)
    {
        //Making the SocketDescriptors in the Set Zero
        AddressInformationCount = 1;
        FD_ZERO(&socketfd);
        FD_SET(0,&socketfd);
        FD_SET(SocketDescriptor[1],&socketfd);

        if(0>SocketDescriptor[1])
        {
            MaxSocketDescriptor = 0;
        }
        else
        {
            MaxSocketDescriptor = SocketDescriptor[1];
        }

        //Adding All The established Sockets To The Set Of Sockets To Be Analyzed For Activity
        for(SocketDescriptorLoop = 2; SocketDescriptorLoop<10; SocketDescriptorLoop++)
        {
            if(SocketDescriptor[SocketDescriptorLoop] != 0)
            {
                FD_SET(SocketDescriptor[SocketDescriptorLoop],&socketfd);
                    if(SocketDescriptor[SocketDescriptorLoop]>MaxSocketDescriptor)
                    {
                        MaxSocketDescriptor = SocketDescriptor[SocketDescriptorLoop];
                    }
            }
        }

        //Checking The Activity At Every Socket In The Set Of Sockets To Be Analyzed
        SocketActivity = select( MaxSocketDescriptor + 1 , &socketfd , NULL , NULL , NULL);
        if ((SocketActivity < 0) && (errno!=EINTR))
            {
                puts("select Error...");
            }
        //Checking Which All Sockets In The Select Showed Activity
        if(FD_ISSET(0,&socketfd))
            {
                gets(buffer);
                strcpy(a,buffer);
                if(strcmp(a,"HELP") == 0)
                {
                    int i;
                    i=Help();
                }

                if(strcmp(a,"DISPLAY") == 0)
                {
                    printf("ProcessIP:  %s \n",ReceiveIP);
                    printf("ListeningPortNumber:  %d \n",portNumber);
                }

                if(strcmp(a,"LIST") == 0)
                {
                    for(AddressInformationLoop=0; AddressInformationLoop <10;AddressInformationLoop++)
                    {
                        if(addInfo[AddressInformationLoop].count != 0)
                        {
                            printf("%d \n",addInfo[AddressInformationLoop].count);
                            puts(addInfo[AddressInformationLoop].details);
                        }
                    }

                }

            }

        if(FD_ISSET(SocketDescriptor[1],&socketfd))
            {
                NewSocketDescriptor = accept(SocketDescriptor[1], (struct sockaddr *)&ClientAddress, (socklen_t*)&ClientAddressLength);
                if(NewSocketDescriptor<0)
                {
                    puts("Error In Connecting...");
                }
                else{puts("SuccessConnect");}
                for(SocketDescriptorLoop = 2; SocketDescriptorLoop<10 ; SocketDescriptorLoop++)
                {
                    if(SocketDescriptor[SocketDescriptorLoop] == 0)
                    {
                        SocketDescriptor[SocketDescriptorLoop]= NewSocketDescriptor;
                        break;
                    }
                }

                for(AddressInformationLoop=1; AddressInformationLoop<10;AddressInformationLoop++)
                {
                    if(addInfo[AddressInformationLoop].count == 0)
                    {
                        addInfo[AddressInformationLoop].discriptorVar = NewSocketDescriptor;
                        break;
                    }
                }
            }



        for(SocketDescriptorLoop = 2; SocketDescriptorLoop<10 ; SocketDescriptorLoop++)
            {
                if(SocketDescriptor[SocketDescriptorLoop] != 0)
                {
                    if(FD_ISSET(SocketDescriptor[SocketDescriptorLoop],&socketfd))
                    {
                        if ((ClientReadValue = read( SocketDescriptor[SocketDescriptorLoop] , ClientReadBuffer, 2000)) == 0)
                        {
                            for(AddressInformationLoop = 1; AddressInformationLoop<10; AddressInformationLoop++)
                            {
                                if(addInfo[AddressInformationLoop].discriptorVar == SocketDescriptor[SocketDescriptorLoop])
                                {
                                    addInfo[AddressInformationLoop].count = 0;
                                    addInfo[AddressInformationLoop].discriptorVar = 0;
                                }
                            }
                            close(SocketDescriptor[SocketDescriptorLoop]);
                            SocketDescriptor[SocketDescriptorLoop] = 0;
                            puts("Client Disconnected");
                        }
                        else if((ClientReadValue = read( SocketDescriptor[SocketDescriptorLoop] , ClientReadBuffer, 2000)) != 0)
                        {
                            for(AddressInformationLoop = 1; AddressInformationLoop<10; AddressInformationLoop++)
                            {
                                AddressInformationCount = AddressInformationCount+1;
                                if(addInfo[AddressInformationLoop].discriptorVar == SocketDescriptor[SocketDescriptorLoop])
                                {
                                    if(strcmp(addInfo[AddressInformationLoop].details,NullifyAddInfoDetails) == 0)
                                    {
                                        addInfo[AddressInformationLoop].count = AddressInformationCount;
                                        strcpy(addInfo[AddressInformationLoop].details,ClientReadBuffer);
                                        AddressInformationCount = 1;
                                        break;
                                    }
                                }
                            }

                            puts("Success");
                        }

                    }
                }
            }


    }


}
