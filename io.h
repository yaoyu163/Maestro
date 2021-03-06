/*
 * Copyright (C) 2020  Edward LEI <edward_lei72@hotmail.com>
 *
 * The code is licensed under the MIT license
 */

#ifndef _IO_H_
#define _IO_H_


unsigned char *io_read_socket(int sockfd,
                              int *rc);

void io_write_socket(int sockfd,
                     unsigned char *bytes,
                     size_t len);

unsigned char *io_fread(char *fname,
                        size_t len);

unsigned char *io_fread_pipe(FILE *f,
                             size_t len);

char *io_fgetc(FILE *fpipe,
               int *len);

void io_send_chunk(int clifd,
                   char *data);


#endif
