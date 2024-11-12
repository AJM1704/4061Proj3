#include "../include/client.h"



int port = 0;

pthread_t worker_thread[100];
int worker_thread_id = 0;
char output_path[1028];

processing_args_t req_entries[100];

/* TODO: implement the request_handle function to send the image to the server and recieve the processed image
* 1. Open the file in the read-binary mode (i.e. "rb" mode) - Intermediate Submission
* 2. Get the file length using the fseek and ftell functions - Intermediate Submission
* 3. set up the connection with the server using the setup_connection(int port) function - Intermediate Submission
* 4. Send the file to the server using the send_file_to_server(int fd, FILE *file, int size) function - Intermediate Submission
* 5. Receive the processed image from the server using the receive_file_from_server(int socket, char *file_path) function
* 6. receive_file_from_server saves the processed image in the output directory, so pass in the right directory path
* 7. Close the file
*/
void * request_handle(void * img_file_path)
{
    FILE *file;
    long file_size;
    char *buffer;
    int socket;
    struct sockaddr_in server_addr;

    //1. Open the file in the read-binary mode (i.e. "rb" mode)
    file = fopen((char *) img_file_path, "rb");
    if(file == NULL) {
        perror("File open failed");
        return NULL;
    }

    //2. Get the file length using the fseek and ftell functions
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store the file content
    buffer = (char *)malloc(file_size);
    if (buffer == NULL)
    {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file content into the buffer
    fread(buffer, 1, file_size, file);  
    fclose(file);

    //3. set up the connection with the server using the setup_connection(int port) function
    socket = setup_connection(port);
    if (socket < 0)
    {
        free(buffer);
        return NULL;
    }

    //4. Send the file to the server using the send_file_to_server(int fd, FILE *file, int size) function
    if (send_file_to_server(socket, buffer, file_size) < 0)
    {
        perror("File send failed");
        close(socket);
        free(buffer);
        return NULL;
    }

    //5. & 6. Receive the processed image from the server using the receive_file_from_server(int socket, char *file_path)
    if (receive_file_from_server(socket, output_path) < 0)
    {
        perror("File receive failed");
        close(socket);
        free(buffer);
        return NULL;
    }

    //7. Close the file
    close(socket);
    free(buffer);
    return NULL;
}

/* Directory traversal function is provided to you. */
void directory_trav(char * img_directory_path)
{
    char dir_path[BUFF_SIZE]; 
    strcpy(dir_path, img_directory_path);
    struct dirent *entry; 
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        perror("Opendir ERROR");
        exit(0);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".DS_Store") != 0)
        {
            sprintf(req_entries[worker_thread_id].file_name, "%s/%s", dir_path, entry->d_name);
            printf("New path: %s\n", req_entries[worker_thread_id].file_name);
            pthread_create(&worker_thread[worker_thread_id], NULL, request_handle, (void *) req_entries[worker_thread_id].file_name);
            worker_thread_id++;
        }
    }
    closedir(dir);
    for(int i = 0; i < worker_thread_id; i++)
    {
        pthread_join(worker_thread[i], NULL);
    }
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: ./client <directory path> <Server Port> <output path>\n");
        exit(-1);
    }
    /*TODO:  Intermediate Submission
    * 1. Get the input args --> (1) directory path (2) Server Port (3) output path
    */

    /*TODO: Intermediate Submission
    * Call the directory_trav function to traverse the directory and send the images to the server
    */
    return 0;  
}