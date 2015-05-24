/******************************************************************
 * Author: Andre Gentric inspired from Guido Socher's work (GPL V2)
 * adapted to Pinguino Project
 *****************************************************************/
#include <ethernet/enc28j60p.c>
#include <ethernet/ip_arp_udp_tcp.c>
#include <typedef.h>
#include <delayms.c>

void eth_init(uint8_t *mymac,uint8_t *myip,uint16_t myport) {
  /*initialize enc28j60*/
  enc28j60Init(mymac);
  enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
  Delayms(10);

  /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
  // LEDA=green LEDB=yellow

  // 0x880 is PHLCON LEDB=on, LEDA=on
  enc28j60PhyWrite(PHLCON,0x880);
  Delayms(500);

  // 0x990 is PHLCON LEDB=off, LEDA=off
  enc28j60PhyWrite(PHLCON,0x990);
  Delayms(500);

  // 0x880 is PHLCON LEDB=on, LEDA=on
  enc28j60PhyWrite(PHLCON,0x880);
  Delayms(500);

  // 0x990 is PHLCON LEDB=off, LEDA=off
  enc28j60PhyWrite(PHLCON,0x990);
  Delayms(500);

  // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
  enc28j60PhyWrite(PHLCON,0x476);
  Delayms(100);

  //init the ethernet/ip layer:
  init_ip_arp_udp_tcp(mymac,myip,myport);
}
