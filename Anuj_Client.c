#include "network.h"

void MainClientFunction(char *port)
{

        int ClientSocketSender;
        int ClientSocketRec;
        int ClientSocketServer;
        struct sockaddr_in ClientListner;
        struct sockaddr_in RemoteServerAddress;
        struct sockaddr_in PeerAddress;
        int PeerAddressLen;
        int ClientConnect;
        char ClientName[100];
        int ClientPortNumber;
        int ClientAddressLength;
        int BindVarClient;
        int ClientSocketDescriptor[5];
        int MaxDescriptor;
        int ClientSocketDescriptorLoop;
        int ClientSocketRecAct;
        char ClientBufferStdin[150];
        fd_set socketfd;
        int NewSocketDes;

        puts("Enter The System Name");
        puts("System name should be like xyz.cse.buffalo.edu");
        gets(ClientName);

        //Getting the IP from name START
        char ClientIP[100];
        char *RecIP;
        RecIP = GetIP(ClientName);
        strcpy(ClientIP,RecIP);

        //Getting the port number
        ClientPortNumber = atoi(port);

        //Generating the Full Address
        char *a;
        char StringAddress[100];
        a = StringAddressGen(ClientName,RecIP,port);
        strcpy(StringAddress,a);

        //Creating Client Sockets
        ClientSocketSender = socket(AF_INET , SOCK_STREAM , 0);
        if(ClientSocketSender < 0)
        {
            puts("Error Creating Client SENDER Socket");
        }

        ClientSocketServer = socket(AF_INET , SOCK_STREAM , 0);
        if(ClientSocketServer < 0)
        {
             puts("Error Creating Client SERVER Socket");
        }
        //Creating Client Listener Socket
        ClientListner.sin_family = AF_INET;
        inet_pton(AF_INET, ClientIP, &(ClientListner.sin_addr));
        ClientListner.sin_port = htons(ClientPortNumber);
        bzero(ClientListner.sin_zero,8);
        ClientAddressLength = sizeof(ClientListner);

        ClientSocketRec = socket(AF_INET , SOCK_STREAM , 0);
        if(ClientSocketRec < 0)
        {
        puts("Error creating Client LISTENER Socket...");
        }
        BindVarClient = bind(ClientSocketRec, (struct sockaddr *)&ClientListner, sizeof(ClientListner));
        if(BindVarClient < 0)
        {
        puts("Error Binding Client listener...");
        }
        if((listen(ClientSocketRec, 3)) < 0)
        {
        puts("Error in Listening Method...");
        }
        puts("Success");

        ClientSocketDescriptor[0] = stdin;
        ClientSocketDescriptor[1] = ClientSocketRec;
        for(ClientSocketDescriptorLoop = 2; ClientSocketDescriptorLoop<5;ClientSocketDescriptorLoop++)
        {
            ClientSocketDescriptor[ClientSocketDescriptorLoop] = 0;
        }
        puts("Success");
        //Running the infinite Loop
        while(1)
        {
            //Setting the sockets in select API
            FD_ZERO(&socketfd);
            FD_SET(0,&socketfd);
            FD_SET(ClientSocketDescriptor[1],&socketfd);
            if(0>ClientSocketDescriptor[1])
            {
            MaxDescriptor = 0;
            }
            else
            {
            MaxDescriptor = ClientSocketDescriptor[1];
            }

            //Setting up all the other sockets in select API which are non zero
            for(ClientSocketDescriptorLoop = 2; ClientSocketDescriptorLoop<5;ClientSocketDescriptorLoop++)
            {
                if(ClientSocketDescriptor[ClientSocketDescriptorLoop] !=0)
                {
                    FD_SET(ClientSocketDescriptor[ClientSocketDescriptorLoop],&socketfd);
                    if(ClientSocketDescriptor[ClientSocketDescriptorLoop]>MaxDescriptor)
                    {
                        MaxDescriptor = ClientSocketDescriptor[ClientSocketDescriptorLoop];
                    }
                }
            }

            //Checking The Activity At Every Socket In The Set Of Sockets To Be Analyzed
            ClientSocketRecAct = select( MaxDescriptor + 1 , &socketfd , NULL , NULL , NULL);
            if ((ClientSocketRecAct < 0) && (errno!=EINTR))
            {
                puts("select Error...");
            }

            //Checking the standard input output Socket
            if(FD_ISSET(0,&socketfd))
            {
                gets(ClientBufferStdin);
                char a[150];
                char *RegisterPointer;
                char *PortPointer;
                char SplitStringPort[100];
                char SplitString1[100];
                strcpy(a,ClientBufferStdin);
                if(strcmp(a,"HELP") == 0)
                {
                    int i;
                    i = Help();
                }
                if(strcmp(a,"DISPLAY") == 0)
                {
                    printf("ProcessIP: %s \n", RecIP);
                    printf("Listening port: %d \n", ClientPortNumber);
                }
                RegisterPointer = strSeperate(a,1);
                strcpy(SplitString1,RegisterPointer);
                puts(a);
                if(strcmp(SplitString1,"REGISTER") == 0)
                {
                    puts("Success");
                    PortPointer = strSeperate(a,3);
                    strcpy(SplitStringPort,PortPointer);
                    //creating server address
                    /*char split1[100];
                    char *c;
                    c = strSeperate(a,3);
                    strcpy(split1,c);
                    char Timbername[] = "timberlake.cse.buffalo.edu";
                    char *TimberIP;
                    TimberIP = GetIP(Timbername);
                    puts(split1);
                    RemoteServerAddress.sin_family = AF_INET;
                    inet_pton(AF_INET, TimberIP, &(RemoteServerAddress.sin_addr));
                    RemoteServerAddress.sin_port = htons(atoi(split1));
                    bzero(RemoteServerAddress.sin_zero,8);
                    //Connecting Server
                    ClientConnect = connect(ClientSocketServer, (struct sockaddr *)&RemoteServerAddress, sizeof(RemoteServerAddress));
                    if(ClientConnect < 0)
                    {
                        puts("Error Connecting Server...");
                    }
                    else
                    {
                        puts("Success");
                    }*/

                }
            }

            //Checking the listener Socket
            if(FD_ISSET(ClientSocketDescriptor[1],&socketfd))
            {
                NewSocketDes = accept(ClientSocketDescriptor[1],(struct sockaddr *)&PeerAddress,(socklen_t*)&PeerAddressLen);
                if(NewSocketDes<0)
                {
                    puts("Error in connecting..Peer Busy cannot handle more than three connections");
                }
                for(ClientSocketDescriptorLoop = 2; ClientSocketDescriptorLoop < 5; ClientSocketDescriptorLoop++)
                {
                    if(ClientSocketDescriptor[ClientSocketDescriptorLoop] == 0)
                    {
                        ClientSocketDescriptor[ClientSocketDescriptorLoop] = NewSocketDes;
                        break;
                    }
                }
            }

            //Checking For the Remaining Sockets
            for(ClientSocketDescriptorLoop = 2; ClientSocketDescriptorLoop<5;ClientSocketDescriptorLoop++)
            {
                if(ClientSocketDescriptor[ClientSocketDescriptorLoop] != 0)
                {
                    if(FD_ISSET(ClientSocketDescriptor[ClientSocketDescriptorLoop],&socketfd))
                    {

                    }
                }
            }


        }

        /*
        //Declaring the RemoteServer address to bind and Constructing a server Address to bind
        RemoteServerAddress.sin_family = AF_INET;
        inet_pton(AF_INET, ipstring, &(RemoteServerAddress.sin_addr));
        RemoteServerAddress.sin_port = htons(4545);
        bzero(RemoteServerAddress.sin_zero,8);
        RemoteServerAddressLength = sizeof(RemoteServerAddress);



        //Creating Client Sender Socket
        ClientSocketSender = socket(AF_INET , SOCK_STREAM , 0);
        if(ClientSocketSender == 0)
        {
            puts("Error Creating Client Sender Socket...");
        }
        //Creating Client Server Socket
        ClientSocketServer = socket(AF_INET , SOCK_STREAM , 0);
        if(ClientSocketServer == 0)
        {
            puts("Error Creating Client Server Socket...");
        }
        //Creating Client Receiver Socket
        ClientSocketRec = Socket(AF_INET, SOCK_STREAM, 0);
        if(ClientSocketRec == 0)
        {
            puts("Error Creating Client Receiver socket");
        }

        //Client Connecting To Server
        ClientConnect = connect(ClientSocketSender, (struct sockaddr *)&RemoteServerAddress, sizeof(RemoteServerAddress));
        if(ClientConnect < 0)
        {
            puts("Error Connecting Server...");
        }
        else
        {
            puts("Connection Successful");
        }*/

}
