#ifndef QUEUE_M_H
#define QUEUE_M_H

#include <queue>
#include <mutex> 
#include "types.h"

extern std::queue<msg> internal_msg_q;  // declare the queue here
extern std::mutex queue_mutex; // declare the mutex here


#endif // QUEUE_M_H
