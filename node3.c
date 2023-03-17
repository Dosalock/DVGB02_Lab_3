#include <stdio.h>

#include "sim_engine.h"

#define NODES 4
extern int TRACE;
extern int YES;
extern int NO;


struct distance_table {
  int costs[4][4];
} dt3, temp3;

void printdt3(struct distance_table *dtptr);

/* Students to write the following two routines, and maybe some others */
static int min(int a, int b)
{
    return a < b ? a : b;
}

static void send_pkt()
{
    struct rtpkt pkt;
    pkt.sourceid = 3;

    for (int i = 0; i < NODES; i++) {
        if (i != 3) {
            pkt.destid = i;
            for (int j = 0; j < NODES; j++) {
                pkt.mincost[j] = dt3.costs[j][j];
            }
            tolayer2(pkt);
        }
    }
}

void rtinit3() {
  /* TODO */
  //memset(&dt3, 9, sizeof(dt3));
  for(int i=0; i< NODES; i++){
    for(int j = 0; j<NODES; j++){
      dt3.costs[i][j] = 999;
    } 
  } 
  dt3.costs[0][0] = 7;
  //dt3.costs[3][1] = inf;
  dt3.costs[2][2] = 2;
  //dt3.costs[3][3] = 0; 
    
  /* Send distance vector to other nodes */
  send_pkt();
  printdt3(&dt3);
}


void rtupdate3(struct rtpkt *rcvdpkt) {
  /* TODO */
  int sourceid = rcvdpkt->sourceid;

    /* Update the distance table with new mincost from the received packet */
    for (int i = 0; i < NODES; i++) {
        temp3.costs[sourceid][i] = rcvdpkt->mincost[i];
    }

    dt3.costs[0][2] = dt3.costs[2][2] + temp3.costs[2][0];
    dt3.costs[1][0] = dt3.costs[0][0] + temp3.costs[0][1];
    dt3.costs[2][0] = dt3.costs[0][0] + temp3.costs[0][2];
    dt3.costs[1][2] = dt3.costs[2][2] + temp3.costs[2][1];

    //printdt3(&dt3);
}

void printdt3(struct distance_table *dtptr) {
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
}







