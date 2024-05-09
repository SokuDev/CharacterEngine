//
// Created by PinkySmile on 01/05/24.
//

#ifndef CHARACTERENGINE_LOG_HPP
#define CHARACTERENGINE_LOG_HPP


#include <cstdio>

#define log(fmt, ...) do { printf(fmt, ##__VA_ARGS__); fprintf(file, fmt, ##__VA_ARGS__); fflush(file); } while (0)

extern FILE *file;


#endif //CHARACTERENGINE_LOG_HPP
