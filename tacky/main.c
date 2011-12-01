#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pcap.h>
#include <time.h>
#include <netinet/if_ether.h>
#define BUFSIZE 256
#define FILTER_EXP_SIZE 256

void callback(u_char *useless, const struct pcap_pkthdr* header, const u_char* packet){
  int i;
  struct ether_header *eptr; /* net/ethernet.h */
  u_char *ptr; /* printing out hardware header info */

  // Received time
//  printf("%p\n", ((const time_t*)header->ts.tv_sec)); /* error on Mac OS X 10.7 */
  
  // Check packet type
  eptr = (struct ether_header *)packet;
  if(ntohs(eptr->ether_type) == ETHERTYPE_IP){
    printf("IP ");
  }else if(ntohs(eptr->ether_type) == ETHERTYPE_ARP){
    printf("ARP ");
  }else{
    printf("OTHER ");
  }

  // Source address
  ptr = eptr->ether_shost;
  i = ETHER_ADDR_LEN;
  do{
    printf("%s%02x",(i == ETHER_ADDR_LEN) ? "" : ":",*ptr++);
  }while(--i > 0);
  printf(" > ");

  // Destination address
  ptr = eptr->ether_dhost;
  i = ETHER_ADDR_LEN;
  do{
    printf("%s%02x",(i == ETHER_ADDR_LEN) ? "" : ":",*ptr++);
  }while(--i > 0);
  
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