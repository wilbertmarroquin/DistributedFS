#include <bits/stdc++.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <unistd.h>     /* for close() */
#include "../Comportamiento/utils.h"
using namespace std;

//aca el header tiene el tamaño de a y b
//#a|#b|a + b
class protocolo_conexion
{
public:
	string ip;
    string puerto;
    const int HEADER_SIZE=4; //tamaño del header
    protocolo_conexion(string i,string p)
    {
    	cout<<"asignado parametros nueva conexion "<<endl;
        ip=i;
        puerto=p;
        cout<<ip<<" "<<puerto<<endl;
    }
    protocolo_conexion(string Buffer)
    {
    	printf("\nline a procesar conexion %s\n",Buffer.c_str());

        string Mensaje = Buffer;
        Mensaje = Mensaje.erase(0,2);
        string Header= Mensaje.substr(0,HEADER_SIZE);
        Mensaje = Mensaje.substr(HEADER_SIZE,Mensaje.size()-HEADER_SIZE);//saco al header del string

        cout<<"Header= "<<Header<<" Datos ="<<Mensaje<<endl;
        vector<string>   Header_Value;

        Header_Value.push_back(Header.substr(0,2));//tamaño a
        Header_Value.push_back(Header.substr(2,2));//tamaño b


        int inicio=0;
        int siguiente=_utils.str_to_int(Header_Value[0]);

        ip = Mensaje.substr(inicio,siguiente);

        inicio+=siguiente;
        siguiente=_utils.str_to_int(Header_Value[1]);

        puerto = Mensaje.substr(inicio,siguiente);
    }
    /*
     *
     * genero un header con los tamaños de los parametros que necesito
     * */
    string make_header()
    {
        string header="-X";
        string ip_size=to_string(ip.size());
        string puerto_size = to_string(puerto.size());

        header+=string(2-ip_size.size(),'0').append(ip_size);
        header+=string(2-puerto_size.size(),'0').append(puerto_size);
        string tam= to_string(header.size()+ip.size()+puerto.size());
        header = string(10-tam.size(),'0').append(tam) + header;
        return header;
    }
};