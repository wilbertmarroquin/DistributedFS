all:
	g++ -std=c++11 -o master main_master.cpp -L/usr/local/pgsql/lib -lpq -pthread
	g++ -std=c++11 -o client main_client.cpp -L/usr/local/pgsql/lib -lpq -pthread
	g++ -std=c++11 -o nodo main_nodo.cpp -L/usr/local/pgsql/lib -lpq -pthread
