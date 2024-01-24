#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 10000
void read_childproc(int sig);  //To define child process function
void signalhandler(int SIG); //signal handler so that ABORT, terminated command can be triggered

int main(int argc, char *argv[]) {
    int server_socket, client_socket;  //server socker and client socket variable initialized
    struct sockaddr_in server_address, client_address; //predefined strucutre for IP, Port, protocol

    pid_t pid;  
    struct sigaction act;  // structure for child process 
    socklen_t address_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    
    // User should provide port, directory name where they want to save file
    if(argc!=3){
        printf("Please Enter in this format: %s <Port> <Directory> \n", argv[0]);
        exit(1);
    }

    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state=sigaction(SIGCHLD, &act, 0);

//Creating socket
server_socket=socket(PF_INET, SOCK_STREAM,0);
memset(&server_address,0, sizeof(server_address)); //set address values zoro
server_address.sin_family=AF_INET; //defining family for IPV4
server_address.sin_addr.s_addr=htonl(INADDR_ANY); //Local IP 127.0.0.1 but for any
server_address.sin_port=htons(atoi(argv[1])); //converting port to network adress

//binding socket
if(bind(server_socket,(struct sockaddr*)&server_address, sizeof(server_address))==-1){
    perror("Binding error");
}
//listening for at least five client at the same time
if(listen(server_socket,5)==-1) {
    perror("listen() error");
}
if(listen(server_socket,5)==0) {
    printf("Server is listening on port %s \n", argv[1]);
    signal(SIGTERM,signalhandler);
    signal(SIGINT,signalhandler);
}
while(1)
{
    //accept connection from client and file read write operation
    FILE *destination;
    address_sz=sizeof(client_address);
    client_socket=accept(server_socket,(struct sockaddr*)&client_address,&address_sz);
    if(client_socket==-1)
           continue;
    else 
          puts("Client Connected to the Server\n");
          //printf("%d", getpid());
    pid=fork();
    if(pid==-1){
        close(client_socket);
        continue;
    }
    if(pid==0) {
        close(server_socket); //connection established then close socket
        char filename[100]={0}; //filename defined
        snprintf(filename,100,"\\%d.txt",getpid());  //it will create all file with name of corresponding PID value.txt
        destination=fopen(strcat(argv[2],filename),"w"); //opening file creating at directory argv[2] with filename pid.txt
        while((str_len=read(client_socket,buf,BUF_SIZE))!=0){ //reading file via buffer from client
               fputs(buf,destination);  //writing buffer values to destination file
               bzero(buf,BUF_SIZE); //reseting buffer 
        }
        fclose(destination);
        close(client_socket);
        puts("Client disconnected\n");
        return 0;
    }
    else 
        close(client_socket);     
}
close(server_socket);

return 0;
}
//defining fucntion
void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG); // wait if any child process remaining -1 value
    printf("Removed Process ID: %d \n",pid);
}
//defining signal function
void signalhandler(int SIG){
    if(SIG==SIGINT || SIGTERM|| SIGQUIT) {
        printf("\nServer Aborted: %d", SIG);
        exit(EXIT_FAILURE);
    }
}