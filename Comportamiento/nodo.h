#include "base.h"
#include "../Protocolo/Protocolo_Nodo.h"
#include "../Protocolo/Protocolo_Query.h"
#include "../Protocolo/Protocolo_Conexion.h"
class Nodo: public Base
{

public:
	Nodo(int b):Base(b) {}
	string IP_Address;
	int Mi_Num;
	int Num_Querys(vector<string>* Querys)
	{
		int count=0;
		for (int i = 0; i < Querys->size(); i++)
		{
			int Eleccion=_utils.Seleccionar_Esclavo((*Querys)[i]);
			if(Eleccion<3 && Eleccion!=Mi_Num)
    		{
	    		count++;
	    	}
		}
		return count;
	}
	void Proceso_Comando_C(string M)
    {
    	string Comando=M.substr(0,2);
    	cout<<"Comando= "<<Comando<<endl;
    	if(Comando=="-N")
    	{
    		protocolo_new_nodo Estructura(M);
    		cout<<"Dato = "<<Estructura.dato<<endl;
    		Estructura.Procesar_Pedido(Mi_Num);
    	}
    	else
    	{
    		if(Comando=="-Q")
    		{
    			protocolo_query Estructura(M);
    			cout<<"Dato1 = "<<Estructura.dato<<endl;
    			vector<string> Res=Estructura.Procesar_Pedido(Mi_Num);
    			cout<<"Encontro: "<<Res[0]<<endl;
				string Palabras;
				string tam=to_string(Res.size());
				string header=string(8-tam.size(),'0').append(tam);
				tam.clear();
				for(int i=0;i<Res.size();i++)
				{
					tam=to_string(Res[i].size());
					header+=string(3-tam.size(),'0').append(tam);
					Palabras+=Res[i];
				}
				tam.clear();
				string Men="-Q"+header+Palabras;
				tam=to_string(Men.size());
				tam=string(10-tam.size(),'0').append(tam);
				Men=tam+Men;
				cout<<Men<<endl;
				_conectar_Esclavo("1100",IP_Master,querySock);
				//echoBuffer= new char[Men.size()];
				//strcpy(echoBuffer, Men.c_str());
				Enviar_Mensaje(querySock,&Men);
				cout<<"Aqui Termina"<<endl;
				//usleep(50000);
    			
    		}
    		else
    		{
    			if(Comando=="-X")
    			{
	    			protocolo_conexion Estructura(M);
	    			Tabla_Esclavos[Esclavos_Actuales]->ip=Estructura.ip;
	    			Tabla_Esclavos[Esclavos_Actuales]->puerto=Estructura.puerto;
	    			Tabla_Esclavos[Esclavos_Actuales]->Activo=true;
	    			cout<<"IP = "<<Tabla_Esclavos[Esclavos_Actuales]->ip<<" puerto = "<<Tabla_Esclavos[Esclavos_Actuales]->puerto<<" Activo = "<<Tabla_Esclavos[Esclavos_Actuales]->Activo<<endl;
	    			permitir_conexion();
	    			cout<<"Me conecte con: "<<Tabla_Esclavos[Esclavos_Actuales]->puerto<<endl;
	    			int S=Tabla_Esclavos[Esclavos_Actuales]->Socket_Esclavo;
	    			T_Vecinos.push_back( thread(&Nodo::_escuchar_vecinos,this,S,Esclavos_Actuales) );
	    			Esclavos_Actuales++;
	    			cout<<"Numero de Esclavos: "<<Esclavos_Actuales<<endl;
    			}
 
    		}
    	}

    }
    void Mi_Ip()
    {
    	struct ifaddrs * ifAddrStruct=NULL;
	    struct ifaddrs * ifa=NULL;
	    void * tmpAddrPtr=NULL;
	    int count=0;
	    getifaddrs(&ifAddrStruct);

	    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
	        if (!ifa->ifa_addr) {
	            continue;
	        }
	        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
	            // is a valid IP4 Address
	            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
	            char addressBuffer[INET_ADDRSTRLEN];
	            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
	            //ºif(count==1)
	            	IP_Address=addressBuffer;
	            count++;
	        }
	    }
	    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

	    //cout<<"IP_Address: "<<IP_Address<<endl;
    }
    void Enviar_Info()
    {
    	string port_s=to_string(port);
    	protocolo_conexion Estructura_Mensaje(IP_Address,port_s);
        string Mensaje=Estructura_Mensaje.make_header() + IP_Address + port_s;
        //echoBuffer= new char[Mensaje.size()];
        //strcpy(echoBuffer, Mensaje.c_str());
        Enviar_Mensaje(SocketFD,&Mensaje);
        cout<<"Se envio Mensaje\n";
    }
    void Recibir_Info()
    {
    	string Mensaje_Maestro;
		Recibir_Mensaje(SocketFD, &Mensaje_Maestro);
		string Verificar=Mensaje_Maestro.substr(0,2);
		if(Verificar=="-X")
		{
			printf("\nline a procesar %s\n",Mensaje_Maestro.c_str());
	        Mensaje_Maestro = Mensaje_Maestro.erase(0,2);
	        int N_Esc=_utils.str_to_int(Mensaje_Maestro.substr(0,2));
	        Mensaje_Maestro = Mensaje_Maestro.erase(0,2);
	        int HEADER_SIZE=N_Esc*4;
	        Esclavos_Actuales=N_Esc+1;
	        Mi_Num=N_Esc;
	        string Header= Mensaje_Maestro.substr(0,HEADER_SIZE);
	        Mensaje_Maestro = Mensaje_Maestro.substr(HEADER_SIZE,Mensaje_Maestro.size()-HEADER_SIZE);//saco al header del string

	        cout<<"Header= "<<Header<<" Datos ="<<Mensaje_Maestro<<endl;
	        int inicio=0;
	        for (int i = 0; i < N_Esc; i++)
	        {
	        	vector<string> Header_Value;

		        Header_Value.push_back(Header.substr(i*4,2));//tamaño a
		        Header_Value.push_back(Header.substr(i*4+2,2));//tamaño b

		        int siguiente=_utils.str_to_int(Header_Value[0]);

		        Tabla_Esclavos[i]->ip= Mensaje_Maestro.substr(inicio,siguiente);
		        cout<<"IP: "<<Tabla_Esclavos[i]->ip<<endl;
		        inicio+=siguiente;
		        siguiente=_utils.str_to_int(Header_Value[1]);

		        Tabla_Esclavos[i]->puerto = Mensaje_Maestro.substr(inicio,siguiente);
		        cout<<"Puerto: "<<Tabla_Esclavos[i]->puerto<<endl;
		        inicio+=siguiente;
		        siguiente=1;

		        Tabla_Esclavos[i]->Activo =_utils.str_to_int(Mensaje_Maestro.substr(inicio,siguiente));
		        cout<<"Activo: "<<Tabla_Esclavos[i]->Activo<<endl;
		        inicio+=siguiente;
		        _conectar_Esclavo(Tabla_Esclavos[i]->puerto,Tabla_Esclavos[i]->ip,Tabla_Esclavos[i]->Socket_Esclavo);
		        cout<<"Me conecte con: "<<Tabla_Esclavos[i]->puerto<<endl;
		        T_Vecinos.push_back( thread(&Nodo::_escuchar_vecinos,this,Tabla_Esclavos[i]->Socket_Esclavo,i) );
	        }
	        Tabla_Esclavos[N_Esc]->ip=IP_Address;
	        Tabla_Esclavos[N_Esc]->puerto=to_string(port);
	        Tabla_Esclavos[N_Esc]->Activo=true;

		}
    }
    void Conectar_BD()
    {
    	conn = PQconnectdb("dbname=postgres host=localhost user=postgres password=sostastico");
		if (PQstatus(conn) == CONNECTION_BAD) {
		        puts("We were unable to connect to the database");
		        exit(0);
		}
    }
    //protocolo_dos _protocolo_dos;
    void _iniciar()
    {
    	Mi_Ip();
    	cout<<"Mi IP= "<<IP_Address<<endl;
    	_conectar("1100",IP_Master);
    	Enviar_Info();
    	Recibir_Info();
    	Conectar_BD();
    }
    void _escuchar()
    {
    	printf("Conectado a Maestro \n");
        for (;;) /* Run forever */
        {

            int recvMsgSize=0;
            string Mensaje;
            Recibir_Mensaje(SocketFD, &Mensaje);
          	cout<<"Mensaje= "<<Mensaje<<endl;
          	Proceso_Comando_C(Mensaje);
          	cout<<"Esperando Maestro: "<<endl;
            /*protocolo_dos::datos_protocol recibido;
            _protocolo_dos.process_message_recv(recibido,echoBuffer);
            cout<<"recibido "<<recibido.a<<"  "<<recibido.b<<endl;
            recibido.suma();
            strcpy(echoBuffer,recibido.rpt.c_str());
            Enviar_Mensaje(SocketFD, echoBuffer);*/
            //close(clntSock);
        }

    }
    void _escuchar_vecinos(int SocketV, int Pos_Esc)
    {
    	bool seguir=1;
        while (seguir==1)
        {
        	cout<<"Estoy_escuchando: "<<SocketV<<endl;
            int recvMsgSize=0;
            string Mensaje;
            Recibir_Mensaje(SocketV, &Mensaje);
          	cout<<"Mensaje_Vecino= "<<Mensaje<<endl;
          	//thread temp (&Nodo::Proceso_Comando_C,this,Mensaje);
          	cout<<"Bote_Thread"<<endl;
          	//temp.detach();
          	Proceso_Comando_C(Mensaje);
          	cout<<"Esperando Maestro: "<<endl;
          	Ver_Tabla_Esclavos();
          	if(!Tabla_Esclavos[Pos_Esc]->Activo)
          	{
          		seguir=0;
          	}
          	cout<<"Seguir: "<<seguir<<endl;
            /*protocolo_dos::datos_protocol recibido;
            _protocolo_dos.process_message_recv(recibido,echoBuffer);
            cout<<"recibido "<<recibido.a<<"  "<<recibido.b<<endl;
            recibido.suma();
            strcpy(echoBuffer,recibido.rpt.c_str());
            Enviar_Mensaje(SocketFD, echoBuffer);*/
            //close(clntSock);
        }

    }
};


