// wpcapEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

#include <pcap.h>

#define MAX_PRINT 80
#define MAX_LINE 16

#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "Ws2_32.lib")

void usage();


int main(int argc, char **argv)
{
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *source = NULL;
	char *ofilename = NULL;
	char *filter = NULL;
	int i;
	pcap_dumper_t *dumpfile;
	struct bpf_program fcode;
	bpf_u_int32 NetMask;
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	if (argc == 1)
	{
		usage();
		return -1;
	}

	/* Parse parameters */
	for (i = 1; i < argc; i += 2)
	{
		switch (argv[i][1])
		{
		case 's':
		{
					source = argv[i + 1];
		};
			break;

		case 'o':
		{
					ofilename = argv[i + 1];
		};
			break;

		case 'f':
		{
					filter = argv[i + 1];
		};
			break;
		}
	}

	char *dev;
	char *net;
	char *mask;

	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	
	int ret;
	struct pcap_pkthdr hdr;
	struct in_addr net_addr, mask_addr;
	struct ether_header *eptr;
	const u_char *packet;


	pcap_t *pcd;  // packet capture descriptor

	// 사용중인 디바이스 이름을 얻어온다. 
	dev = pcap_lookupdev(errbuf);
	if (dev == NULL)
	{
		printf("%s\n", errbuf);
		exit(1);
	}
	printf("DEV : %s\n", dev);

	// 디바이스 이름에 대한 네트웍/마스크 정보를 얻어온다. 
	ret = pcap_lookupnet(dev, &netp, &maskp, errbuf);
	if (ret == -1)
	{
		printf("%s\n", errbuf);
		exit(1);
	}

	// 네트웍/마스트 정보를 점박이 3형제 스타일로 변경한다. 
	net_addr.s_addr = netp;
	net = inet_ntoa(net_addr);
	printf("NET : %s\n", net);

	mask_addr.s_addr = maskp;
	mask = inet_ntoa(mask_addr);
	printf("MSK : %s\n", mask);
	printf("=======================\n");


	// open a capture from the network
	if (source != NULL)
	{
		if ((fp = pcap_open_live(dev,		// name of the device
			65536,								// portion of the packet to capture. 
			// 65536 grants that the whole packet will be captured on all the MACs.
			1,									// promiscuous mode (nonzero means promiscuous)
			1000,								// read timeout
			errbuf								// error buffer
			)) == NULL)
		{
			fprintf(stderr, "\nUnable to open the adapter.\n");
			return -2;
		}
	}
	else usage();

	if (filter != NULL)
	{
		// We should loop through the adapters returned by the pcap_findalldevs_ex()
		// in order to locate the correct one.
		//
		// Let's do things simpler: we suppose to be in a C class network ;-)
		NetMask = 0xffffff;

		//compile the filter
		if (pcap_compile(fp, &fcode, filter, 1, NetMask) < 0)
		{
			fprintf(stderr, "\nError compiling filter: wrong syntax.\n");

			pcap_close(fp);
			return -3;
		}

		//set the filter
		if (pcap_setfilter(fp, &fcode)<0)
		{
			fprintf(stderr, "\nError setting the filter\n");

			pcap_close(fp);
			return -4;
		}

	}

	//open the dump file
	if (ofilename != NULL)
	{
		dumpfile = pcap_dump_open(fp, ofilename);

		if (dumpfile == NULL)
		{
			fprintf(stderr, "\nError opening output file\n");

			pcap_close(fp);
			return -5;
		}
	}
	else usage();

	//start the capture
	while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
	{

		if (res == 0)
			/* Timeout elapsed */
			continue;

		//save the packet on the dump file
		pcap_dump((unsigned char *)dumpfile, header, pkt_data);

	}

	pcap_close(fp);
	pcap_dump_close(dumpfile);

	return 0;
}


void usage()
{

	printf("\npf - Generic Packet Filter.\n");
	printf("\nUsage:\npf -s source -o output_file_name [-f filter_string]\n\n");
	exit(0);
}
