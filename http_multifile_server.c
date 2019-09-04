// server
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <json/json.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <time.h> 
#define PORT 8000 

int main(int argc, char const *argv[]) 

{ 
	//while(1)
	{
	    int server_fd, new_socket, valread; 
	    struct sockaddr_in address; 
	    int opt = 1; 
	    int addrlen = sizeof(address); 
	    char buffer[2048],test_buffer[1024]; 
	    unsigned char *buffer_1 ;
 	    char *httpimage ;
 	    char *httpimagetemp;
	    int n=0;
	    char ch;
	    int len;

            time_t seconds; 
      
            time(&seconds); 

            int *hi =seconds;

	   json_object *jobj = json_object_new_object();

    	   json_object *jstring1 = json_object_new_int((int *)hi);

           json_object *jstring2 = json_object_new_string("client");

    	   json_object_object_add(jobj,"server message1", jstring1);

    	   json_object_object_add(jobj,"server message2", jstring2);
 
	   strcpy(test_buffer,json_object_to_json_string(jobj));
 		
		// HTTP Response Headers

    	    char *text = "HTTP/1.1 200 OK\nAccept: text/plain\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    	    char *json = "HTTP/1.1 200 OK\nAccept: application/json\nContent-Type:application/json\nContent-Length: 1024\n\n ";    

    	    char *image = "HTTP/1.1 200 OK\nConnection: keep-alive\nAccept: image/gif\nContent-Type: image/gif\n";
	  
		// Read Image data from File        

	   FILE *file= fopen("/home/akash/Desktop/pika.gif","rb");

	   if(NULL == file)
	   {
		printf("File pointer not valid \n");
	   }


	   fseek(file,0,SEEK_END);

	   len = ftell(file);

	   fseek(file,0,SEEK_SET);
	

	   printf("file size : %d Bytes \n",len);

	   buffer_1 = (unsigned char *) malloc(sizeof(unsigned char)*len);

	   if(NULL == buffer_1)
	   {
		printf("buffer_1 pointer not valid \n");
		return -1;
	   }


           size_t r = fread(buffer_1,1,len+1,file);

	   if(r == len)
	   {
	    printf("File read proplerly \n");
	   }
#if 0

      	   //Print image file data

	   int count =0,j=0,k=0;

	   for(count =0;count <len;count++)
	   {
		printf("  %02X",buffer_1[count]);
		j++;
		if(j == 16)
		{
			printf("\n");
			j = 0;
		}
	   }

#endif	   

	       
	    // Creating socket file descriptor 

	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	    { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	    } 
	       
	    // Forcefully attaching socket to the port  

	    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	    { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	    } 
	    address.sin_family = AF_INET; 
	    address.sin_addr.s_addr = INADDR_ANY; 
	    address.sin_port = htons( PORT ); 
	       
	    // Forcefully attaching socket to the port  

	    if (bind(server_fd, (struct sockaddr *)&address,  
		                         sizeof(address))<0) 
	    { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	    } 

	    if (listen(server_fd, 3) < 0) 
	    { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	    } 

	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
		               (socklen_t*)&addrlen))<0) 
	    { 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	    }
    
	    
	    /*if(!(strcmp(buffer,name)))
	    {
		n=1;
	    }
	    if(!(strcmp(buffer,date)))
	    {
		n=2;
	    }
   	    switch (n) 
   	    { 
	       	case 1: send(new_socket , response1 , strlen(response1) , 0 );
		break;  
    		case 2: send(new_socket , response2 , strlen(response2) , 0 );
		break;  
	 	default: send(new_socket , error , strlen(error) , 0 );
	    }
	    n=0; 
*/
       	  bzero((char *) &buffer, sizeof(buffer)); 

	  char filesize[100],content_len[100];
      
	  sprintf (filesize, "%d", len);

          strcat (content_len, "Content-Length:");

          strcat (content_len, filesize);

          strcat (content_len, "\n\n");

	while(1)
	{

	   read(new_socket, buffer, 2048);

	   printf("%s",buffer );

	   bzero((char *) &buffer, sizeof(buffer));

	   send(new_socket , json , strlen(json),0); 

	   send(new_socket , test_buffer , strlen(test_buffer),0); 
	  // send(new_socket , image , strlen(image),0); 

	   //send(new_socket , content_len , strlen(content_len),0); 

	  // send(new_socket , buffer_1 , strlen(buffer_1),0); 

	  memset(buffer,0,strlen(buffer));
	}
	  free(buffer_1);
	  fclose(file); 
	  close(new_socket);
	  close(server_fd);	   
	    //return 0; 
	}
}
