#include "Comportamiento/master.h"


int main(int argc, char *argv[])
{
    if (argc != 2)     /* Test for correct number of arguments */
    {
        cout<<"debes de proporcionar el puerto"<<endl;
        exit(1);
    }

    int puerto =atoi(argv[1]);


    Master base(puerto);
    base.conexiones();
    //thread th_conexion(&Master::conexiones,&base);
    thread th_keep_alive (&Master::Keep_Alive,&base);
    base.conexiones2();
    //th_conexion.join();
    th_keep_alive.join();
    //base.iniciar_socket();
    //base._escuchar();
}

