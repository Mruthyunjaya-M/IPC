#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#define SOCKET_NAME "/mnt/Example_Server"
#define BUFFER_SIZE 64

int main(void)
{
    struct sockaddr_un socket_detalis;
    uint8_t ui8RetVal = 0;
    uint16_t ui16ConnectionSocket = 0;
    uint16_t ui16Result = 0;
    uint32_t ui32Data = 0;
    int8_t caBuffer[128]={0};

    /*In case the Any socket available in the same name, Removing the socket */
    unlink(SOCKET_NAME);

    /* Create socket. */
    ui16ConnectionSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if( 0 < ui16ConnectionSocket )
    {
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("socket created\n");
    printf("File descripter for socket %d\n", ui16ConnectionSocket );

    /*Initilize the sockaddr_un structure*/
    memset(&socket_detalis, 0, sizeof(struct sockaddr_un));
    socket_detalis.sun_family = AF_UNIX;
    strncpy( socket_detalis.sun_path, SOCKET_NAME, (sizeof(socket_detalis.sun_path) - 1) );

    /* Bind socket */
    ui8RetVal = bind(ui16ConnectionSocket, (const struct sockaddr *) &socket_detalis,
            sizeof(struct sockaddr_un));

    if ( 0 < ui8RetVal )
    {
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }    

    printf("bind is succeed\n");

    /* Prepare for accepting connections. */
    ui8RetVal = listen(ui16ConnectionSocket, 10);
    if ( 0 < ui8RetVal )
    {
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        /* Wait for incoming connection. */
        printf("Waiting on accept() sys call\n");
        ui32Data = accept(ui16ConnectionSocket, NULL, NULL);
        if ( 0 < ui32Data )
        {
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("Accepted from client\n");
        printf("Descriptor for reading data\n");

        /*Prepare the buffer to recv the data*/
        memset(caBuffer, 0, BUFFER_SIZE );   

        while(1)
        {

            /*Server is blocked here. Waiting for the data to arrive from client
             * 'read' is a blocking system call*/
            printf("Waiting for data from the client\n");
            ui8RetVal = read(ui32Data, caBuffer, BUFFER_SIZE);
            if ( 0 < ui8RetVal )
            {
                printf("Error: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            printf("Received data from client %s\n", caBuffer );

            /* Received Data processing from cloient 
             * Perform what ever algorithm what ever want to do */



            /* quet if Client send q */
            if( (*caBuffer == 'q') && (ui8RetVal == 1)) 
            {
                break;
            }
        }

        sprintf(caBuffer, "This string received from server");
        ui8RetVal = write(ui32Data, caBuffer, BUFFER_SIZE);
        if ( 0 < ui8RetVal )
        {
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* Close File descripter */
        close(ui32Data);
    }

    /*close the socket*/
    close(ui16ConnectionSocket);
    printf("connection closed..\n");

    /* Server should release resources before getting terminated.
     * Unlink the socket. */

    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}
