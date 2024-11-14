#ifndef IMAGE_ROTATION_H_
#define IMAGE_ROTATION_H_

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define CHANNEL_NUM 1

/********************* [ Helpful Macro Definitions ] **********************/
#define MIN_PORT 1024      // Minimum port number, < 1024 is protected
#define MAX_PORT 65535     // Maximum port number supported by OS
#define MAX_THREADS 100    // Maximum number of threads
#define MAX_QUEUE_LEN 100  // Maximum queue length
#define INVALID -1  // Reusable int for marking things as invalid or incorrect
#define INVALID_FLAG 99999  // Reusable flag to marking a flag as invalid
#define BUFF_SIZE 1024      // Maximum file path length to make things easier
#define BUFFER_SIZE 1028
#define DATABASE_SIZE 1028
#define LOG_FILE_NAME "server_log"  // Standardized log file name

/********************* [ Helpful Typedefs        ] ************************/

typedef struct request_queue {
  int file_size;
  int file_descriptor;
  char *buffer;
} request_t;

typedef struct request_details {
  char buffer[1028];
  long filelength;
  char file_name[1028];
} request_details_t;

typedef struct packet {
  unsigned int size;
} packet_t;

typedef struct database_entry {
  char file_name[1028];
  int file_size;
  char *buffer;
} database_entry_t;

/********************* [ Function Prototypes       ] **********************/
void *dispatch(void *arg);
void *worker(void *args);
void loadDatabase(char *path);
void LogPrettyPrint(FILE *to_write, int threadId, int requestNumber,
                    char *file_name, int file_size);
int image_match(char *input_image, int size, database_entry_t *entry);

#endif