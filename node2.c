#include <stdio.h>

#include "sim_engine.h"

#define NODES 4
extern int TRACE;
extern int YES;
extern int NO;


struct distance_table {
  int costs[4][4];
} dt2;

void printdt2(struct distance_table *dtptr);

/* Students to write the following two routines, and maybe some others */
static int min(int a, int b)
{
    return a < b ? a : b;
}

static void send_pkt()
{
    struct rtpkt pkt;
    pkt.sourceid = 2;

    for (int i = 0; i < NODES; i++) {
        if (i != 2) {
            pkt.destid = i;
            for (int j = 0; j < NODES; j++) {
                pkt.mincost[j] = dt2.costs[j][j];
            }
            tolayer2(pkt);
        }
    }
}

void rtinit2() {
  /* TODO */
  //memset(&dt2, 9, sizeof(dt2));
  for(int i=0; i< NODES; i++){
    for(int j = 0; j<NODES; j++){
      dt2.costs[i][j] = 999;
    } 
  } 
  dt2.costs[0][0] = 3;
  dt2.costs[1][1] = 1;
  //dt2.costs[2][2] = 0;
  dt2.costs[3][3] = 2; 
    
 /* Send distance vector to other nodes */
  send_pkt();
  printdt2(&dt2);
}

void rtupdate2(struct rtpkt *rcvdpkt) {
  /* TODO */
  int sourceid = rcvdpkt->sourceid;

    /* Update the distance table with new mincost from the received packet */
    /*for (int i = 0; i < NODES; i++) {
        dt2.costs[i][sourceid]= rcvdpkt->mincost[i];
    }*/

    /* Calculate the new minimum cost to each node */
    int mincost[NODES];
    for (int i = 0; i < NODES; i++) {
        mincost[i] = rcvdpkt->mincost[i];
        for (int j = 0; j < NODES; j++) {
            mincost[i] = min(mincost[i], dt2.costs[i][j]);
        }
    }

    /* Update the distance table and send distance vector to other nodes if there is a change in the minimum cost */
    int is_updated = 0;
    for (int i = 0; i < NODES; i++) {
        int newcost = mincost[sourceid] + dt2.costs[i][sourceid];
        if (newcost < dt2.costs[i][sourceid]) {
            dt2.costs[i][sourceid] = newcost;
            is_updated = 1;
        }
    }
    if (is_updated  && sourceid != 2) {
        send_pkt();
    }
}

void printdt2(struct distance_table *dtptr) {
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







