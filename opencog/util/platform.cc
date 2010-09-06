/*
 * opencog/util/platform.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
 *
 * Written by Gustavo Gama <moshe@metacog.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifdef __APPLE__

#include <sys/timeb.h>
#include <math.h>

#include "platform.h"

namespace opencog
{

#ifndef HAVE_STRTOK_R
#define HAVE_STRTOK_R 1

char* __strtok_r(char *s1, const char *s2, char **lasts)
{
    char *ret;

    if (s1 == NULL)
        s1 = *lasts;
    while (*s1 && strchr(s2, *s1))
        ++s1;
    if (*s1 == '\0')
        return NULL;
    ret = s1;
    while (*s1 && !strchr(s2, *s1))
        ++s1;
    if (*s1)
        *s1++ = '\0';
    *lasts = s1;
    return ret;
}

#endif /* HAVE_STRTOK_R */
} // namespace opencog
#endif

#ifdef WIN32

#include <sys/timeb.h>
#include <winsock2.h>
#include <process.h>
#include <math.h>
#include <io.h>

#include "platform.h"

using namespace opencog;

int opencog::round(float x)
{
    return ((x -(int)(x)) < 0.5 ) ? (int)x : (int)x + 1;
}

int opencog::gettimeofday(struct timeval* tp, void* tzp)
{
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    tp->tv_sec = (long) timebuffer.time;
    tp->tv_usec = timebuffer.millitm * 1000;
    /* 0 indicates that the call succeeded. */
    return 0;
}

void opencog::usleep(unsigned int useconds)
{
    // Sleep is in milliseconds
    // If 0 is passed to Sleep()
    // It skips rest of thread scheduled time
    // This is the best achievable with Millisecond
    // resolution
    Sleep((int)(useconds / 1000));
}

unsigned opencog::sleep(unsigned seconds)
{
    Sleep(seconds * 1000);
    return 0;
}

#ifndef HAVE_STRTOK_R
#define HAVE_STRTOK_R 1

char* opencog::__strtok_r(char *s1, const char *s2, char **lasts)
{
    char *ret;

    if (s1 == NULL)
        s1 = *lasts;
    while (*s1 && strchr(s2, *s1))
        ++s1;
    if (*s1 == '\0')
        return NULL;
    ret = s1;
    while (*s1 && !strchr(s2, *s1))
        ++s1;
    if (*s1)
        *s1++ = '\0';
    *lasts = s1;
    return ret;
}

#endif /* HAVE_STRTOK_R */

int opencog::__getpid(void)
{
    return _getpid();
}

double opencog::rint(double nr)
{
    double f = floor(nr);
    double c = ceil(nr);
    return (((c -nr) >= (nr - f)) ? f : c);
}

int opencog::__dup2(int fd1, int fd2)
{
    return _dup2(fd1, fd2);
}

unsigned long long opencog::atoll(const char *str)
{
    unsigned long long la = 0;
    sscanf(str, "%uL", &la);
    return la;
}

#endif // WIN32

#include <stdlib.h>
#include <unistd.h>
#include "platform.h"

size_t opencog::getMemUsage() {
    static void *old_sbrk = 0;
    void *p = sbrk(0);
    if (old_sbrk == 0 || old_sbrk > p) 
    {
        old_sbrk = p;
        return 0;
    }
    size_t diff = (size_t)p - (size_t)old_sbrk;
    old_sbrk = p;
    return diff;
}

