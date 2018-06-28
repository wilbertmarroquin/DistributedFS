#ifndef UTILS_H
#define UTILS_H
#include<string>
#include<bitset>
#include<sstream>

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
    /*
    * Genera un numero binario(en int) data un tamaño size
    **/
    string generate_random_bin(int size){
        string bin;
        srand(time(0));
        for (int i=0;i<size/8;i++){
            int ran = rand() % 122 + 97;
            string bit = bitset<8>(ran).to_string();
            bin += bit;
        }
        return bin;
    }

    /*
    * Completa el binario tomando en cuenta el tamaño mas grande
    * a -> binario 1
    * b -> binario 2
    * */
    void resize(string& a, string& b){
        int diff = a.size() - b.size();
        string resi = "";
        for (int i=0;i<abs(diff);i++)
            resi += "0";

        if (diff == 0)
            return;
        else if (diff > 0){
            b = resi + b;
        }
        else{
            a = resi + a;
        }
        return;
    }

    /*
    *Operacion xor entre 2 binarios
    * */

    string bin_xor_bin(string a, string b){
        string result;
        resize(a,b);
        for (int i=0;i<a.size();i++){
            string bit;
            if(a[i]==b[i])
                bit = "0";
            else
                bit = "1";
            result += bit;
        }
        return result;
    }

    /*
    * Convierte un string a binario
    * Toma cada letra y lo transforma a bitset de tamaño 8
    * retorna el numero binario en string
    * */

    string to_bin(const string& input){
        ostringstream oss;
        for (auto c:input){
            oss<<bitset<8>(c);
        }
        return oss.str();
    }

    /*
    * Convierte un string binario a un string
    * 0010101 -> hola (ejemplo)
    * */

    string to_str(const string& input){
        stringstream sstream(input);
        string output;
        while(sstream.good()){
            bitset<8> bits;
            sstream >> bits;
            char c = char(bits.to_ullong());
            output += c;
        }
        return output;
    }

    /*
    * Encriptacion
    * Recibe la palabra y 2 strings que seran las 2 nuevas palabras
    * alpha y betha que son las encriptaciones
    * Retorna la clave
    * */
    string encriptar(string input,string& alpha, string& betha ){
        //Input data a binario  
        string D = to_bin(input);
        //Parameter data(have to be shorter than D)
        string C = generate_random_bin((int)D.size()-2);
        //Key
        string key = generate_random_bin((int)D.size());
        //R -> D -(xor) C
        string R = bin_xor_bin(D,C);
        //alpha = C xor key
        alpha = bin_xor_bin(C,key);
        //betha = R xor key
        betha = bin_xor_bin(R,key);
        //Darle formato de palabra a alpha y betha, sino queda en binario
        alpha = to_str(alpha);
        betha = to_str(betha);
        return key;
    }

    /*
    * Desencriptar
    * Recibe alpha,betha y la clave
    * */
    string desencriptar(string alpha, string betha, string key){
        string a = to_bin(alpha);
        string b = to_bin(betha);
        string phi = bin_xor_bin(a,key);
        string phi2 = bin_xor_bin(b,key);
        //D is data
        string D = bin_xor_bin(phi,phi2);
        return to_str(D);
    }

    /*
    * Ejemplo
    setlocale(LC_ALL, "utf8");
    string myString;
    cout<<"Mensaje: ";
    cin>>myString;
    string alpha,betha,key;
    key =  encriptar(myString,alpha,betha);
    cout<<endl<<"alpha: "<<alpha<<endl;
    cout<<"betha: "<<betha<<endl;
    cout<<"Key: "<<to_str(key)<<endl;
    cout<<"Respuesta: "<<desencriptar(alpha,betha,key)<<endl;
    **/
};
utils _utils;//las clases que me importen tendran que llamarme a travez de este objeto
#endif
