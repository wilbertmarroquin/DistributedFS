#ifndef UTILS_H
#define UTILS_H
#include<string>

using namespace std;

/*
 *
 * poner aca todos los metodos de uso general
 *
 * */
class utils
{
public:
    int str_to_int(string s)
    {

        string str = s;
        istringstream buffer(str);
        int value;
        buffer >> value;
        return value;
    }
    unsigned long Hash_String(unsigned char *str)
    {
        unsigned long hash = 5381;
        int c;
        while (c = *str++)
            hash = ((hash << 5) + hash) + c;  /*hash * 33 + c;*/

        //return hash%1000;
            //cout<<"HASH= "<<hash<<endl;
            return hash%300;
    }
    int Seleccionar_Esclavo(string Palabra)
    {
        unsigned long H=Hash_String((unsigned char *) Palabra.c_str());
        if(H<100)return 0;
        if(H<200)return 1;
        if(H<300)return 2;
        /*if(H<400)return 3;
        if(H<500)return 4;
        if(H<600)return 5;
        if(H<700)return 6;
        if(H<800)return 7;
        if(H<900)return 8;
        if(H<1000)return 9;*/
    }
    int Seleccionar_Replica(int elec)
    {
        if(elec==2)
            return 0;
        else
            return elec+1;
    }
};
utils _utils;//las clases que me importen tendran que llamarme a travez de este objeto
#endif
