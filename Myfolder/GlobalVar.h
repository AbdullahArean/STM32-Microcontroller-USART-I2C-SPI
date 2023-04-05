static char send[50],recevie[50],output_buffer[100],input_buffer[100],s1[50],s2[50],ch,reply[100];
static int i=0,j=0,in=0,R1=1,R2=1,Y1=1,Y2=1,G1=1,G2=1,T1=1,T2=1,flag=0;
static int traffic=20,time_delay;//uint32_t
static int totaltime=0;//uint32_t
static int flagfor6to1=0,flagfor1to6=0;
void display(void);