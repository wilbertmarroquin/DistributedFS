# DistributedFS Final_Cloud
Distributed File System Prototype

# Dependencies

  -PostgreSQL (With pgAdmin III)
  
  -Create 3 tables nodo0,nodo1 and nodo2. 
  
  NOTE: There are 1 connector in "protocolo_dos.h" where you should change password.
  
      conn = PQconnectdb("dbname=postgres host=localhost user=postgres password=your password");
  

# Install Dependencies and Run

1.Install libpq5

    sudo apt-get install libpq5 libpq-dev
    
2.Run makefile

3.In terminal run 1 master (not change port 1100)

    ./master 1100

4.Only 3 slaves or nodes:

    ./nodo 12000

    ./nodo 12001
  
    ./nodo 12003

5.Client:

    ./client 1100 -N LOL (For push data)
    ./client 1100 -Q LOL (For request data)

# Troubleshooting

Maybe you will have problem with link postgresql.

1.Request postgresql include address: /usr/local/include (Maybe this will be different).

    pg_config --includedir

2.Change base.h:

    #include </usr/include/postgresql/libpq-fe.h>
    Change axample:
    #include </usr/local/include/libpq-fe.h>
  
3.Request postgresql library address: /usr/local/pgsql/lib. 

    pg_config --libdir

4.Change makefile to compile with correct address.

    g++ -std=c++11 -o master main_master.cpp -L/usr/local/pgsql/lib -lpq

    g++ -std=c++11 -o client main_client.cpp -L/usr/local/pgsql/lib -lpq

    g++ -std=c++11 -o nodo main_nodo.cpp -L/usr/local/pgsql/lib -lpq

  
