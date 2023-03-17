#include <stdio.h>

#include "sim_engine.h"

#define NODES 4
extern int TRACE;
extern int YES;
extern int NO;


struct distance_table {
  int costs[4][4];
} dt1, temp1;

void printdt1(struct distance_table *dtptr);

/* Students to write the following two routines, and maybe some others */
static int min(int a, int b)
{
    return a < b ? a : b;
}

static void send_pkt()
{
    struct rtpkt pkt;
    pkt.sourceid = 1;

    for (int i = 0; i < NODES; i++) {
        if (i != 1) {
            pkt.destid = i;
            for (int j = 0; j < NODES; j++) {
                pkt.mincost[j] = dt1.costs[j][j];
            }
            tolayer2(pkt);
        }
    }
}

void rtinit1() {
  //memset(&dt1, 8 , sizeof(dt1));
  for(int i=0; i< NODES; i++){
    for(int j = 0; j<NODES; j++){
      dt1.costs[i][j] = 999;
    } 
  } 
  dt1.costs[0][0] = 1;
  dt1.costs[1][1] = 0;
  dt1.costs[2][2] = 1; 
  //dt1.costs[1][3] = inf; 
    
  /* Send distance vector to other nodes */
  send_pkt();
  printdt1(&dt1);
}

void rtupdate1(struct rtpkt *rcvdpkt) {
  /* TODO */
  int sourceid = rcvdpkt->sourceid;

  /* Update the distance table with new mincost from the received packet */
  for (int i = 0; i < NODES; i++) {
      temp1.costs[sourceid][i] = rcvdpkt->mincost[i];
  }
  int changed = 0;

    /* Compute new distance estimates based on received mincosts */
    for (int dest = 0; dest < NODES; dest++) {
        if (dest == 1) {
            continue;  // don't need to update distance to self
        }
        int min_cost = 999;
        for (int via = 0; via < NODES; via++) {
            int cost = dt1.costs[via][dest] + temp1.costs[sourceid][via];
            if (cost < min_cost) {
                min_cost = cost;
            }
        }
        if (min_cost != dt1.costs[sourceid][dest]) {
            dt1.costs[sourceid][dest] = min_cost;
            changed = 1;
        }
    }
    if (changed) {
        send_pkt();
        printdt1(&dt1);
    }
}

void printdt1(struct distance_table *dtptr) {
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);
}

void linkhandler1(int linkid, int newcost) {
  /* DON'T CHANGE */
}


