#include "base.h"
#include "../Protocolo/Protocolo_Nodo.h"
#include "../Protocolo/Protocolo_Query.h"
class Client:public Base
{
public:
    string header;
    void new_nodo(string puerto, string dato,string frecuencia="0")
    {
    	protocolo_new_nodo Estructura_Mensaje(dato,frecuencia);
    	string Mensaje=Estructura_Mensaje.make_header() + dato;
    	_conectar(puerto,"192.168.1.12");
        //echoBuffer= new char[Mensaje.size()];
		//strcpy(echoBuffer, Mensaje.c_str());
		Enviar_Mensaje(SocketFD,&Mensaje);
		cout<<"Se envio Mensaje\n";
		 Cerrar_Conexion(SocketFD);    	
    }
    void query(string puerto, string dato)
    {
        vector<string> Res;
        protocolo_query Estructura_Mensaje(dato,10);
        string Mensaje=Estructura_Mensaje.make_header() + dato;
        cout<<"Ayuda: "<<Mensaje<<endl;
        _conectar(puerto,"192.168.1.12");
        //echoBuffer= new char[Mensaje.size()];
        //strcpy(echoBuffer, Mensaje.c_str());
        Enviar_Mensaje(SocketFD,&Mensaje);
        cout<<"Se envio Mensaje\n";
        int Espera_M=1;
        cout<<"Esperando Respuesta"<<endl;
        string Mensaje_Query;
        while(Espera_M!=0)
        {
            cout<<"Contador= "<<Espera_M<<endl;
            Recibir_Mensaje(SocketFD,&Mensaje_Query);
            Espera_M--;
            cout<<"Espera2: "<<Espera_M<<endl;
        }
        cout<<"Mensaje_Recibido: "<<Mensaje_Query<<endl;
        Cerrar_Conexion(querySock);
    }

};


