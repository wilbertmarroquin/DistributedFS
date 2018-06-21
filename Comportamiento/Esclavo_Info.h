class Esclavo_Info
{
public:
	string ip;
	string puerto;
	int Socket_Esclavo;
	bool Activo;
	Esclavo_Info(string n_ip,string n_puerto)
	{
		ip=n_ip;
		puerto=n_puerto;
	}
};