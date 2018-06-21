#include <bits/stdc++.h>
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <unistd.h>     /* for close() */
#include "../Comportamiento/utils.h"
using namespace std;

//aca el header tiene el tamaño de a y b
//#a|#b|a + b
class protocolo_query
{
public:
	string dato;
    const int HEADER_SIZE=3; //tamaño del header
    int m_id;
    protocolo_query(string d,int id)
    {
    	cout<<"asignado parametros nueva query "<<endl;
        dato=d;
        m_id=id;
    }
    protocolo_query(string Buffer)
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
        string header="-Q";
        string dato_size=to_string(dato.size());

        header+=string(3-dato_size.size(),'0').append(dato_size);
        string tam= to_string(header.size()+dato.size());
        header = string(10-tam.size(),'0').append(tam) + header;
        return header;
    }
    void Respuesta_Cliente(string* Men,vector<string>* Resp)
    {
        string Palabras;
        string tam=to_string(Resp->size());
        string header=string(8-tam.size(),'0').append(tam);
        tam.clear();
        for(int i=0;i<Resp->size();i++)
        {
            tam=to_string((*Resp)[i].size());
            header+=string(3-tam.size(),'0').append(tam);
            Palabras+=(*Resp)[i];
        }
        tam.clear();
        *Men="-Q"+header+Palabras;
        tam=to_string(Men->size());
        tam=string(10-tam.size(),'0').append(tam);
        *Men=tam+*Men;
        cout<<*Men<<endl;
    }
    /*
     * proceso el mensaje sacando el header
     * y las partes del mensaje
     * */
    vector<string> Procesar_Pedido(int Tabla)
    {
    	/*Aqui van las consultas y operaciones a la base de datos*/
		int rec_count;
		vector<string> Resultado;
		string instruccion="SELECT frase FROM public.nodo"+to_string(Tabla)+" WHERE frase='"+dato+"'";
		cout<<instruccion<<endl;
		res = PQexec(conn,instruccion.c_str());
		if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			puts("We did not get any data!");
            PQclear(res);
	    }
	    else
	    {
	    	rec_count = PQntuples(res);
	    	printf("We received %d records.\n", rec_count);
	    	for(int i=0;i<rec_count;i++)
	    	{
	    		string palabra(PQgetvalue(res,i,0));
	    		Resultado.push_back(palabra);
	    		cout<<palabra<<endl;
	    	}
            PQclear(res);
            return Resultado;
	    }
    }
};
