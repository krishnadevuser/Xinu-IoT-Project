#include <xinu.h>
#include <stdio.h>
#include <string.h>


#define MSG_MAX_LEN 64
#define TEMP_BUF_LEN 4
#define BBB_INIT 200


#define PORT 4444
#define BBID 0
#define DIG_INPUT 0

#define PORT 2222
#define BBID 1
#define DIG_INPUT 0

pid32 server_id, client_id;

uint32  remip;          /* remote sender's IP address   */
uint16  remport;        /* remote sender's UDP port */
int32   slot;
uint32 localip;

struct msg_struct{
    uint32 bb_id;
    uint32 msg_id;
    uint32 dev_id;
    uint32 status;
};


void cleardata(struct msg_struct *msg){
    msg->bb_id = -1;
    msg->msg_id =  -1;
    msg->dev_id = -1;
    msg->status = -1;

}

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
        retval = udp_recvaddr(slot, &remip, &remport,(char*)&msg,sizeof(struct msg_struct ), 200000);
        if (retval == TIMEOUT) {
            continue;
        } else if (retval == SYSERR) {
            kprintf(": error receiving UDP\n");
            //return 1;
        }
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



process udpserver() {
	kprintf("udp server starts ");
	struct msg_struct msg;
    int32   retval;         /* return value from sys calls  */
   // uint32  localip;        /* local IP address     */
    int32   msglen;         /* length of outgoing message   */

    while (TRUE) {
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
                    if (msg.status >0){
                        putc(LED,1);
                    } else {
                        putc(LED,0);
                    }
                break;


                case TEMP:
                    msg.dev_id = TEMP;
                    msg.bb_id = BBID;
                    msg.status = gettemp();
                    msg.msg_id++;
                    retval = udp_sendto(slot, remip, remport, (char*)&msg,sizeof(struct msg_struct));
                    if (retval == SYSERR) {
                        fprintf(stderr, ": udp_sendto failed\n");
                    }
                 break;
                case INPUT:
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
        cleardata(&msg);
    }
    return OK;
}


process	main(void)
{
 
	recvclr();
    kprintf("System Starts\n");
    BB_init();
    server_id = create(udpserver, 4096, 50, "udpserver",0);
    resume(server_id);
    client_id = create(udpclient, 4096, 50, "udpclient",0);
    resume(client_id);
	return OK;
}

