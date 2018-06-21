#include"Comportamiento/nodo.h"


int main(int argc, char *argv[])
{
    if (argc != 2)     /* Test for correct number of arguments */
    {
        cout<<"debes de proporcionar el puerto"<<endl;
        exit(1);
    }

    int puerto =atoi(argv[1]);


    Nodo b(puerto);
    cout<<"voy a escuchar "<<endl;
    b.conexiones();
    //thread th_conexiones (&Nodo::conexiones,&b);
    thread th_keep_alive (&Nodo::Keep_Alive,&b);
    b.conexiones2();
    th_keep_alive.join();
    //th_conexiones.join();
    //  i//  Master base(puerto);
   // b.iniciar_socket();
   // b._escuchar();

}


