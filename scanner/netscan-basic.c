#include "netscan-basic.h"
/*Ip actual y final*/
in_addr_t IP, IPFinal;
int16 port;

in_addr_t incrementarIP(in_addr_t ip){
    struct in_addr addr;
    addr.s_addr = ip;
    uint32_t ip_num = ntohl(addr.s_addr);
    ip_num++;
    addr.s_addr = htonl(ip_num);
    return addr.s_addr;
}

in_addr_t generar(){
    char buffer[16];
    int32 tam;
    char *p;

    if((!IP) && (!IPFinal)){
        memset(buffer,0,16);
        fgets(buffer,15,stdin);
        tam = (int)(strlen((char*)buffer));
        
        //tam puede tener una IP(tamanio=8) o \n
        if (tam<7){
            return (in_addr_t)0;
        }
        tam--;
        p=buffer+tam;

        if(*p=='\n' || *p=='\r'){
            *p = 0;
        }
        return inet_addr(buffer);
    }

    if(IP>=IPFinal){
        return (in_addr_t)0;
    }
    in_addr_t ip_actual = IP;
    IP = incrementarIP(IP);
    return ip_actual;
}

int8 *header(int s,in_addr_t ip){
    static int8 buffer[256];
    int8 *p;
    int16 i;


    memset(buffer,0,256);
    i = read(s, (char*)buffer,255);
    if (i<2){
        printf("La IP es: 0x%x\n",ip);
    }
    else{
        i--;
        p = buffer+i;
        if(*p=='\n' || *p=='\r'){
            *p=0;
        }

        printf("0x%x: %s\n",ip,buffer);
    }

    return buffer;
}

bool conexion(in_addr_t ip,int16 port){
    struct sockaddr_in sock;
    int s, ret;

    s = socket(AF_INET, SOCK_STREAM,0);
    if(s<0){
        fprintf(stderr, "Error al crear el socket\n");
        return false;
    }

    sock.sin_family = AF_INET;
    sock.sin_port = htons((int)port);
    sock.sin_addr.s_addr = ip;

    //fprintf(stderr, "\nSocket descriptor: %d", s);
    fprintf(stderr, "\nIntentando conectar a %s:%d\n", inet_ntoa(sock.sin_addr), ntohs(sock.sin_port));

    ret = connect(s, (struct sockaddr*)&sock, sizeof(struct sockaddr));
    if(ret<0){
        close(s);
        return false;
    }

    //Comprobamos header de la conexión
    header(s,ip);
    close(s);
    return true;
    
}

int main(int argc, char **argv){
    in_addr_t ip;

    if (argc<2){
        fprintf(stderr, "Uso del escaner: %s <puerto> [ip inicial] [ip final]\n",*argv );
        return -1;
    }

    if(argc > 2){
        IP = inet_addr(argv[2]);
        IPFinal = (argc>3) ? inet_addr(argv[3]) : inet_addr(LAST_IP);

        if (IP==INADDR_NONE || IPFinal==INADDR_NONE || IPFinal<IP){
            fprintf(stderr, "Uso del escaner: %s <puerto> [ip inicial] [ip final]\n",*argv );
            return -1;
        }
    }

    port = (int16)atoi(argv[1]);

    printf("Empieza el escaneo, Inicio %d Final %d Puerto %d\n", IP,IPFinal,port);

    while((ip=generar())){
        conexion(ip,port);
    }

    return 0;

}