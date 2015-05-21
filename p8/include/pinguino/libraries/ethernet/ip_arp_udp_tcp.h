/*********************************************
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 * initially for Arduino environment
 * adapted to Pinguino Project by Andre Gentric - July 2014
 *
 *********************************************/
//@{
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H

// you must call this function once before you use any of the other functions:
extern void init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip,uint8_t wwwp);
//
extern void www_server_reply(uint8_t *buf,uint16_t dlen);

extern uint8_t eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len);
extern uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len);
extern void make_arp_answer_from_request(uint8_t *buf);
extern void make_echo_reply_from_request(uint8_t *buf,uint16_t len);
extern void make_udp_reply_from_request(uint8_t *buf,char *data,uint8_t datalen,uint16_t port);


extern void make_tcp_synack_from_syn(uint8_t *buf);
extern void init_len_info(uint8_t *buf);
extern uint16_t get_tcp_data_pointer(void);
extern uint16_t fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s);
extern void make_tcp_ack_with_data_noflags(uint8_t *buf,uint16_t dlen);
extern void make_tcp_ack_from_any(uint8_t *buf,int16_t datlentoack,uint8_t addflags);
//extern void make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen);
extern void make_arp_request(uint8_t *buf, uint8_t *server_ip);
extern uint8_t arp_packet_is_myreply_arp ( uint8_t *buf );
extern void tcp_client_send_packet(uint8_t *buf,uint16_t dest_port, uint16_t src_port, uint8_t flags, uint8_t max_segment_size, 
uint8_t clear_seqck, uint16_t next_ack_num, uint16_t dlength, uint8_t *dest_mac, uint8_t *dest_ip);
extern uint16_t tcp_get_dlength ( uint8_t *buf );


#endif /* IP_ARP_UDP_TCP_H */
//@}
