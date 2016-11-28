#include <xinu.h>
#include <stdio.h>
#include <string.h>


#define MSG_MAX_LEN 64
#define TEMP_BUF_LEN 4
#define BBB_INIT 200

#define PORT 2222
#define BBID 1
#define DIG_INPUT 0

pid32 server_id, client_id;

uint32  remip;          /* remote sender's IP address   */
uint16  remport;        /* remote sender's UDP port */
int32   slot;
uint32 localip;


/* UDP message struture*/
struct msg_struct{
    uint32 bb_id;  /* ID of the beagle bone present in the XML file*/
    uint32 msg_id;   /* Message ID for each message sent from UDP connection*/
    uint32 dev_id;   /*Device ID  for the device connected with the BB mentioend in the XML file*/
    uint32 status;      /*Actual message or action for the device*/
};

/**************************************************************************
function name:  cleardata
description:    Clear all the Msg_struct data 
input:          pointer to Message structure
                
author:         Madhav Agrawal   
**************************************************************************/
void cleardata(struct msg_struct *msg){
    msg->bb_id = -1;
    msg->msg_id =  -1;
    msg->dev_id = -1;
    msg->status = -1;

}


/**************************************************************************
function name:  gettemp
description:    get the temperature from the adc 
input:          void
return:         return the unsigned int of the temperation          
author:         Madhav Agrawal   
**************************************************************************/
uint32 gettemp(){

    uint32 data[4];
    uint32 avg;
    int i ;
    read(TEMP,(char*)data,TEMP_BUF_LEN);
    for (i=0; i <TEMP_BUF_LEN; i++ ){
        avg+= (uint32) data[i];
    }
    avg = avg/TEMP_BUF_LEN;
    return avg;

}



/**************************************************************************
function name:  BB_init
description:    When Xinu boots first main will Initialize the UDP connection with the Edge server.The main process will wait 
for a Init packet from the edge server INIT status  = 200 with the BBB id should be set for particular board.
So if there are many BBB board are connected with the network each board is waiting for there INIT packet with BBB ID.
As in start Edge doesn't know IP address of the BBB, Edge will just send the Broadcast INIT packet for each BBB
with the BB ID set for each board. Once the BB reaceived a INIT packet with its BBID, it will ECHO the packet as a ack  
to the Edge and stored the IP and port of the Edge server and Initialized the BBB board  otherwise just ignore the 
packet. Once the BB is initialized it will create two process UDP server and UDP client. 
input:          void          
author:         Madhav Agrawal   
**************************************************************************/
void BB_init(){
    struct msg_struct msg;
    int32   retval;
    kprintf("BB_init...... \n");
    localip = getlocalip();

    kprintf("My ip address is %d \n",localip);
    while (localip == SYSERR) {
        kprintf("%s: could not obtain a local IP address retry....\n");
        localip = getlocalip();
        kprintf("My ip address is %d \n",localip);
    }

    /* register local UDP port */

    slot = udp_register(0, 0, PORT);
    while (slot == SYSERR) {
        kprintf("%s: slot register....\n");
        slot = udp_register(0, 0, PORT);
    }
    

    kprintf("initize BBB.........\n");
    while(TRUE) {
        cleardata(&msg);

        retval = udp_recvaddr(slot, &remip, &remport,(char*)&msg,sizeof(struct msg_struct ), 200000);
        if (retval == TIMEOUT) {
            continue;
        } else if (retval == SYSERR) {
            kprintf(": error receiving UDP\n");
            //return 1;
        }
        msg = (struct msg_struct) msg;
        kprintf("Messge Received msg.bb_id = %u ,msg.msg_id = %u ,msg.dev_id = %u,msg.status = %u\n",msg.bb_id,msg.msg_id,msg.dev_id,msg.status);
        if (msg.bb_id == BBID && msg.status == BBB_INIT){

            retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
            if (retval == SYSERR) {
                fprintf(stderr, ": udp_sendto failed\n");
            }
            break;
        }
    }

    kprintf("BBB INITIALIZE complete\n");

}


/**************************************************************************
function name:  udpclient process
description:    UDP client process will send the INPUT data to EDGE server in some interval or based on INPUT interupt.
If a temperature sensor is connected with the BB it will provid the temperature information to a edge in every 
1 sec. so that BB can save power and remain idle. If a INPUT button or digital sensor is connected with the BBB 
BB will triggered IRQ when a rising or falling edge is detected on the device PIN. once the iterrupt is received BB will 
read the current status of the Input pin and send the status to the Edge server using UDP connection.
input:          void         
author:         Madhav Agrawal   
**************************************************************************/
 process udpclient()
 {

     char buf[MSG_MAX_LEN];
     uint32 localip;
     int32   retval; 
     struct msg_struct msg;

     kprintf("udp client starts ");
     localip = getlocalip();

    kprintf("My ip address is %d \n",localip);
    while (localip == SYSERR) {
        kprintf("%s: could not obtain a local IP address retry....\n");
        localip = getlocalip();
        kprintf("My ip address is %d \n",localip);
    }

    while (TRUE) {

    if (DIG_INPUT) {
        msg.dev_id = INPUT;
        msg.bb_id = BBID;
        msg.status = getc(INPUT);
        msg.msg_id++;
        retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
        if (retval == SYSERR) {
            fprintf(stderr, ": udp_sendto failed\n");
        }

    } else {
        msg.dev_id = TEMP;
        msg.bb_id = BBID;
        msg.status = gettemp();
        msg.msg_id++;
        retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
        if (retval == SYSERR) {
            fprintf(stderr, ": udp_sendto failed\n");
        }

    }
                    
        cleardata(&msg);
    }

     return 0;
 }



/**************************************************************************
function name:  udpserver process
description:    UDP server process will continuously listen for packet. If a packet is reacived with the BB id  and the Device ID 
if will perform the Operation. For performing any action on any Device Edge has to set the device ID , BB id and 
Msg ID and the Status for the device. Every device connected to BB has a device id which is mentioned in the XML .
every message has its ID. and status the action which has to be performed on the device. E.g for a GPIO device 
like LED status 1 will turn on  the LED and 0 will turn off the LED. 
input:          void         
author:         Madhav Agrawal   
**************************************************************************/
process udpserver() {
	kprintf("udp server starts ");
	struct msg_struct msg;
    int32   retval;         /* return value from sys calls  */
   // uint32  localip;        /* local IP address     */
    int32   msglen;         /* length of outgoing message   */

    while (1) {
        kprintf("waiting for recv.........\n");
        retval = udp_recvaddr(slot, &remip, &remport,(char*)&msg,sizeof(struct msg_struct ), 200000);
       if (retval == TIMEOUT) {
            continue;
        } else if (retval == SYSERR) {
            kprintf(": error receiving UDP\n");
            //return 1;
        }

        if (msg.bb_id == BBID){
            kprintf( ": echo message\n");
            retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
            if (retval == SYSERR) {
                fprintf(stderr, ": udp_sendto failed\n");
            }
            msg = (struct msg_struct) msg;

            switch(msg.dev_id){
                case LED:
                {
                    kprintf( ": TEMP LED = %u\n",msg.status);
                    if (msg.status >0){
                        putc(LED,1);
                    } else {
                        putc(LED,0);
                    }
                    break;
                }
                case TEMP:
                    {
                        kprintf( ": TEMP reading....\n");
                        msg.dev_id = TEMP;
                        msg.bb_id = BBID;
                        msg.status = gettemp();
                        msg.msg_id++;
                        retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
                        if (retval == SYSERR) {
                            fprintf(stderr, ": udp_sendto failed\n");
                        }
                        break;
                    }
                case INPUT:
                    {
                    kprintf( ": INPUT READING....\n");
                    msg.dev_id = INPUT;
                    msg.bb_id = BBID;
                    msg.status = getc(INPUT);
                    msg.msg_id++;
                    retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
                    if (retval == SYSERR) {
                        fprintf(stderr, ": udp_sendto failed\n");
                    }
                    break;
                }

            }
        }
        kprintf( "sending done\n");
        cleardata(&msg);
    }
    return OK;
}


/**************************************************************************
function name:  main
description:    main function first Initialize the BBB then create two process one UDP client and One udp server.
input:          void        
author:         Madhav Agrawal   
**************************************************************************/
process	main(void)
{
 
	recvclr();
    kprintf("System Starts\n");
    putc(LED,1);
    BB_init();
    putc(LED,0);
    server_id = create(udpserver, 4096, 50, "udpserver",0);
    resume(server_id);
    client_id = create(udpclient, 4096, 50, "udpclient",0);
    resume(client_id);
	return OK;
}


