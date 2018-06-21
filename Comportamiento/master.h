#include "base.h"
#include "../Protocolo/Protocolo_Nodo.h"
#include "../Protocolo/Protocolo_Query.h"
#include "../Protocolo/Protocolo_Conexion.h"
class Master:public Base
{
public:
    Master(int b):Base(b) {}
    /*
     *
     * metodo para esperar conexiones entrantes
     *
     * */
    void Proceso_Comando(string M)
    {

    	string Comando=M.substr(0,2);
    	if(Comando=="-N")
    	{
    		protocolo_new_nodo Estructura(M);
    		cout<<"Dato = "<<Estructura.dato<<endl;
    		int Eleccion=_utils.Seleccionar_Esclavo(Estructura.dato);
    		cout<<"Mi Eleccion = "<<Eleccion<<endl;
    		int Socket_Envio=Tabla_Esclavos[Eleccion]->Socket_Esclavo;
    		M=Estructura.make_header() + Estructura.dato;
            if(Tabla_Esclavos[Eleccion]->Activo)
            {
                //echoBuffer= new char[M.size()];
                //strcpy(echoBuffer, M.c_str());
                Enviar_Mensaje(Socket_Envio,&M);
            }
            else
                cout<<"Se perdio Esclavo: "<<Eleccion<<endl;
            int Replica= _utils.Seleccionar_Replica(Eleccion);
            Socket_Envio=Tabla_Esclavos[Replica]->Socket_Esclavo;
            //echoBuffer= new char[M.size()];
            //strcpy(echoBuffer, M.c_str());
            Enviar_Mensaje(Socket_Envio,&M);
    		//Estructura.Procesar_Pedido();
    	}
    	else
    	{
    		if(Comando=="-Q")
    		{
                vector<string> Resp;
    			protocolo_query Estructura(M);
    			cout<<"Dato1 = "<<Estructura.dato<<endl;
    			int Eleccion=_utils.Seleccionar_Esclavo(Estructura.dato);
	    		cout<<"Mi Eleccion = "<<Eleccion<<endl;
                Ver_Tabla_Esclavos();
                cout<<"Activo: "<<Tabla_Esclavos[Eleccion]->Activo<<endl;
                if(!Tabla_Esclavos[Eleccion]->Activo)
                {
                    cout<<"Pasando a Replica"<<endl;
                    Eleccion=_utils.Seleccionar_Replica(Eleccion);
                    cout<<"Nueva Eleccion = "<<Eleccion<<endl;
                }
                int Socket_Envio=Tabla_Esclavos[Eleccion]->Socket_Esclavo;
	    		M=Estructura.make_header() + Estructura.dato;
                //echoBuffer= new char[M.size()];
	    		//strcpy(echoBuffer, M.c_str());
	    		Enviar_Mensaje(Socket_Envio,&M);
                permitir_conexion_query();
                string Mensaje_Query;
                Recibir_Mensaje(querySock,&Mensaje_Query);
                Mensaje_Query = string(10-to_string(Mensaje_Query.size()).size(),'0').append(to_string(Mensaje_Query.size())) + Mensaje_Query;
                cout<<"Mensaje Query"<<Mensaje_Query<<endl;
                Cerrar_Conexion(querySock);
                Enviar_Mensaje(clntSock,&Mensaje_Query);
    		}
                
    	}
    }
    void Enviar_Esclavos(string Mensa)
    {
    	string M;
    	string N_Esc=to_string(Esclavos_Actuales);
    	string header="-X"+string(2-N_Esc.size(),'0').append(N_Esc);
    	string cuerpo;
    	int count=0;
    	string Sub_Mensaje(Mensa);
    	string T_Sub_M=to_string(Sub_Mensaje.size());
    	Sub_Mensaje=string(10-T_Sub_M.size(),'0').append(T_Sub_M)+Sub_Mensaje;
    	cout<<"Mensaje Enviando Ex= "<<Sub_Mensaje<<endl;
        //echoBuffer= new char[Sub_Mensaje.size()];
    	//strcpy(echoBuffer, Sub_Mensaje.c_str());
    	for(int i=0;i<Esclavos_Actuales;i++)
    	{
            cout<<"envaindo al esclavo "<<i<<endl;
    		string ip_size=to_string(Tabla_Esclavos[i]->ip.size());
            cout<<"ip_size "<<ip_size<<endl;
        	string puerto_size = to_string(Tabla_Esclavos[i]->puerto.size());
            cout<<"puerto_size "<<puerto_size<<endl;
    		header+=string(2-ip_size.size(),'0').append(ip_size);
            cout<<"header "<<header<<endl;
        	header+=string(2-puerto_size.size(),'0').append(puerto_size);
            cout<<"header 2 "<<header<<endl;
            cout<<"tabla_esclavo ip"<<Tabla_Esclavos[i]->ip<<endl;
            cout<<"tabla esclavo puerto"<<Tabla_Esclavos[i]->puerto<<endl;
            cout<<"tabla eslavo[i] activo "<<Tabla_Esclavos[i]->Activo<<endl;
        	cuerpo+=Tabla_Esclavos[i]->ip;
            cuerpo+=Tabla_Esclavos[i]->puerto;
            cuerpo+=to_string(Tabla_Esclavos[i]->Activo);
        	Enviar_Mensaje(Tabla_Esclavos[i]->Socket_Esclavo,&Sub_Mensaje);
    		cout<<"Enviando a:"<<Tabla_Esclavos[i]->puerto<<" Socket: "<<Tabla_Esclavos[i]->Socket_Esclavo<<endl;
    	}
    	string tam= to_string(header.size()+cuerpo.size());
    	header = string(10-tam.size(),'0').append(tam) + header;
    	M=header+cuerpo;
        //echoBuffer= new char[M.size()];
    	//strcpy(echoBuffer, M.c_str());
        Enviar_Mensaje(Tabla_Esclavos[Esclavos_Actuales]->Socket_Esclavo,&M);
    }
    void _iniciar()
    {
    	while(Esclavos_Actuales < Numero_Esclavos)
    	{
    		permitir_conexion();
    		printf("Entro Esclavo: %d\n",Esclavos_Actuales);
    		cout<<"Esperando Informacion Esclavo..."<<endl;
            string Mensaje_Esclavo;
    		Recibir_Mensaje(Tabla_Esclavos[Esclavos_Actuales]->Socket_Esclavo,&Mensaje_Esclavo);
            //cout<<echoBuffer<<endl;
            cout<<"Ayudalo"<<Mensaje_Esclavo<<endl;
            string temp(Mensaje_Esclavo);
			string Verificar=Mensaje_Esclavo.substr(0,2);
    		if(Verificar=="-X")
    		{
    			protocolo_conexion Estructura(Mensaje_Esclavo);
    			Tabla_Esclavos[Esclavos_Actuales]->ip=Estructura.ip;
    			Tabla_Esclavos[Esclavos_Actuales]->puerto=Estructura.puerto;
    			Tabla_Esclavos[Esclavos_Actuales]->Activo=1;
    			Enviar_Esclavos(temp);
    			Esclavos_Actuales++;
                Ver_Tabla_Esclavos();
    			//cout<<"Esclavo Registrado IP= "<<Estructura.ip<<" Puerto= "<<Estructura.puerto<<endl;
    		}
    	}
    }
    void _escuchar()
    {
        for (;;) /* Run forever */
        {
            permitir_conexion_cliente();
            printf("administrando cliente %s\n", inet_ntoa(echoClntAddr.sin_addr));

            int recvMsgSize=0;
            string Mensaje;
            Recibir_Mensaje(clntSock, &Mensaje);
          	cout<<"Mensaje= "<<Mensaje<<endl;
          	Proceso_Comando(Mensaje);
            //protocolo_uno.process_message_recv(recibido,echoBuffer);
            //cout<<"recibido "<<recibido.command<<"  "<<recibido.parameters<<endl;
            //me comunico con el nodo de ese puerto y reenvio
            // strcpy(echoBuffer, mensaje.c_str());
            /*int Socket_Envio=Tabla_Esclavos[0]->Socket_Esclavo;
            Enviar_Mensaje(Socket_Envio,echoBuffer);
            //recibo el resultado
            Recibir_Mensaje(Socket_Envio, echoBuffer);
            //cierro la conexion con el nodo
            //Cerrar_Conexion(Socket_Envio);


            //envio al cliente el resultado
            Enviar_Mensaje(clntSock, echoBuffer);
            //cierro la conexion con el cliente*/
            Cerrar_Conexion(clntSock);

        }

    }
};
