/*
 * Copyright (C) 2020  Edward LEI <edward_lei72@hotmail.com>
 *
 * The code is licensed under the MIT license
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http_msg.h"

//#define DEBUG
#include "debug.h"


#define MAX_NUM_MSG_LINES 35  /* MAX_NUM_HEADERS + 3 */


httpmsg_t *http_parse_req(unsigned char *buf)
{
  unsigned char *line[MAX_NUM_MSG_LINES];  /* http messages lines */
  char *method;
  char *path;
  char *version;
  int major;
  int minor;
  int count, end;
  int len_body;

  char *rest;
  httpmsg_t *req;

  if (!buf) return NULL;

  end = 0;
  count = msg_split(line, &end, &len_body, buf);
  if (!count) {
    DEBS("Empty message!!!");
    return NULL;
  }

  if (count < 3) {
    DEBS("Incomplete message 1!!!");
    msg_lines_destroy(line, count);
    return NULL;
  }

  req = msg_new();

  /* request line ... */
  rest = (char *)line[0];
  method = strtok_r(rest, " ", &rest);
  path = strtok_r(NULL, " ", &rest);
  version = strtok_r(NULL, " ", &rest);
  major = version[5] - '0';
  minor = version[7] - '0';

  if (strcmp(path, "/") == 0)
    msg_set_req_line(req, method, "/index.html", major, minor);
  else
    msg_set_req_line(req, method, path, major, minor);

  /* headers */
  if (!msg_add_headers(req, line, end)) {
    msg_lines_destroy(line, count);
    return NULL;
  }

  if (len_body > 0) {
    msg_add_body(req, line[count], len_body);
    DEBSS("[PARSER] body", line[count]);
    msg_lines_destroy(line, end);
  }
  else
    msg_lines_destroy(line, count);

  return req;
}

httpmsg_t *http_parse_rep(unsigned char *buf)
{
  unsigned char *line[MAX_NUM_MSG_LINES];  /* http messages lines */
  char *version;
  int major;
  int minor;
  int code;
  char *status;
  int count, end;
  int len_body;

  char *rest;
  httpmsg_t *rep;

  if (!buf) return NULL;

  end = 0;
  count = msg_split(line, &end, &len_body, buf);
  if (count < 3) {
    DEBS("Incomplete message 1!!!");
    msg_lines_destroy(line, count);
    return NULL;
  }

  rep = msg_new();

  /* status line ... */
  rest = (char *)line[0];
  version = strtok_r(rest, " ", &rest);
  major = version[5] - '0';
  minor = version[7] - '0';
  code = atoi(strtok_r(NULL, " ", &rest));
  status = strtok_r(NULL, " ", &rest);

  msg_set_rep_line(rep, major, minor, code, status);

  /* headers */
  if (!msg_add_headers(rep, line, end)) {
    msg_lines_destroy(line, count);
    return NULL;
  }

  if (len_body > 0) {
    msg_add_body(rep, line[count], len_body);
    DEBSS("[PARSER] body", line[count]);
    msg_lines_destroy(line, end);
  }
  else
    msg_lines_destroy(line, count);

  return rep;
}

httpmsg_t *http_parse_headers(unsigned char *buf)
{
  unsigned char *line[MAX_NUM_MSG_LINES];
  int count, end;
  int len_body;

  httpmsg_t *hdrs;

  end = 0;
  count = msg_split(line, &end, &len_body, buf);
  if (count < 3) {
    DEBS("Incomplete message 1!!!");
    return NULL;
  }

  hdrs = msg_new();

  /* headers */
  if (!msg_add_headers(hdrs, line, end)) {
    msg_lines_destroy(line, count);
    return NULL;
  }

  msg_lines_destroy(line, count);
  return hdrs;
}
