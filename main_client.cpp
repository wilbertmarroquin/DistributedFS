#include "Comportamiento/client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
int main(int argc, char *argv[])
{
	
    /* if (argc != 4)     /* Test for correct number of arguments */
    /*{
        cout<<"debes de proporcionar la suma, operacion  y parametros ejemplo:"<<endl;
        cout<<"11000 hola mundo"<<endl;
        cout<<"11000 suma 5 2"<<endl;
        exit(1);
    }*/

    string puerto = string(argv[1]);
    string comm =string(argv[2]);
    Client Manejo_Cliente;

    if(comm=="-N")
    {

        cout<<"enviando mensaje nuevo nodo";

        string dato=string(argv[3]);
        if(argc > 4)
        {
        	string atributo =string(argv[4]);
        	if(atributo=="-A")
        	{
        		atributo = string(argv[5]);
        		Manejo_Cliente.new_nodo(puerto,dato,atributo);
        	}
        }
        else
        {
        	Manejo_Cliente.new_nodo(puerto,dato);
        }

    }
    else {
	    if(comm=="-Q")
    	{
			cout<<"enviando mensaje query";

	        string dato=string(argv[3]);
	        
	        Manejo_Cliente.query(puerto,dato); 		
		}
    }
    return 1;
}


