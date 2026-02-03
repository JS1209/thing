#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "global.h"

#define RULE_BUFFER_SIZE 1024
#define RULE_PORT 5000
#define RULE_IP "127.0.0.1"
#define RULE_INTERFACE INADDR_ANY
#define RULE_IPVX AF_INET
#define RULE_ALL_INTERFACES 1

#endif // GLOBAL_H