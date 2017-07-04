#if !defined(CANOPEN_INCLUDED)
#define CANOPEN_INCLUDED

#include <mcp_can.h>
#include <mcp_can_dfs.h>

// Enough CANOPEN stuff to define COBIDs
#define MK_COBID(ID,FN) ((ID)&0x07f | (((FN)&0xf)<<7) | (IS_EXTENDED_COBID&(ID)))
#define GET_NID(COBID) ((COBID)&0x7f)
#define GET_FN(COBID) (((COBID)>>7)&0xf)
#define EMERGENCY 1
#define TXPDO1 3
#define TXPDO2 5
#define TXPDO3 7
#define TXPDO4 9
#define RXPDO1 4
#define RXPDO2 6
#define RXPDO3 8
#define RXPDO4 10
#define TXSDO 11
#define RXSDO 12
#define NODE_GUARD 14

#define IS_EXTENDED_COBID 0x80000000L // we'll set bit 31 (of a 29-bit COBID) to mean 'Extended'

#undef DO_LOGGING
#ifdef DO_LOGGING
#define NUM_BUFFS 20
struct CanRXType {
  INT32U COBID;
  INT8U Length;
  INT8U Message[8];
  INT32U Time;
};
extern volatile CanRXType CanRXBuff[NUM_BUFFS];
void print_hex(long int v, int num_places);
void AddToDisplayBuffer(INT32U id, INT8U len, INT8U *buf);
extern volatile int Head; // updated in ISR
extern int Tail;
extern int Overflow;
#endif


extern MCP_CAN CAN; // Defined elsewhere

void SDOread(int NID, int index, INT8U subIndex);
void SDOwrite(int NID, int index, INT8U subIndex, INT32U value, INT8U valuesize);
void SYNCsend();
void NMTsend();

#endif // CANOPEN_INCLUDED
