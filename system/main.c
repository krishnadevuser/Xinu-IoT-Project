/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

#define MSG_MAX_LEN 64
#define ECHO_PORT 9989

// process udpclient(uint nargs, char *dest,)
// {
//     ushort dev = 0;
//     char buf[MSG_MAX_LEN];

//     struct netaddr dst;
//     struct netaddr *localhost;
//     struct netif *interface;


//     /* Allocate a new UDP device */
//     if ((ushort)SYSERR == (dev = udpAlloc()))
//     {
//         fprintf(stderr, "Client: Failed to allocate a UDP device.");
//         return SYSERR;
//     }

//     /* Look up local ip info */
//     interface = netLookup((ethertab[0].dev)->num);
//     if (NULL == interface)
//     {
//         fprintf(stderr, "Client: No network interface found\r\n");
//         return SYSERR;
//     }
//     localhost = &(interface->ip);
    
//      Change the destination to ipv4 
//     if (SYSERR == dot2ipv4(dest, &dst))
//     {
//         fprintf(stderr, "Client: Failed to convert ip address.");
//         return SYSERR;
//     }

//     /* Open the UDP device with the destination and echo port*/
//     if (SYSERR == open(dev, localhost, &dst, NULL, ECHO_PORT))
//     {
//         fprintf(stderr, "Client: Could not open the UDP device\r\n");
//         return SYSERR;
//     }

//     /* Send the message to the destination*/
//     memcpy(buf, args[2], MSG_MAX_LEN);
    
//     if(SYSERR == write(dev, buf, MSG_MAX_LEN))
//     {
//         close(dev);
//         return SYSERR;
//     }

//     /* Closee the device when done */
//     close(dev);

//     return 0;
// }

pid32 server_id;

struct ack_msg{
	uint32 ack;
	uint32 id;

};

process udpserver() {
	kprintf("udp starts ");
	struct ack_msg msg;
    int32   retval;         /* return value from sys calls  */
    uint32  localip;        /* local IP address     */
    uint32  remip;          /* remote sender's IP address   */
    uint16  remport;        /* remote sender's UDP port */
    char    buff[1500];     /* buffer for incoming reply    */
    int32   msglen;         /* length of outgoing message   */
    int32   slot;           /* slot in UDP table        */
    uint16  echoserverport = 7;  /* port number for UDP echo */
    localip = getlocalip();

    buff[0] = 'h';
     buff[1] = 'e';
      buff[2] = 'l';

    
    kprintf("My ip address is %d\n",localip);
    if (localip == SYSERR) {
        kprintf(
            "%s: could not obtain a local IP address\n");
        return 1;
    }

    /* register local UDP port */

    slot = udp_register(0, 0, echoserverport);
    if (slot == SYSERR) {
        kprintf(": could not reserve UDP port %d\n",echoserverport);
        return 1;
    }

    /* Do forever: read an incoming datagram and send it back */
    

    while (TRUE) {
    	retval = udp_sendto(slot, IP_BCAST, 33333, buff, 50);
        if (retval == SYSERR) {
            kprintf(stderr, ": udp_sendto failed\n");
            return 1;
        }
        retval = udp_recvaddr(slot, &remip, &remport, (char*)&msg,
                        sizeof(struct ack_msg), 2000);

       if (retval == TIMEOUT) {
            continue;
        } else if (retval == SYSERR) {
            kprintf(": error receiving UDP\n");
            //return 1;
        }
        //msglen = retval;
      /*  retval = udp_sendto(slot, remip, remport, buff, msglen);
        if (retval == SYSERR) {
            fprintf(stderr, ": udp_sendto failed\n");
            return 1;
        }*/
    }
    return OK;
}


/*process	main(void)
{
 
	recvclr();
    server_id = create(udpserver, 4096, 50, "udpserver",0);
    kprintf("udp resume ");
    resume(server_id);
	return OK;
}*/

process	main(void)
{

	recvclr();
	
	


	kprintf("ADC init complete\n");
	
	putc(LED,1);

	kprintf("Start main function\r\n");
	char data[4];
	unsigned int integer = 0;
	unsigned int decimal = 0;
	float voltage;
	while(1)
	{
		read(TEMP,(char*)data,4);
		unsigned int vol = *(unsigned int*)data;
		voltage = (vol*1.8/4095)*1000;

		integer = (unsigned int)voltage;
		decimal = (unsigned int)((voltage - integer)*10);
		kprintf("voltage = %d.%d  mv\r\n",integer,decimal);
		sleep(1);
	}

	//getc(TEMP);
	while(1){
		char value = getc(INPUT);
			kprintf("value = %u\n", value);
		//kprintf("data value = %f",((float)analogRead() / 4095.0f) * 1.8f);
	}

	
	//resume(timer_id);
	//resched_cntl(DEFER_STOP);

	return OK;
}
