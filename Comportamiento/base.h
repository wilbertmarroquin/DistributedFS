#include <bits/stdc++.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <unistd.h>     /* for close() */
#include <thread>
#include <ifaddrs.h>
#include </usr/include/postgresql/libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/poll.h>
#include "utils.h"
#include "Esclavo_Info.h"
PGconn *conn;
PGresult *res;
using namespace std;

#define MAXPENDING 12    /* Maximum outstanding connection requests */
//const unsigned long BUFSIZE =999999;   /* tamaño del mensaje */

class Base
{
public:
	utils _utils;
    char* echoBuffer;
    string IP_Master="192.168.1.12";
    int SocketFD;
    int port;
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    int querySock;
    vector<thread> T_Vecinos;
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    vector<Esclavo_Info*> Tabla_Esclavos;
    thread th_keep_alive;
    int Numero_Esclavos;
    int Esclavos_Actuales;
    unsigned int clntLen;            /* Length of client address data structure */
    //virtual void Enviar_Mensaje()=0;
    Base(int p)
    {
        port = p;
        Numero_Esclavos=3;
        Esclavos_Actuales=0;
        echoBuffer=new char [10];
        Generar_Tabla();
    }
    Base() {}
    void Ver_Tabla_Esclavos()
    {
        for(int i=0;i<Esclavos_Actuales;i++)
        {
            cout<<"Puerto: "<<Tabla_Esclavos[i]->puerto<<endl;
            cout<<"Socket: "<<Tabla_Esclavos[i]->Socket_Esclavo<<endl;
            cout<<"Activo: "<<Tabla_Esclavos[i]->Activo<<endl;
        }
    }
    void Generar_Tabla()
    {
    	for(int i=0;i<Numero_Esclavos;i++)
    	{
    		Esclavo_Info* Nuevo=new Esclavo_Info("127.0.0.1",to_string(10000+i));
    		Tabla_Esclavos.push_back(Nuevo);
    	}
    }
	/*
	 *
	 * metodo que inicializa parametros para escuchar conexiones entrantes
	 * */
    void iniciar_socket()
    {
        if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)

        {
            cout<<"socket() init fallo en class base"<<endl;
            exit(1);
        }


        /* Construct local address structure */
        memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
        echoServAddr.sin_family = AF_INET;                /* Internet address family */
        echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
        echoServAddr.sin_port = htons(port);      /* Local port */
        cout<<"Mi puerto: "<<port<<endl;

        /* Bind to the local address */
        if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)

        {
            cout<<"bind() failed en class base "<<endl;
            exit(1);
        }
        /* Mark the socket so it will escuchar for incoming connections */
        if (listen(servSock, MAXPENDING) < 0)
        {
            cout<<"escuchar() failer en clase base"<<endl;
            exit(1);
        }
    }
	/*
	 *
	 * metodo para conectararse a otro socket
	 * p: puerto de conexion
	 *
	 * */
    void  _conectar(string p,string ip)
    {
        cout<<"conectarando a puerto "<<p<<endl;
        struct sockaddr_in stSockAddr;
        SocketFD = socket(AF_INET, SOCK_STREAM, 0); //IPPROTO_TCP

        if (-1 == SocketFD)
        {
            cout<<"error escuchar_  clase base : cannot create socket"<<endl;
            exit(1);
        }

        memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

        stSockAddr.sin_family = AF_INET;
        stSockAddr.sin_port = htons(_utils.str_to_int(p) );

        int Res = inet_pton(AF_INET, ip.c_str(), &stSockAddr.sin_addr);

        if (0 > Res)
        {
            cout<<"error: first parameter is not a valid address family"<<endl;
            close(SocketFD);
            exit(1);
        }

        else if (0 == Res)
        {
            cout<<"char string (second parameter does not contain valid ipaddress"<<endl;
            close(SocketFD);
            exit(1);
        }

        if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
        {
            perror("connect failed");
            close(SocketFD);
            exit(1);
        }
    }
    void  _conectar_Esclavo(string p,string ip,int & SocketFD)
    {


        cout<<"conectarando a puerto "<<p<<endl;
        struct sockaddr_in stSockAddr;
        SocketFD = socket(AF_INET, SOCK_STREAM, 0); //IPPROTO_TCP

        if (-1 == SocketFD)
        {
            cout<<"error escuchar_  clase base : cannot create socket"<<endl;
            exit(1);
        }

        memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

        stSockAddr.sin_family = AF_INET;
        stSockAddr.sin_port = htons(_utils.str_to_int(p) );

        int Res = inet_pton(AF_INET, ip.c_str(), &stSockAddr.sin_addr);

        if (0 > Res)
        {
            cout<<"error: first parameter is not a valid address family"<<endl;
            close(SocketFD);
            exit(1);
        }

        else if (0 == Res)
        {
            cout<<"char string (second parameter does not contain valid ipaddress"<<endl;
            close(SocketFD);
            exit(1);
        }

        if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
        {
            perror("connect failed");
            close(SocketFD);
            exit(1);
        }
    }
	/*
    *
    *
    * Funcion para saber que sockets están conenctados
    * */
    void Keep_Alive()
    {
        cout<<endl<<"Iniciando Keep_Alive"<<endl;
        unsigned int intervalo_tiempo = 100000;
        //int aux = 0;
        while (1)
        {
            for (int i = 0;i < (int)Tabla_Esclavos.size();i++)
            {
                //cout<<endl<<"Verificando conexiones"<<endl;
                //if (Tabla_Esclavos[i]->Activo != 1) continue;
                struct pollfd fd;
                int ret;

                fd.fd = Tabla_Esclavos[i]->Socket_Esclavo; // your socket handler
                fd.events = POLLIN;
                ret = poll(&fd, 1, 500); // 1 second for timeout
                //Ver_Tabla_Esclavos();
                //cout<<"RET: "<<ret<<endl;
                switch (ret) {
                     case -1:
                     Tabla_Esclavos[i]->Activo = 0;
                    //     cout<<"El socket con ip "<<Tabla_Esclavos[i]->puerto<<" no esta conectado"<<endl;
                     break;
                    case 0:
                    //cout<<"El socket con ip "<<Tabla_Esclavos[i]->puerto<<"  esta conectado "<<endl;
                    break;
                    case 1:
                    Tabla_Esclavos[i]->Activo = 0;
                    //cout<<"El socket con ip "<<Tabla_Esclavos[i]->puerto<<"  no esta conectado"<<endl;
                    break;
                }
            }
            usleep(intervalo_tiempo);
        }
    }
    /*
	 *
	 * este metodo tiene que ser sobreescrito
	 * cuando se herede a otra clase
	 * */
	virtual void _escuchar(){}
	virtual void _iniciar(){}
    void conexiones()
    {
    	iniciar_socket();
    	_iniciar();
        cout<<endl;
        for (int i=0;i<(int)Tabla_Esclavos.size();i++)
            cout<<Tabla_Esclavos[i]->ip<<" "<<Tabla_Esclavos[i]->puerto<<" "
        <<Tabla_Esclavos[i]->Socket_Esclavo<<" "<<Tabla_Esclavos[i]->Activo<<endl;
        //th_keep_alive = thread(&this->Keep_Alive);
    	//_escuchar();
        //th_keep_alive.join();
    }
    void conexiones2()
    {
        _escuchar();
    }

	/*
	 *
	 * aceptar conexiones de otros sockets
	 * */
    void permitir_conexion()
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);
        cout<<"esperando esclavos"<<endl;
        /* Wait for a client to connect */
        if (( Tabla_Esclavos[Esclavos_Actuales]->Socket_Esclavo = accept(servSock, (struct sockaddr *) &echoClntAddr,&clntLen)) < 0)
        {
            cout<<"accept() failed en clase base.h metodo accept conectarion"<<endl;;
            exit(1);
        }

    }
    /*
	 *
	 * aceptar conexiones de otros sockets
	 * */
    void permitir_conexion_cliente()
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);
        cout<<"esperando clientes"<<endl;
        /* Wait for a client to connect */
        if (( clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,&clntLen)) < 0)
        {
            cout<<"accept() failed en clase base.h metodo accept conectarion"<<endl;;
            exit(1);
        }
    }
    void permitir_conexion_query()
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);
        cout<<"esperando query"<<endl;
        /* Wait for a client to connect */
        if (( querySock = accept(servSock, (struct sockaddr *) &echoClntAddr,&clntLen)) < 0)
        {
            cout<<"accept() failed en clase base.h metodo accept conectarion"<<endl;;
            exit(1);
        }
    }
	/*
	 * metodo para hacer envios de mensajes
	 * */
    int Enviar_Mensaje(int clntSocket,string * stringBuffer)
    {
        // char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
        int len_send=0;
        printf("enviare: ");
        cout<<"Sending:<"<<*stringBuffer<<">"<<endl;
        //printf("Sending:<%s>\n",stringBuffer);
        /* Echo message back to client */
        if((len_send=send(clntSocket, &((*stringBuffer)[0]), stringBuffer->size(), MSG_NOSIGNAL)!=stringBuffer->size()))
        {
            printf("no se pudo mandar error en clase base HandleTpcClient_send:(\n");
            return -1;
        }
        printf("fin envio \n");
        return 1;

    }
	/*
	 *
	 * metodo para recepcion de mensajes
	 * */
    void Recibir_Mensaje(int SocketFD,string* stringBuffer)
    {
        //fill_n(echoBuffer, BUFSIZE, 0);
    	char size[10];
        int recvMsgSize = recv(SocketFD, size, 10, 0);
        if ((recvMsgSize) <= 0)
        {
            printf("error en base.class metodo Recibir_Mensaje \n");
            //return -2;
        }
        string tam(size);
        int tam_mensaje=_utils.str_to_int(tam);
        cout<<"Tama: "<<tam_mensaje<<endl;
        stringBuffer->resize(tam_mensaje+1);
        recvMsgSize = read(SocketFD, &((*stringBuffer)[0]), tam_mensaje);
        //recvMsgSize = recv(SocketFD, (void*)stringBuffer, tam_mensaje, 0);
        if ((recvMsgSize) <= 0)
        {
            printf("error en base.class metodo Recibir_Mensaje \n");
            //return -2;
        }
    }
	/*
	 *
	 * metodo para cerrar conexion
	 * */
    void Cerrar_Conexion(int & SocketFD)
    {
        close(SocketFD);
        cout<<"conexion con "<<SocketFD<<" cerrada "<<endl;
    }
};

