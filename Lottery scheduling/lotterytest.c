#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

void
spin(char *name)
{
  int i;
  printf(1, "\n%s starting\n", name);

  for(i = 0; i < 500000000; i++){
    if(i % 100000000 == 0)
      printf(1, "%s running\n", name);
  }
  printf(1, "%s done\n", name);
}

int
main(void)
{
  int pid1, pid2, pid3;

  pid1 = fork();
  if(pid1 < 0){
    printf(1, "fork for A failed\n");
    exit();
  }
  if(pid1 == 0){
    settickets(30);
    spin("A-30");
    exit();
  }

  pid2 = fork();
  if(pid2 < 0){
    printf(1, "fork for B failed\n");
    exit();
  }
  if(pid2 == 0){
    settickets(20);
    spin("B-20");
    exit();
  }

  pid3 = fork();
  if(pid3 < 0){
    printf(1, "fork for C failed\n");
    exit();
  }
  if(pid3 == 0){
    settickets(10);
    spin("C-10");
    exit();
  }

  printf(1, "\nparent: A=%d B=%d C=%d\n", pid1, pid2, pid3);
   sleep(2000);

  struct pstat ps;
  if(getpinfo(&ps) < 0){
    printf(1, "getpinfo failed\n");
    exit();
  }

  int i;
  for(i = 0; i < NPROC; i++){
    if(ps.inuse[i] &&
       (ps.pid[i] == pid1 || ps.pid[i] == pid2 || ps.pid[i] == pid3)){
      printf(1, "pid %d tickets %d ticks %d\n",
             ps.pid[i], ps.tickets[i], ps.ticks[i]);
    }
  }
  wait();
  wait();
  wait();

  exit();
}

