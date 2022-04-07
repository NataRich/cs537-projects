#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "mmu.h"

#define MAXTH 32

typedef struct __thread_metadata
{
  int idle;
  void *istack; // Address of pointer to be freed
  void *ustack; // Actual user stack addrses (DONT FREE)
                // used as the thread's unique identifier
} th_meta;

th_meta ths[MAXTH]; // Keep at most 32 threads' info

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  void *istack = malloc(PGSIZE * 2); // Allocate 2 pages for page-alignment
  void *ustack = istack;
  if ((uint)istack % PGSIZE != 0) // Check if page-aligned
    ustack = istack + (PGSIZE - (uint)istack % PGSIZE);

  // Keep track of thread memory metadata to prevent potential memory leaks
  // due to page-alignment
  for(int i = 0; i < MAXTH; i++){
    th_meta th = ths[i];
    // Found an available entry to store thread's metadata
    if (th.idle == 1){
      th.idle = 0; // Indiate this entry is in use
      th.istack = istack; // Save the address to be freed later in join
      th.ustack = ustack; // Save the address as the unique identifier
      break;
    }
  }

  return clone(start_routine, arg1, arg2, ustack);
}

int thread_join(void)
{ 
  void *ustack;
  int rtn = join(&ustack);

  // Cleanup allocated memory to prevent memory leaks
  for(int i = 0; i < MAXTH; i++){
    th_meta th = ths[i];
    // Found the thread's info
    if(th.idle == 0 && th.ustack == ustack){
      th.idle = 1; // Indiate this entry is free to use
      free(th.istack); // Free unused memory
      break;
    }
  }

  return rtn;
}

static inline int FAA(int* variable, int value)
{
    __asm__ volatile("lock; xaddl %0, %1"
      : "+r" (value), "+m" (*variable) // input + output
      : // No input-only
      : "memory"
    );
    return value;
}

void lock_init(lock_t *lock)
{
  lock->ticket = 0;
  lock->turn = 0;
}

void lock_acquire(lock_t *lock)
{
  int myturn = FAA(&lock->ticket, 1);
  
  //spin
  while(lock->turn != myturn);
}

void lock_release(lock_t *lock)
{
  lock->turn = lock->turn + 1;
}
