#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/time.h> //i wanted to implement select() for time out but did not succeed 

#define BUF_SIZE 10000
#define filesize 256
#define Path_Dir "E:\\Client\\" //You can change directory from which you want to load file from client side

//void read_operation(int client_socket, char *buf); for echo purpose reply from server
void write_operation(int client_socket, char *buf, char *argv[]);
void write_operation_simple(int client_socket, char *buf);


int main(int argc, char *argv[]) {
    int client_socket;
    pid_t pid;
    char buf[BUF_SIZE];
    struct timeval tv;
    struct sockaddr_in client_address;
    if(argc<3 || argc>4) {
        printf("Please type: %s <IP> <Port>\n",argv[0]);
        exit(1);
    }
    //Socket creation
    client_socket=socket(PF_INET,SOCK_STREAM,0);
    memset(&client_address,0,sizeof(client_address));
    client_address.sin_family=AF_INET;
    client_address.sin_addr.s_addr=inet_addr(argv[1]);
    client_address.sin_port=htons(atoi(argv[2]));
    //connection to the server
    if(connect(client_socket,(struct sockaddr*)&client_address,sizeof(client_address))==-1) {
       perror("Connection  error!");
       exit(1);
    }
                 
    pid=fork();
    if(pid==0){ //if child is created
        if(argv[3]==0) {
            write_operation_simple(client_socket,buf);
            //exit(1);
        }
        if (argc==4)
       {
         write_operation(client_socket,buf,argv);
            exit(EXIT_FAILURE);
        }       
    }
    else 
        exit(EXIT_FAILURE);
    close(client_socket);

return 0;
}
void write_operation(int client_socket, char *buf,char *argv[]){
    while(1)
    {
        FILE *source;
        char filename[filesize]={0};
        strcat(filename,Path_Dir);
        strcat(filename,argv[3]);  //Upto now filename has directory and file name
        /*if(!strcmp(argv[3],"q\n")|| !strcmp(argv[3],"Q\n")) {
            shutdown(client_socket,SHUT_WR);
            return;
        }*/
        
            source=fopen(filename,"r"); //opening file in read mode
            if(source==NULL) {
                printf("File Failed to open: Please keep correct file name\n");
                exit(1);
            }
            else {
                printf("\n File Opened \n");
                while(fgets(buf,sizeof(buf),source)!=NULL){
                    write(client_socket,buf,BUF_SIZE); //writing from buf to file descriptor
                }
                bzero(buf,BUF_SIZE);
                printf("\n File copied successfully on socket \n");
                fclose(source);
                exit(1);
            }   

        

    }
}
void write_operation_simple(int client_socket, char *buf){
    
      
        printf("\nType Q or q to disconnect client\n");
        fgets(buf, BUF_SIZE, stdin); 
        if(!strcmp(buf,"q\n")|| !strcmp(buf,"Q\n")) {
            shutdown(client_socket, SHUT_WR);
			return;
        }
        
           
        

    
}