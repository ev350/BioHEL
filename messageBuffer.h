#ifndef _MESSAGE_BUFFER_H_
#define _MESSAGE_BUFFER_H_
 
#define GRANULARITY 500000
 
#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "JVector.h"
 
using namespace std;
 
class messageBuffer {
    JVector < char *>buffer;
    int currentSize;
    int enabled;
    int dumpToFile;
    bool ignoreMessage;
    FILE *fp;
 
    public:
    messageBuffer() {
        char *temp = new char[GRANULARITY];
        temp[0] = 0;
        buffer.addElement(temp);
        currentSize = 0;
        enabled = 0;
        dumpToFile = 0;
        ignoreMessage=0;
    } 
 
    ~messageBuffer() {
        flushBuffer();
        delete buffer.elementAt(0);
        buffer.removeAllElements();
    }
 
    void ignoreMessages() {
        ignoreMessage = true;
    }
    void allowMessages() {
        ignoreMessage = false;
    }
 
    void setFile(char *fileName) {
        fp = fopen(fileName, "w");
        if (!fp) {
            fprintf(stderr, "Could not open log file\n");
            exit(1);
        }
        dumpToFile = 1;
    }
 
    inline void flushBuffer() {
        while (buffer.size() > 1) {
            char *temp = buffer.elementAt(0);
            if(dumpToFile) {
                fprintf(fp,"%s",temp);
                fflush(fp);
            } else {
                ::printf("%s", temp);
                fflush(stdout);
            }
            delete temp;
            buffer.removeElementAt(0);
        }
        char *temp = buffer.elementAt(0);
        if(dumpToFile) {
            fprintf(fp,"%s",temp);
            fflush(fp);
        } else {
            ::printf("%s", temp);
            fflush(stdout);
        }
        temp[0] = 0;
        currentSize = 0;
    }
 
 
    inline void addMessage(char *message) {
        if (!enabled) {
            if(dumpToFile) {
                fprintf(fp,"%s",message);
                fflush(fp);
            } else {
                ::printf("%s", message);
                fflush(stdout);
            }
            return;
        }
 
        int length = strlen(message);
        while (currentSize + length >= GRANULARITY) {
            strncat(buffer.lastElement(), message,
                    GRANULARITY - currentSize - 1);
            message += GRANULARITY - currentSize - 1;
            length -= GRANULARITY - currentSize - 1;
            char *temp = new char[GRANULARITY];
            temp[0] = 0;
            buffer.addElement(temp);
            currentSize = 0;
        }
        strcat(buffer.lastElement(), message);
        currentSize += length;
    }
 
    inline void printf(const char *fmt, ...) {
 
        if(ignoreMessage)
            return;
 
 		va_list ap;

        va_start(ap, fmt);
        int n = vsnprintf(NULL, 0, fmt, ap);
        va_end(ap);
 
        if(n < 0) {
            perror("vsnprintf failed");
            exit(1);
        }
 
        char buf[++ n];
 
        va_start(ap, fmt);
        vsnprintf(buf, n, fmt, ap);
        va_end(ap);
 
        addMessage(buf);
    }
 
    inline void enable() {
        enabled = 1;
    }
 
    inline void disable() {
        flushBuffer();
        enabled = 0;
    }
};
 
#endif