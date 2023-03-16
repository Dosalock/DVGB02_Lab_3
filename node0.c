#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sim_engine.h"

#define NODES 4
extern int TRACE;
extern int YES;
extern int NO;


struct distance_table {
  int costs[4][4];
} dt0, temp;

void printdt0(struct distance_table *dtptr);

/* Students to write the following two routines, and maybe some others */


static int min(int a, int b)
{
    return a < b ? a : b;
}

static void send_pkt()
{
    struct rtpkt pkt;
    pkt.sourceid = 0;

    for (int i = 0; i < NODES; i++) {
        if (i != 0) {
            pkt.destid = i;
            for (int j = 0; j < NODES; j++) {
                pkt.mincost[j] = dt0.costs[j][j];
            }
            tolayer2(pkt);
        }
    }
}

void rtinit0() {
    /* TODO: Initialize the distance table dt0 */
    //memset(&dt0, 9 , sizeof(dt0));x
    for(int i=0; i<NODES; i++){
        for(int j=0; j<NODES; j++){
        dt0.costs[i][j] = 999;
        } 
  } 
    //dt0.costs[0][0] = 0;
    dt0.costs[1][1] = 1;
    dt0.costs[2][2] = 3;
    dt0.costs[3][3] = 7;
    
    /* TODO: Send distance vector to other nodes */
    send_pkt();
    printdt0(&dt0);
}
void printdt0(struct distance_table *dtptr) {
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],dtptr->costs[3][2],dtptr->costs[3][3]);
}

void rtupdate0(struct rtpkt *rcvdpkt) {
    int sourceid = rcvdpkt->sourceid;

    /* Update the distance table with new mincost from the received packet */
    for (int i = 0; i < NODES; i++) {
        temp.costs[sourceid][i] = rcvdpkt->mincost[i];
    }
        dt0.costs[2][1] = dt0.costs[1][1] + temp.costs[2][1];
        dt0.costs[1][2] = dt0.costs[2][2] + temp.costs[2][1];
        dt0.costs[3][2] = dt0.costs[2][2] + temp.costs[2][3];
        dt0.costs[2][3] = dt0.costs[3][3] + temp.costs[2][3];
        dt0.costs[1][3] = dt0.costs[3][3] + temp.costs[3][2] + temp.costs[2][1];
        dt0.costs[3][1] = dt0.costs[1][1] + temp.costs[3][2] + temp.costs[2][1];
        /*
    if (sourceid == 1){
        
    }
    if (sourceid == 2){
        dt0.costs[2][1] = dt0.costs[1][1] + rcvdpkt->mincost[1];
        dt0.costs[1][2] = dt0.costs[2][2] + rcvdpkt->mincost[1];
        dt0.costs[3][2] = dt0.costs[2][2] + rcvdpkt->mincost[3];
        dt0.costs[2][3] = dt0.costs[3][3] + rcvdpkt->mincost[3];
    } 
    if (sourceid == 3){
        dt0.costs[1][3] = dt0.costs[3][3] + rcvdpkt->mincost[2] + 1;
        dt0.costs[3][1] = dt0.costs[1][1] + rcvdpkt->mincost[2] + 1;
    }*/
    printdt0(&dt0);
}


void linkhandler0(int linkid, int newcost) {
  /* DON'T CHANGE */
}

