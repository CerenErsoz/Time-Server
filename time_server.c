#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>    // for strlen
#include <unistd.h>    // for write
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_addr

#define PORT_NUMBER 60000
time_t rawtime;
struct tm *time_info;
char time_array[80];

int main(int argc, char *argv[]){
    int socket_desc, new_socket, c;
    struct sockaddr_in server , client;
    char *message;

    //create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);//adress family, type(TCP), protocol

    if (socket_desc == -1){
        puts("Could not create socket");
        return 1;
    }
    puts("Socket created");

    server.sin_family = AF_INET;//IPv4 Internet protocols
    server.sin_addr.s_addr = INADDR_ANY;//IPv4 local host address
    server.sin_port = htons(PORT_NUMBER);//server will listen to 60000 port

    // Bind
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
        puts("Binding failed");
        return 1;
    }
    puts("Socket is binded");
    
    listen(socket_desc, 3);
    char recv_client_msg[100];//holds the input
    int length = 0;//input length
    char tempWord[100];
    int i = 0;
    
    c = sizeof(struct sockaddr_in);
    
    while(new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) ){
        puts("Connection accepted");
        message = "Hello client. I am waiting your request.\n";
        write(new_socket, message, strlen(message));


        while(1){//where it will constantly receive requests from the user
            
            message = "\n>>>";
            write(new_socket, message, strlen(message));            
            read(new_socket, recv_client_msg, sizeof(recv_client_msg));  

            length = strlen(recv_client_msg);

	        for(i = 0; i < length; i++){
                if(recv_client_msg[i] == '\n')//Adds \r\n when receiving input, checks to delete them
               	    recv_client_msg[i] = '\0';
               	    
                if(recv_client_msg[i] == '\r')
               	    recv_client_msg[i] = '\0';

                if(recv_client_msg[i] != '\0')
                    tempWord[i] = recv_client_msg[i];
    	    }

            time(&rawtime);//local time is taken
            time_info = localtime(&rawtime);

	        if(strcmp(tempWord, "GET_TIME") == 0){
                strftime (time_array, 80, "%X", time_info);
                write(new_socket, time_array, strlen(time_array));
            }
            else if(strcmp(tempWord, "GET_DATE") == 0){
                strftime (time_array, 80, "%D", time_info);
                write(new_socket, time_array, strlen(time_array));
            }
            else if(strcmp(tempWord, "GET_TIME_DATE") == 0){
                strftime (time_array, 80, "%X, %D", time_info);
                write(new_socket, time_array, strlen(time_array));
            }
            else if(strcmp(tempWord, "GET_TIME_ZONE") == 0){
                strftime (time_array, 80, " %z", time_info);
                write(new_socket, time_array, strlen(time_array));
            }
            else if(strcmp(tempWord, "GET_DAY_OF_WEEK") == 0){
                strftime (time_array, 80, "%A", time_info);
                write(new_socket, time_array, strlen(time_array));
            }
            else if(strcmp(tempWord, "CLOSE_SERVER") == 0){
                message = "GOOD BYE\n";
                write(new_socket, message, strlen(message));
                close(new_socket);
            }
            else{
                message = "INCORRECT REQUEST\n";
                write(new_socket, message, strlen(message));
            }
        }
    }    

    close(socket_desc);
    return 0;
}
