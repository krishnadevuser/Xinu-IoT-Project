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
        cleardata(&msg);
       /* msg.dev_id = 1;
        msg.bb_id = BBID;
        msg.status = BBB_INIT;
        msg.msg_id++;
        retval = udp_sendto(slot,0xC0A86402, 3333, (char*)&msg,sizeof(struct msg_struct));
            if (retval == SYSERR) {
                fprintf(stderr, ": udp_sendto failed\n");
        }
        continue;*/
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

/*process main(void)
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

    

    return OK;
}*/

