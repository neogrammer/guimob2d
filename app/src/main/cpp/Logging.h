//
// Created by jlhar on 3/15/2026.
//

#ifndef RAYMOB1_LOGGING_H
#define RAYMOB1_LOGGING_H
#include <android/log.h>
#define log(msg) __android_log_print(ANDROID_LOG_INFO, "info", "Hello from native code")
#endif //RAYMOB1_LOGGING_H
