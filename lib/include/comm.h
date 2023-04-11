#pragma once
#ifndef COMM_H
#define COMM_H

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define LOG(format, ...)                                                       \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout,                                                              \
          ANSI_COLOR_YELLOW "LOG: [%s, %d, %s]\n" format ANSI_COLOR_RESET,     \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define INFO(format, ...)                                                      \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout,                                                              \
          ANSI_COLOR_BLUE "INFO: [%s, %d, %s]\n" format ANSI_COLOR_RESET,      \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define WARNING(format, ...)                                                      \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout,                                                              \
          ANSI_COLOR_RED "WARNING: [%s, %d, %s]\n" format ANSI_COLOR_RESET,      \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define ERROR(exitcode, format, ...)                                           \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stderr,                                                              \
          ANSI_COLOR_RED "ERROR: [%s, %d, %s]\n" format ANSI_COLOR_RESET,      \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);                        \
  exit(exitcode);

#define POSIX_ERROR(msg, errno)                                                \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stderr,                                                              \
          ANSI_COLOR_RED "ERROR: [%s, %d, %s]\n%s: %s\n" ANSI_COLOR_RESET,     \
          __FILE__, __LINE__, __func__, msg, strerror(errno));                 \
  exit(EXIT_FAILURE);

#endif /* COMM_H */
