
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

static void do_set_plugin(const char *property, const char *value ) {
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
    if (target_protocol_number == 6){
      printf("Protocol: 6(tcp)\n");
    }
    else if (target_protocol_number == 17){
      printf("Protocol: 17(udp)\n");
    }
    else if (target_protocol_number == 1){
      printf("Protocol: 128(icmp)\n");
    }
    else{
      printf("Protocol number:%d\n", target_protocol_number);
    }
    return;
  }
}
static void do_reset_plugin(const char *property) {
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
static void do_toggle_plugin(const char *property) {
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
    Caplen = size + 4;//test for wireshark process name
    fwrite( &Caplen, sizeof( Caplen ), 1, fp );
    fwrite( &Caplen, sizeof( Caplen ), 1, fp );
  }
  else {
    int len = size;
    Caplen = PDU_bytes + 4;//test for wireshark process name
    fwrite( &Caplen, sizeof( Caplen ), 1, fp );
    fwrite( &len, sizeof( len ), 1, fp );
  }
  int i = 0;
  char content;
  for (i = 0; i < Caplen - 4; i++) {

    content = *(buf + i);
    fwrite( &content, sizeof( content ), 1, fp );

  }
  //wiret WORM for wireshark process name test
  int j;
  j = 0x4D524F57;
  fwrite( &j, sizeof( j ), 1, fp );

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
  if(
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
  // printf("----------------------------------\n");
  // printf("%s\n", target_s_ip);
  // printf("%s\n", s_ip);
  // printf("%d\n", strcmp(target_s_ip, target_ip_not_set));
  // printf("%d\n", strcmp(target_s_ip, s_ip));
  // printf("----------------------------------\n");
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

static void do_nic_receive(const uint8_t *buf, size_t size) {
  get_packet(buf, size, 0);
}

static void do_nic_send(const uint8_t *buf, size_t size) {
  get_packet(buf, size, 1);
}

static void create_logfile(void) {
  remove("packet_log.pcap");
  FILE * fp;
  long int header;
  fp = fopen ("packet_log.pcap", "ab"); //use time
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
  if (!(my_log = fopen("plugin.log", "w"))) {
    fprintf(stderr, "cannot create plugin.log\n");
    return NULL;
  }
  create_logfile();
  my_interface.nic_send = do_nic_send;
  my_interface.nic_recv = do_nic_receive;

  // my_interface.term_cmds = my_term_cmds;

  my_interface.reset_plugin = do_reset_plugin;
  my_interface.set_plugin = do_set_plugin;
  my_interface.toggle_plugin = do_toggle_plugin;
  return &my_interface;
}


