
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
// #include "../config.h"
#include "main.h"
#include "../plugin.h"

static plugin_interface_t my_interface;
FILE *my_log;

typedef enum { false = 0, true = !false } bool;
int PDU_bytes = 128;

int target_s_port = -1;
int target_d_port = -1;

char* target_s_ip = "NOT_SET";
char* target_d_ip = "NOT_SET";

int target_protocol_number = -1;

bool enable_print_packet = false;
bool enable_log = false;
bool enable_pcap_log = false;

bool enable_traceblk = false;
bool enable_print_blkio = false;
bool first_file = 1;
int count = 0;

uint64_t sector_number = 0;
char* target_file_name = "NOT_SET";

typedef struct node
{
  int data;
  char fname[50];
  struct node* next;
} NODES;

//NODES* list;
static NODES *list = NULL;

typedef struct mon_cmd_t {
  const char *name;
  const char *args_type;
  const char *params;
  const char *help;
  union {
    void (*cmd);
  } mhandler;
} mon_cmd_t;

static void temp_function()
{
  printf("nic_target_port\n");
}

static void get_sectornum(char* filename);
static void saveFile(NODES* list, char* fname);
static void print_lists(NODES *node);


static mon_cmd_t my_term_cmds[] = {
  {
    .name       = "temp_function",
    .args_type  = "",
    .params     = "",
    .help       = "temp_function",
    .mhandler.cmd = temp_function
  },
  {NULL, NULL},
};

static void test()
{
  printf("test\n");
}

static void set_plugin(const char *property, const char *value ) {
  char* temp_string;
  temp_string = "target_s_port";
  if (strcmp(property, temp_string) == 0) {
    target_s_port = atoi(value);
    printf("setting target source port: %d\n", target_s_port);
    return;
  }
  temp_string = "target_d_port";
  if (strcmp(property, temp_string) == 0) {
    target_d_port = atoi(value);
    printf("setting target destination port: %d\n", target_d_port);
    return;
  }
  temp_string = "target_s_ip";
  if (strcmp(property, temp_string) == 0) {
    target_s_ip = strdup(value);
    printf("setting target source ip: %s\n", target_s_ip);
    return;
  }
  temp_string = "target_d_ip";
  if (strcmp(property, temp_string) == 0) {
    target_d_ip = strdup(value);
    printf("setting target destination ip: %s\n", target_d_ip);
    return;
  }
  temp_string = "PDU_bytes";
  if (strcmp(property, temp_string) == 0) {
    PDU_bytes = atoi(value);
    printf("setting PDU bytes: %d\n", PDU_bytes);
    return;
  }

  temp_string = "target_protocol_number";
  if (strcmp(property, temp_string) == 0) {
    target_protocol_number = atoi(value);
    printf("setting target protocol number: %d\n", target_protocol_number);
    if (target_protocol_number == 6) {
      printf("Protocol: 6(tcp)\n");
    }
    else if (target_protocol_number == 17) {
      printf("Protocol: 17(udp)\n");
    }
    else if (target_protocol_number == 1) {
      printf("Protocol: 128(icmp)\n");
    }
    else {
      printf("Protocol number:%d\n", target_protocol_number);
    }
    return;
  }

  temp_string = "target_file_name";
  if (strcmp(property, temp_string) == 0) {
    target_file_name = strdup(value);
    printf("setting target file name: %s\n", target_file_name);
    char * file;
    file = strdup(value);
    //if(first_file == 1)

    saveFile(list, file);
    return;
  }
  temp_string = "file_list";
  if (strcmp(property, temp_string) == 0) {
    printf("Print current file list: \n");
    print_lists(list);
  }
}
static void reset_plugin(const char *property) {
  char* temp_string;
  temp_string = "target_s_port";
  if (strcmp(property, temp_string) == 0) {
    printf("resetting target source port\n");
    target_s_port = -1;
    return;
  }
  temp_string = "target_d_port";
  if (strcmp(property, temp_string) == 0) {
    printf("resetting target destination port\n");
    target_d_port = -1;
    return;
  }
  temp_string = "target_s_ip";
  if (strcmp(property, temp_string) == 0) {
    printf("resetting target source ip\n");
    target_s_ip = "NOT_SET";
    return;
  }
  temp_string = "target_d_ip";
  if (strcmp(property, temp_string) == 0) {
    printf("resetting target destination ip\n");
    target_d_ip = "NOT_SET";
    return;
  }
  temp_string = "PDU_bytes";
  if (strcmp(property, temp_string) == 0) {
    printf("resetting PDU bytes (128)\n");
    PDU_bytes = 128;
    return;
  }
  temp_string = "target_protocol_number";
  if (strcmp(property, temp_string) == 0) {
    printf("resetting target protocol number\n");
    target_protocol_number = -1;
    return;
  }
}
static void toggle_plugin(const char *property) {
  char* temp_string;
  temp_string = "enable_print_packet";
  if (strcmp(property, temp_string) == 0) {
    enable_print_packet = !enable_print_packet;
    printf("toggle enable_print_packet: %d\n", enable_print_packet);
    return;
  }
  temp_string = "enable_log";
  if (strcmp(property, temp_string) == 0) {
    enable_log = !enable_log;
    printf("toggle enable_log: %d\n", enable_log);
    return;
  }
  temp_string = "enable_pcap_log";
  if (strcmp(property, temp_string) == 0) {
    enable_pcap_log = !enable_pcap_log;
    printf("toggle enable_pcap_log: %d\n", enable_pcap_log);
    return;
  }
  temp_string = "enable_traceblk";
  if (strcmp(property, temp_string) == 0) {
    enable_traceblk = !enable_traceblk;
    printf(" enable_traceblk: %d\n", enable_traceblk );
    return;

  }
  temp_string = "enable_print_blkio";
  if (strcmp(property, temp_string) == 0) {
    enable_print_blkio = !enable_print_blkio;
    printf("enable_print_blkio\n");
    return;
  }


}

static void log_packet_pcap(const uint8_t *buf, size_t size) {
  // return;
  FILE * fp;

  // time_t rawtime;
  // struct tm * timeinfo;
  // time ( &rawtime );
  // timeinfo = localtime ( &rawtime );

  fp = fopen ("packet_log.pcap", "ab"); //use time

  struct timeval tv;
  unsigned int timestamp;
  gettimeofday(&tv, NULL);
  timestamp = tv.tv_sec;
  fwrite( &timestamp, sizeof( timestamp ), 1, fp );

  timestamp = tv.tv_usec;
  fwrite( &timestamp, sizeof( timestamp ), 1, fp );

  // fwrite( &timestamp, sizeof( timestamp ), 1, fp );

  int Caplen;
  if (size <= PDU_bytes) {
    Caplen = size;//test for wireshark process name
    fwrite( &Caplen, sizeof( Caplen ), 1, fp );
    fwrite( &Caplen, sizeof( Caplen ), 1, fp );
  }
  else {
    int len = size;
    Caplen = PDU_bytes;//test for wireshark process name
    fwrite( &Caplen, sizeof( Caplen ), 1, fp );
    fwrite( &len, sizeof( len ), 1, fp );
  }
  int i = 0;
  char content;
  for (i = 0; i < Caplen; i++) {

    content = *(buf + i);
    fwrite( &content, sizeof( content ), 1, fp );

  }
  //wiret WORM for wireshark process name test
  // int j;
  // j = 0x4D524F57;
  // fwrite( &j, sizeof( j ), 1, fp );

  chmod("packet_log.pcap", 0777);
  fclose(fp);
}
static void log_packet_readable(const uint8_t *buf, size_t size) {
  FILE * fp;

  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  fp = fopen ("packet_log", "a"); //use time

  fprintf(fp, "[packet received]%s\n", asctime(timeinfo));
  fprintf(fp, "Source IP:%d.%d.%d.%d\n", *(buf + 26), *(buf + 27), *(buf + 28), *(buf + 29));
  fprintf(fp, "Source Port:%d\n", 256 * (*(buf + 34)) + * (buf + 35));
  fprintf(fp, "Destination IP:%d.%d.%d.%d\n", *(buf + 30), *(buf + 31), *(buf + 32), *(buf + 33));
  fprintf(fp, "Destination Port:%d\n", 256 * (*(buf + 36)) + * (buf + 37));
  if (*(buf + 23) == 6)
    fprintf(fp, "Protocol: 6(tcp)\n");
  else if (*(buf + 23) == 17)
    fprintf(fp, "Protocol: 17(udp)\n");
  else if (*(buf + 23) == 1)
    fprintf(fp, "Protocol: 128(icmp)\n");
  else
    fprintf(fp, "Protocol number:%d\n", *(buf + 23));
  fprintf(fp, "pdu: ");
  int i;
  for (i = 0; i < size; i++) {
    if (i % 2 == 0)
      fprintf(fp, " ");
    fprintf(fp, "%02x", *(buf + i));
  }
  fprintf(fp, "     -end\r\n");
  fprintf(fp, "---------------------------------------\r\n");
  chmod("packet_log", 0777);
  fclose(fp);
}

static void print_packet(const uint8_t *buf, size_t size) {
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf("[packet received]%s\n", asctime(timeinfo));
  printf("Source IP:%d.%d.%d.%d\n", *(buf + 26), *(buf + 27), *(buf + 28), *(buf + 29));
  printf("Source Port:%d\n", 256 * (*(buf + 34)) + * (buf + 35));
  printf("Destination IP:%d.%d.%d.%d\n", *(buf + 30), *(buf + 31), *(buf + 32), *(buf + 33));
  printf("Destination Port:%d\n", 256 * (*(buf + 36)) + * (buf + 37));
  if (*(buf + 23) == 6)
    printf("Protocol: tcp\n");
  else if (*(buf + 23) == 17)
    printf("Protocol: udp\n");
  else if (*(buf + 23) == 1)
    printf("Protocol: icmp\n");
  else
    printf("Protocol number:%d\n", *(buf + 23));

  printf("pdu: ");
  int i;
  for (i = 0; i < size; i++) {
    printf("%02x ", *(buf + i));
  }
  printf("\n---------------------------------------\n");
}

static void get_logged(const uint8_t *buf, size_t size) {
  if (enable_print_packet) {
    print_packet(buf, size);
  }
  if (enable_log) {
    log_packet_readable(buf, size);
  }
  if (enable_pcap_log) {
    log_packet_pcap(buf, size);
  }
}

static void get_packet(const uint8_t *buf, size_t size, int mode) {
  if (
    !enable_print_packet &&
    !enable_log &&
    !enable_pcap_log
  )
  {
    return;
  }
  int s_port = 256 * (*(buf + 34)) + *(buf + 35);
  int d_port = 256 * (*(buf + 36)) + *(buf + 37);

  char s_ip[100];
  char d_ip[100];
  sprintf(s_ip, "%d.%d.%d.%d", *(buf + 26), *(buf + 27), *(buf + 28), *(buf + 29));
  sprintf(d_ip, "%d.%d.%d.%d", *(buf + 30), *(buf + 31), *(buf + 32), *(buf + 33));
  char* target_ip_not_set = "NOT_SET";


  int protocol_number = *(buf + 23);

  if ((
        (
          target_s_port != -1 &&
          target_s_port != s_port
        ) || (
          target_d_port != -1 &&
          target_d_port != d_port
        )
      )
      || (
        (
          strcmp(target_s_ip, target_ip_not_set) != 0 &&
          strcmp(target_s_ip, s_ip) != 0
        ) || (
          strcmp(target_d_ip, target_ip_not_set) != 0 &&
          strcmp(target_d_ip, d_ip) != 0
        )
      )
      || (
        target_protocol_number != -1 &&
        protocol_number != target_protocol_number
      )
     ) {
    return;
  }
  get_logged(buf, size);
}

static void get_sectornum(char* filename){
  char buf[30]; 
  char buff[30]; 

  FILE *fp;
  //printf("%s\n","hi" );
  // bash ./fname2sector.sh filename
  char* bash = "bash ../../fname2sector_singl.sh ";  
  char* file = filename;

  char *s = malloc(strlen(bash) + strlen(file) + 1);
  strcpy(s, bash);
  strcat(s, file);

  if ((fp = popen(s, "r")) == NULL) {
    printf("popen() error!\n");
    exit(1);
  }
  // run the shell script with popen
  fgets(buf, sizeof buf, fp);
  // fgets the first sector info
  //printf("%s", buf);
  pclose(fp);
  free(s);


  if (atoi(buf) == -1000)
  {
    FILE *fp;
    char* c  ="../../fname2sector_res ../../../win7_test.img ";

    char* s = malloc(strlen(c)+strlen(file)+1);
    strcpy(s,c);
    strcat(s,file);
    if ((fp = popen(s, "r")) == NULL) {  
      printf("popen() error!\n");  
      exit(1);
    }
    fgets(buff,sizeof buff, fp);
    pclose(fp);
    free(s);
  }
  int sector;
  if(atoi(buf) == -1000)
    sector = atoi(buff);
  else
    sector = atoi(buf);
  

  sector_number = (uint64_t) sector;
  printf("get sector num:%"PRIu64"\n", sector_number);
  //get
}


static void print_blockio (uint64_t sector_num, uint64_t base, uint64_t len, int dir, char* fname) {
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  printf("\n");
  printf("[io time]%s\n", asctime(timeinfo));
  printf("filename: %s\n", fname);
  printf("sector number: %"PRIu64"\n", sector_num);

  // if(strstr(fname,"txt"))
  //   printf("Process: notepad.exe" );
  // else
  //   printf("Process: mspaint.exe");
  printf("\nIO buffer: \n");
  printf("  base: %"PRIu64"\n", base);
  printf("  length: %"PRIu64"\n", len);
  if(dir == 1)
    printf("IO Write\n");
  else if (dir == 0)
    printf("IO Read\n");

FILE* log = fopen("../IO_log.log", "a");
  fprintf(log,"\n");
  fprintf(log,"[io time]%s\n", asctime(timeinfo));
  fprintf(log,"filename: %s\n",fname);
  fprintf(log,"sector number: %"PRIu64"\n", sector_num);
  fprintf(log,"Process: notepad.exe" );
  fprintf(log,"\nIO buffer: \n");
  fprintf(log,"  base: %"PRIu64"\n", base);
  fprintf(log,"  length: %"PRIu64"\n", len);
  if(dir == 1)
    fprintf(log,"IO Write\n");
  else if(dir == 0)
    fprintf(log,"IO Read\n");
  fprintf(log, "=====================================\n");
fclose(log);

}


static void log_blkio(uint64_t sector_num, uint64_t base, uint64_t len, int dir, char* fname) {
  //if(enable_print_blkio)
// printf("say\n");
  print_blockio(sector_num, base, len, dir, fname);

}



static void get_blockio(uint64_t sector_num, uint64_t base, uint64_t len, int dir) {
//printf("hi2\n");
  // if((sector_number == 0) || (sector_number != sector_num))
  // {
  //  // printf("Nothing!\n");
  //   return;
  // }
  // log_blkio(sector_num, base, len, dir);
  NODES* tmp = list;

  while (tmp != NULL)
  {
    if ((sector_num == 0))
    {
      return;
    }

    if (sector_num == tmp->data)
      log_blkio(sector_num, base, len, dir, tmp->fname);
    
    tmp = tmp->next;
  }
}

  





static void do_nic_receive(const uint8_t *buf, size_t size) {
  get_packet(buf, size, 0);
}

static void do_nic_send(const uint8_t *buf, size_t size) {
  get_packet(buf, size, 1);
}

static void do_blk_write(uint64_t sector_num, uint64_t base, uint64_t len) {
  get_blockio(sector_num, base, len, 1);
  
}

static void do_blk_read(uint64_t sector_num, uint64_t base, uint64_t len) {
  get_blockio(sector_num, base, len, 0);\
}

static void insertNode(NODES *node, char fname[], int data)
{
  NODES *newNode = (NODES *)malloc(sizeof(NODES));
  newNode->data = data;
  strcpy(newNode->fname, fname);
  newNode->next = node->next;
  node->next = newNode;

}

static void print_lists(NODES *node)
{
  NODES* n = node;

  while (n != NULL)
  {
    printf("%s: %d\n", n->fname, n->data);

    n = n->next;
  }

}

static void freeList(NODES* head)
{
  NODES* tmp;

  while (head != NULL)
  {
    tmp = head;
    head = head->next;
    free(tmp);
  }
}

static void saveFile(NODES* list, char* fname)
{
  get_sectornum(fname);
  int sector = (int) sector_number;
  if (first_file) {
    if (sector != 0) {
      //*list = malloc(sizeof(NODES));
      strcpy(list->fname, fname);
      list->data = sector;
      list->next = NULL;
      first_file =  0;
    }
  } else {

    insertNode(list, fname, sector);
    count ++;
  }

}


static void create_logfile(void) {
  remove("packet_log.pcap");
  FILE * fp;
  long int header;
  fp = fopen ("packet_log.pcap", "a"); //use time
  // const char *header = "d4c3 b2a1 0200 0400 0000 0000 0000 0000\nffff 0000 0100 0000 ";
  header = 0x00040002a1b2c3d4;
  fwrite( &header, sizeof( header ), 1, fp );
  header = 0x0000000000000000;
  fwrite( &header, sizeof( header ), 1, fp );
  header = 0x000000010000ffff;
  fwrite( &header, sizeof( header ), 1, fp );

  fclose(fp);
}

plugin_interface_t * init_plugin()
{
  if (!(my_log = fopen("plugin.log", "a"))) {
    fprintf(stderr, "cannot create plugin.log\n");
    return NULL;
  }
  create_logfile();
  //NODES *list = (NODES *)malloc(sizeof(NODES));
  //static NODES *list = NULL;
  if (list == NULL)
    list = (NODES *)malloc(sizeof(NODES));
  my_interface.nic_send = do_nic_send;
  my_interface.nic_recv = do_nic_receive;
  my_interface.blk_write = do_blk_write;
  my_interface.blk_read = do_blk_read;
  // my_interface.term_cmds = my_term_cmds;

  my_interface.test = test;
  my_interface.set_plugin = set_plugin;
  my_interface.toggle_plugin = toggle_plugin;
  my_interface.reset_plugin = reset_plugin;
  return &my_interface;
}


