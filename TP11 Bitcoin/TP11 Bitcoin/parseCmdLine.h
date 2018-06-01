#pragma once

#include "parser.h"

#define OPTION_IDENTIFIER '-'
#define ERRORPARSE -1

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData);
