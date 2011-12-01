#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pcap.h>
#include <time.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "headers.h"
#define BUFSIZE 256
#define FILTER_EXP_SIZE 256
#define SIZE_ETHERNET 14

void callback(u_char *useless, const struct pcap_pkthdr* header, const u_char* packet){
  int i;
  const struct struct_ethernet *eh;        
  const struct struct_ip *ip;
  const struct struct_tcp *tcp;
  u_char *ptr; /* printing out hardware header info */
  u_int size_ip;
  char ip_src[20], ip_dst[20];

  // Cast
  eh = (struct struct_ethernet *)(packet);
  ip = (struct struct_ip *)(packet + SIZE_ETHERNET);
  size_ip = IP_HL(ip) * 4;
  tcp = (struct struct_tcp *)(packet + SIZE_ETHERNET + size_ip);

  // Check packet type
  if(ntohs(eh->ether_type) == ETHERTYPE_IP){
    printf("IP ");
    // Save pointer: inet_ntoa() returns internal pointer *char
    strcpy(ip_src, inet_ntoa(ip->ip_src));
    strcpy(ip_dst, inet_ntoa(ip->ip_dst));
    printf("%s.%d > %s.%d ", ip_src, tcp->th_sport, ip_dst, tcp->th_dport);
  }else if(ntohs(eh->ether_type) == ETHERTYPE_ARP){
    printf("ARP ");
  }else{
    printf("OTHER ");
  }

  // Source Mac address
  for (i = 0; i < 6; ++i) {
    printf("%02x", (int)eh->ether_shost[i]);
    if(i < 5){
      printf(":");
    }
  }
  printf(" > ");
  // Dest Mac address
  for (i = 0; i < 6; ++i) {
    printf("%02x", (int)eh->ether_dhost[i]);
    if(i < 5){
      printf(":");
    }
  }
  
  // Packet length
  printf(", length %d", header->len);
  
  printf("\n");
}

int main(int argc, char *argv[])
{
  int i;
  int opt;
  int option_status = -1;
  char *ifname;
	char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *handle; /* packet capture descripter */
  struct bpf_program pf; /* filter */
  bpf_u_int32 netmask; /* netmask */
  bpf_u_int32 addr; /* IP address */
  char filter_exp[FILTER_EXP_SIZE]; /* filter */
  struct pcap_pkthdr header; /* header for pcap */
  
	// Check command line options
	while((opt = getopt(argc, argv, "i:")) != -1){
	  switch(opt){
	    case 'i':
        ifname = optarg;
        option_status = 0;
	    	printf("Use interface: %s\n", ifname);
        break;
	  }
	}
	if(option_status != 0){
    fprintf(stderr, "Usage: %s -i interface\n", argv[0]);
    return 1;
	}
	// Set filter str
	while(optind < argc){
    strcat(filter_exp, argv[optind]);
    strcat(filter_exp, " ");
    optind++;
  }
  fprintf(stdout, "Filter: %s\n", filter_exp);

  // Set IP address and netmask
  if(pcap_lookupnet(ifname, &addr, &netmask, errbuf) == -1){
    fprintf(stderr, "Cannot get netmask: %s\n", ifname);
    addr = 0;
    netmask = 0;
  }

  // Open interface
	handle = pcap_open_live(ifname, BUFSIZE, 1, 1000, errbuf);
  if(handle == NULL){
  	fprintf(stderr, "Cannot open interface: %s\n", ifname, errbuf);
  	return 1;
  }
  
  // Compile filter
  if(pcap_compile(handle, &pf, filter_exp, 0, addr) == -1){
    fprintf(stderr, "Cannot complie filter: %s\n", filter_exp, pcap_geterr(handle));
    return 1;
  }
  
  // Set filter
  if(pcap_setfilter(handle, &pf) == -1){
    fprintf(stderr, "Cannot set filter: &s\n", filter_exp, pcap_geterr(handle));
    return 1;
  }
  
  // Receive start
  pcap_loop(handle, -1, callback, NULL);
  pcap_close(handle);
	
	return 0;
}