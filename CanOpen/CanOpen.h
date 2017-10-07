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
#define COB_ID_MASK 0x1FFFFFFFL // 29-bit mask - what is valid for a 29-bit COBID

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


/* SDO Error codes
  Abort code (hexa)
  0503 0000 Toggle bit not alternated
  0504 0000 SDO protocol timed out
  0504 0001 Client/server command specifier not valid or unknown
  0504 0002 Invalid block size (block mode only)
  0504 0003 Invalid sequence number (block mode only)
  0504 0004 CRC error (block mode only)
  0504 0005 Out of memory
  0601 0000 Unsupported access to an object
  0601 0001 Attempt to read a write only object
  0601 0002 Attempt to write a read only object
  0602 0000 Object does not exist in the object dictionary
  0604 0041 Object cannot be mapped to the PDO
  0604 0042 The number and length of the objects to be mapped whould exeed PDO length
  0604 0043 General parameter incompatibility reason
  0604 0047 General internal incompatibility in the device
  0606 0000 Access failed due to a hardware error
  0607 0010 Data type does not match, length of service parameter does not match
  0607 0012 Data type does not match, length of service parameter too high
  0607 0013 Data type does not match, length of service parameter too low
  0609 0011 Sub-index does not exist.
  0609 0030 Value range of parameter exceeded (only for write access)
  0609 0031 Value of parameter written too high
  0609 0032 Value of parameter written too low
  0609 0036 Maximum value is less than minimum value
  0800 0000 General error
  0800 0020 Data cannot be transferred or stored to the application
  0800 0021 Data cannot be transferred or stored to the application because of local control
  0800 0022 Data cannot be transferred or stored to the application because ofthe present device state
  0800 0023 Object dictionary dynamic generation fails or no object dictionary is present.
*/
#endif // CANOPEN_INCLUDED
