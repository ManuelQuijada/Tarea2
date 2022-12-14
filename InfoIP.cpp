#include<iostream>
#include<thread>
#include<fstream>
#include<string>
#include<mutex>

using namespace std;

mutex mymutex;

void info (string sol, string packargv){
    int i = 0;
    string data = "ping " + sol + " -c " + packargv;
    char buffer[100];
    string result = "";
    //Creacion de parametros a tipo archivo
    FILE* pipe = popen(data.c_str(), "r");
    if(!pipe){
        cout << "No se pudo ejecutar" << endl;
    }
    while(!feof(pipe)){
        if(fgets(buffer, 100, pipe) != NULL)
            result +=buffer;
    }
    pclose(pipe);
    //Busqueda para sacar los paquetes recibidos
    string packetr = "";
    for (int i= 0 ; i< result.length(); i++){
        //utilizando la busqueda de la coma, de esta manera se busca la primera coma
        // y luego se busca hasta que aparesca un valor y no un espacio.
        if (result[i] == ','){
            for(int j=i+2; result[j]!= ' '; j++){
                packetr += result[j];
            }
            break;
        }
    }
    string valor;
    int packt = stoi(packargv); //conversion de paquetes transmitidos de string a int
    int pack = stoi(packetr); // conversion de paquete recibidos de string a int 
    int packresu = packt - pack; // valor correspondiente a la cantidad de paquetes recibido
    if (pack>0){
        valor = "UP";
    }else{
        valor = "DOWN";
    }
    mymutex.lock();
    cout << sol;
    int large = 15-sol.length();// cantidad de espacios entre la muestra en pantalla de la ip y los paquetes transmitidos
    for (int i=0;i<large;i++){
        cout<<" ";
    }
    cout << packt << "          " << pack << "          " << packresu << "          " << valor << endl;
    mymutex.unlock();
}

int main(int argc, char *argv[]){
    string packa = argv[2];
    int i = 0;
    int c_lineas = 0;
    //Cantidad de lineas que presenta el archvio txt
    ifstream cantidad(argv[1]);
    string lineas;
    while(getline(cantidad, lineas)){
        c_lineas++;
    }
    cantidad.close();
    cout << "IP           Trans.      Rec.     Perd.      Estado"<< endl;
    cout << "---------------------------------------------------"<< endl;
    string lista[c_lineas];
    //Se asigna la ip a lista[i], de esta manera se guarda en la variable
    ifstream cantip(argv[1]);
    string tip;
    while(getline(cantip,tip)){
        lista[i]=tip;
        i++;
    }
    cantip.close();
    //Inicializacion de los hilos a ocupar
    thread threads[c_lineas];
    for (i=0; i < c_lineas; i++) {
        threads[i] = thread(info, lista[i], packa);
    }

    for (i=0; i< c_lineas; i++) {
        threads[i].join();
    }
    return 0;
}