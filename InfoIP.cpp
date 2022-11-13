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
    FILE* pipe = popen(data.c_str(), "r");
    if(!pipe){
        cout << "No se pudo ejecutar" << endl;
    }
    while(!feof(pipe)){
        if(fgets(buffer, 100, pipe) != NULL)
            result +=buffer;
    }
    pclose(pipe);
    string packetr = "";
    int busqueda = 0;
    for (int i= 0 ; i< result.length(); i++){
        if (result[i] == ','){
            for(int j=i+2; result[j]!= ' '; j++){
                packetr += result[j];
            }
            break;
        }
    }
    string valor;
    int packt = stoi(packargv);
    int pack = stoi(packetr);
    int packresu = packt - pack;
    if (pack>0){
        valor = "UP";
    }else{
        valor = "DOWN";
    }
    mymutex.lock();
    cout << sol;
    int large = 13-sol.length();
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
    ifstream cantidad(argv[1]);
    string lineas;
    while(getline(cantidad, lineas)){
        c_lineas++;
    }
    cantidad.close();
    cout << "IP           Trans.      Rec.     Perd.      Estado"<< endl;
    cout << "---------------------------------------------------"<< endl;
    string lista[c_lineas];
    ifstream cantip(argv[1]);
    string tip;
    while(getline(cantip,tip)){
        lista[i]=tip;
        i++;
    }
    cantip.close();
    thread threads[c_lineas];
    for (i=0; i < c_lineas; i++) {
        threads[i] = thread(info, lista[i], packa);
    }

    for (i=0; i< c_lineas; i++) {
        threads[i].join();
    }
    return 0;
}