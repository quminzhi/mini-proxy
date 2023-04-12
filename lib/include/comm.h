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

#ifdef DEBUG
#define LOG LOG_DEBUG
#define INFO INFO_DEBUG
#define WARNING WARNING_DEBUG
#define ERROR ERROR_DEBUG
#define POSIX_ERROR POSIX_ERROR_DEBUG
#else
#define LOG LOG_PUBLIC
#define INFO INFO_PUBLIC
#define WARNING WARNING_PUBLIC
#define ERROR ERROR_PUBLIC
#define POSIX_ERROR POSIX_ERROR_PUBLIC
#endif

#define LOG_PUBLIC(format, ...)                                                \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout, ANSI_COLOR_YELLOW "LOG: " format ANSI_COLOR_RESET,           \
          __VA_ARGS__);

#define INFO_PUBLIC(format, ...)                                               \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout, ANSI_COLOR_BLUE "INFO: " format ANSI_COLOR_RESET,            \
          __VA_ARGS__);

#define WARNING_PUBLIC(format, ...)                                            \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout, ANSI_COLOR_RED "WARNING: " format ANSI_COLOR_RESET,          \
          __VA_ARGS__);

#define ERROR_PUBLIC(exitcode, format, ...)                                    \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stderr, ANSI_COLOR_RED "ERROR: " format ANSI_COLOR_RESET,            \
          __VA_ARGS__);                                                        \
  exit(exitcode);

#define POSIX_ERROR_PUBLIC(msg, errno)                                         \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stderr, ANSI_COLOR_RED "ERROR: %s: %s\n" ANSI_COLOR_RESET, msg,      \
          strerror(errno));                                                    \
  exit(EXIT_FAILURE);

#define LOG_DEBUG(format, ...)                                                 \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout,                                                              \
          ANSI_COLOR_YELLOW                                                    \
          "LOG: [%s, %d, %s]\n" ANSI_COLOR_RESET ANSI_COLOR_YELLOW format      \
              ANSI_COLOR_RESET,                                                \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define INFO_DEBUG(format, ...)                                                \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout,                                                              \
          ANSI_COLOR_BLUE                                                      \
          "INFO: [%s, %d, %s]\n" ANSI_COLOR_RESET ANSI_COLOR_BLUE format       \
              ANSI_COLOR_RESET,                                                \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define WARNING_DEBUG(format, ...)                                             \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stdout,                                                              \
          ANSI_COLOR_RED                                                       \
          "WARNING: [%s, %d, %s]\n" ANSI_COLOR_RESET ANSI_COLOR_RED format     \
              ANSI_COLOR_RESET,                                                \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define ERROR_DEBUG(exitcode, format, ...)                                     \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stderr,                                                              \
          ANSI_COLOR_RED                                                       \
          "ERROR: [%s, %d, %s]\n" ANSI_COLOR_RESET ANSI_COLOR_RED format       \
              ANSI_COLOR_RESET,                                                \
          __FILE__, __LINE__, __func__, ##__VA_ARGS__);                        \
  exit(exitcode);

#define POSIX_ERROR_DEBUG(msg, errno)                                          \
  fflush(stdout);                                                              \
  fflush(stderr);                                                              \
  fprintf(stderr,                                                              \
          ANSI_COLOR_RED "ERROR: [%s, %d, %s]\n%s: %s\n" ANSI_COLOR_RESET,     \
          __FILE__, __LINE__, __func__, msg, strerror(errno));                 \
  exit(EXIT_FAILURE);
#endif /* COMM_H */
