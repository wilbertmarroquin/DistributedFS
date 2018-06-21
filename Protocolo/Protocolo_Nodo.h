#include <bits/stdc++.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <unistd.h>     /* for close() */
#include "../Comportamiento/utils.h"
using namespace std;

//aca el header tiene el tamaño de a y b
//#a|#b|a + b
class protocolo_new_nodo
{
public:
	string dato;
    string frecuencia;
    const int HEADER_SIZE=3; //tamaño del header
    protocolo_new_nodo(string d,string f)
    {
    	cout<<"asignado parametros nuevo nodo "<<endl;
        dato=d;
        cout<<dato<<endl;
    }
    protocolo_new_nodo(string Buffer)
    {
    	printf("\nline a procesar %s\n",Buffer.c_str());

        string Mensaje = Buffer;
        Mensaje = Mensaje.erase(0,2);
        string Header= Mensaje.substr(0,HEADER_SIZE);
        Mensaje = Mensaje.substr(HEADER_SIZE,Mensaje.size()-HEADER_SIZE);//saco al header del string
        cout<<"Header= "<<Header<<" Datos ="<<Mensaje<<endl;
        vector<string>   Header_Value;
        Header_Value.push_back(Header.substr(0,3));//tamaño a
        int inicio=0;
        int siguiente=_utils.str_to_int(Header_Value[0]);

        dato = Mensaje.substr(inicio,siguiente);
    }
    /*
     *
     * genero un header con los tamaños de los parametros que necesito
     * */
    string make_header()
    {
        string header="-N";
        string dato_size=to_string(dato.size());

        header+=string(3-dato_size.size(),'0').append(dato_size);
        string tam= to_string(header.size()+dato.size());
        header = string(10-tam.size(),'0').append(tam) + header;
        return header;
    }
    /*
     * proceso el mensaje sacando el header
     * y las partes del mensaje
     * */
    void Procesar_Pedido(int Tabla)
    {
    	
		string instruccion="INSERT INTO public.nodo"+to_string(Tabla)+"(frase) VALUES ('"+dato+"')";
		res = PQexec(conn,instruccion.c_str());
	    cout<<"Se añadio un nuevo nodo\n";
	    PQclear(res);
    	/*Aqui van las consultas y operaciones a la base de datos*/
    }
};

