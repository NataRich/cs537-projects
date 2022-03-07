#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int
sys_settickets(void)//(int pid, int n_tickets)
{
  int pid;
  int ntickets;
  if (argint(0,&pid) < 0 || argint(1,&ntickets) < 0)
    return -1;
  //add a funciton in proc.c to search through ptable for pid
  struct proc* p = findproc(pid);
  if (p == 0) {
    return -1;
  } else {
    p->ticket = ntickets;
    return 0;
  } 
}

void
sys_srand(void)//(uint seed)
{
  extern uint rseed;
  uint seed;
  if (argint(0, &seed) < 0) {
    return;
  }
  rseed = seed;
}
int
sys_getpinfo(void)//(struct pstat *)
{
  struct pstat *ps;
  if (argptr(0, (void*)&ps, sizeof(*ps)) < 0)
    return -1;
  //need to call helper function in proc.c
  pinfo(ps);
  
  return 0;  
}
