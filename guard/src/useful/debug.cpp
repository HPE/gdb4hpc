/*********************************************************************************\
 * Debug handling routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "alloc.h"
#include "debug.h"
#include "defs.h"

#ifdef PERFLOG
#include "list.h"
#endif /*PERFLOG*/

// external error printing function
extern void PrintOutError(const char*, ...);

#ifdef PERFLOG
static FILE* _timer_log = NULL;
static list_t* timer_list = (list_t*)NULL;

guard_timer_t* CreateTimer(char* name) {
    guard_timer_t* timer;
    timer = (guard_timer_t*)Alloc(sizeof(guard_timer_t));
    if (timer != (guard_timer_t*)NULL) timer->name = strdup(name);

    return timer;
}

void ReleaseTimer(guard_timer_t* timer) {
    Free(timer->name);
    Free(timer);
}

list_t* CreateTimerList() {
    timer_list = NewList();

    return timer_list;
}

void ReleaseTimerList() {
    if (timer_list != (list_t*)NULL) {
        DestroyList(timer_list, ReleaseTimer);
        timer_list = NULL;
    }
}

double CalElapsedTime(struct timeval* tv_begin, struct timeval* tv_end)
/*return the duration in seconds */
{
    double ela_time;
    long ela_secs, ela_usecs;

    if (tv_end->tv_usec >= tv_begin->tv_usec) {
        ela_secs = tv_end->tv_sec - tv_begin->tv_sec;
        ela_usecs = tv_end->tv_usec - tv_begin->tv_usec;
    } else {
        ela_secs = tv_end->tv_sec - tv_begin->tv_sec - 1;
        ela_usecs = tv_end->tv_usec - tv_begin->tv_usec + 1000000;
    }

    ela_usecs += ela_secs * 1000000;
    ela_time = (double)ela_usecs / 1000000;

    return ela_time;
}

/* return 1, if l > r */
/* return 0, if l = r */
/* return -1, if l < r */
int CompareTime(struct timeval* tv_l, struct timeval* tv_r) {
    if (tv_l->tv_sec > tv_r->tv_sec) {
        return 1;
    } else if (tv_l->tv_sec < tv_r->tv_sec) {
        return -1;
    } else if (tv_l->tv_usec > tv_r->tv_usec) {
        return 1;
    } else if (tv_l->tv_usec < tv_r->tv_usec) {
        return -1;
    }

    return 0;
}

int CreateTimerLog(char* name) {
    if (_timer_log == NULL) _timer_log = fopen(name, "a");

    if (_timer_log != NULL) return 0;

    return -1;
}

int CloseTimerLog() {
    ReleaseTimers();

    if (_timer_log != NULL) {
        fclose(_timer_log);
        _timer_log = NULL;

        return 0;
    }

    return -1;
}

void ReleaseTimers() { ReleaseTimerList(); }

guard_timer_t* FindTimerByName(char* name) {
    guard_timer_t* timer = NULL;

    if (timer_list == (list_t*)NULL) return (guard_timer_t*)NULL;

    SetList(timer_list);
    while ((timer = (guard_timer_t*)GetListElement(timer_list)) !=
           (guard_timer_t*)NULL) {
        if (!strcmp(timer->name, name)) break;
    }

    return timer;
}

guard_timer_t* SetTimerStart(char* name) {
    guard_timer_t* timer = NULL;

    if (name == NULL) return (guard_timer_t*)NULL;

    if (timer_list == (list_t*)NULL) timer_list = CreateTimerList();

    timer = FindTimerByName(name);
    if (timer != (guard_timer_t*)NULL) return (guard_timer_t*)NULL;

    timer = CreateTimer(name);
    if (timer == (guard_timer_t*)NULL) return (guard_timer_t*)NULL;

    gettimeofday(&timer->tv_begin, NULL);

    AddToList(timer_list, (void*)timer);

    return timer;
}

guard_timer_t* SetTimerStop(char* name) {
    guard_timer_t* timer = NULL;

    if (name == NULL) return (guard_timer_t*)NULL;

    timer = FindTimerByName(name);
    if (timer == (guard_timer_t*)NULL) return (guard_timer_t*)NULL;

    gettimeofday(&timer->tv_end, NULL);

    return timer;
}

double TimerElapsedTime(guard_timer_t* timer) {
    return CalElapsedTime(&(timer->tv_begin), &(timer->tv_end));
}

void SetTimerStopAndLog(char* name) {
    double ela_time;
    guard_timer_t* timer = SetTimerStop(name);

    if (timer == (guard_timer_t*)NULL) return;

    ela_time = TimerElapsedTime(timer);

    if (_timer_log != NULL) {
        fprintf(_timer_log, "%s: %f\n", name, ela_time);

        fflush(_timer_log);
    } else {
        PrintOutError("%s: %f\n", name, ela_time);
    }

    RemoveFromList(timer_list, timer);
    Free(timer);
}

void SetLogInfo(char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    if (_timer_log != NULL) {
        vfprintf(_timer_log, fmt, ap);
        fflush(_timer_log);
    }
    va_end(ap);
}
#endif /*PERFLOG*/

void PrintDebugTime(char* fmt, ...) {
    char* s;
    char buf[BUFSIZ];
    va_list ap;

    struct timeval tv;
    struct tm* tm;
    time_t curtime;

    gettimeofday(&tv, NULL);
    curtime = tv.tv_sec;
    tm = localtime(&curtime);

    s = (char*)Alloc(strlen(fmt) + 2);
    strcpy(s, fmt);

    if (fmt[strlen(fmt) - 1] != '\n') strcat(s, "\n");

    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ - 1, s, ap);
    va_end(ap);

    DPRINT(">>>%s", buf);
    PrintOutError("Debug Time %d:%d:%d.%06ld %s", tm->tm_hour, tm->tm_min,
                  tm->tm_sec, tv.tv_usec, buf);

    Free(s);
}

void PrintDebugTimeLog(char* fmt, ...) {
    char* s;
    char buf[BUFSIZ];
    va_list ap;

    struct timeval tv;
    struct tm* tm;
    time_t curtime;

    char* envDir;
    const char* rootDir;
    char filePath[MAXPATHLEN + 1];
    char hostname[MAXHOSTNAMELEN + 1];
    uid_t userid;
    FILE* logFD;

    gettimeofday(&tv, NULL);
    curtime = tv.tv_sec;
    tm = localtime(&curtime);

    s = (char*)Alloc(strlen(fmt) + 2);
    strcpy(s, fmt);

    if (fmt[strlen(fmt) - 1] != '\n') strcat(s, "\n");

    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ - 1, s, ap);
    va_end(ap);

    if (gethostname(hostname, MAXHOSTNAMELEN) < 0) sprintf(hostname, "unknown");

    if ((userid = getuid()) < 0) userid = -1;

    envDir = getenv("GUARD_LOG_DIR");
    rootDir = envDir ? envDir : "/tmp";

    sprintf(filePath, "%s/guard_%u_%s.log", rootDir, userid, hostname);

    logFD = fopen(filePath, "a");

    fprintf(logFD, "Debug Time %d:%d:%d.%06ld %s", tm->tm_hour, tm->tm_min,
            tm->tm_sec, tv.tv_usec, buf);

    fclose(logFD);

    Free(s);
}
