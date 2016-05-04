#pragma once

#define STATE_EMPTY 0
#define STATE_LIST_START 1
#define STATE_LIST_END 2
#define STATE_NUMBER 3
#define STATE_DIV 4
#define STATE_COMMENT 5
#define STATE_ADD 6
#define STATE_SUBTRACT 7
#define STATE_ID 8
#define STATE_ERROR -1
#define STATE_EOF -2