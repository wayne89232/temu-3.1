
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include <stdint.h>
#include "../include/exec/hwaddr.h"
#include <inttypes.h>
#include "../static_structs_test.h"
#include "../plugin.h"

static plugin_interface_t my_interface;
FILE *my_log;

static void tests(p_cmdline* opaque)
{
  printf("testing\n");
}


static void test()
{
  printf("test on booting\n");
}

// /*return the integer for a given addr*/
static uint64_t my_memory_dump_printd(CPUArchState *env, hwaddr addr)
{
    int  len;
    uint8_t buf[16];
    uint64_t v;
    len = 8;

    
    if (cpu_memory_rw_debug(ENV_GET_CPU(env), addr, buf, len, 0) < 0) {
                return -1;
    }  
    v = ldq_p(buf);
    return v;
}

/*print the char for a given addr*/
static void my_memory_dump_printc(CPUArchState *env, hwaddr addr)
{
    int  i, len;
    uint8_t buf[16];
    uint64_t v;
    len = 16; 
    i = 0;
   // char name[16];

    if (cpu_memory_rw_debug(ENV_GET_CPU(env), addr, buf, len, 0) < 0) {
             printf( " Cannot get ProcessName.\n");
    }
    while (i < len) {
              v = ldub_p(buf + i);         
              if (v >= 32 && v <= 126) { //from space to ~
                     printf( "%c", (int) v);       
                    // snprintf(name[i], 16,"%c",(int)v);
              }  else{
                     printf(" ");       
              }
              i ++;
     }
    //printf( " %s\n, name");
}

/*return the hex value for a given addr*/
static uint64_t my_memory_dump(CPUArchState *env, hwaddr addr)
{ 
    int  len;
    uint8_t buf[16]; 
    uint64_t v; 
    len = 8; 
    
    if (cpu_memory_rw_debug(ENV_GET_CPU(env), addr, buf, len, 0) < 0) {
                return -1;
    }else{   
                v = ldq_p(buf);
                return v;
    }
}

/*find KDBG address between start_addr and end_addr*/
static uint64_t findKDBG(CPUArchState *env)
{ 
  target_ulong start_addr = 0xfffff80003b00000;
  target_ulong end_addr = 0xfffff80004000000; 
  target_ulong kdbg_value = 0x000003404742444b;

   while(start_addr != end_addr){
           start_addr += KDBG_offset; 
            if(my_memory_dump(env, start_addr) == kdbg_value)            
                   break;
   }
    return start_addr - KDBG_offset ;  //if not found,return end_addr
}

static void getcr3(p_cmdline* opaque)
{
    target_ulong KDBG_addr;
    target_ulong pshead_addr ; 
    target_ulong eprocess_actproclink_addr; 
    target_ulong eprocess_next_actproclink_addr ;
    target_ulong pcb_addr;//eprocess_head_addr = pcb
    target_ulong imagefilename ;
    target_ulong pid_addr , pid_value ;
    target_ulong pshead_value; 
    target_ulong cr3_value;
    target_ulong cr3_addr;  
    
    CPUArchState *env = opaque->env;
    
    KDBG_addr = findKDBG(env);

    if(KDBG_addr + KDBG_offset == 0xfffff80004000000)
    {
             printf("Could not found KDBG address.\n");
    }else{           
             pshead_addr  = KDBG_addr + PsActiveProcessHead;
             pshead_value = my_memory_dump(env, pshead_addr); 
               
             //first process 
             eprocess_actproclink_addr = my_memory_dump(env, pshead_value) ; //pshead_value 's value = eprocess_actkink_addr
             pcb_addr = eprocess_actproclink_addr - PCB;
             cr3_addr = pcb_addr + CR3;
             cr3_value = my_memory_dump(env, cr3_addr) ; 
             imagefilename = pcb_addr + ImageFileName;
             pid_addr = pcb_addr + PID; 
             pid_value = my_memory_dump_printd(env, pid_addr);

             printf(  "-----------------Process CR3-----------------\n");
             printf(  "ProcessName:");
             my_memory_dump_printc(env, imagefilename);
             printf(  "PID:%i %lu   ",4,(unsigned long)pid_value);
             printf(  "CR3=0x%lu\n", (unsigned long)cr3_value);

             while(my_memory_dump(env, eprocess_actproclink_addr) != pshead_value)
             {
                    eprocess_next_actproclink_addr = my_memory_dump(env, eprocess_actproclink_addr) ; 
                    pcb_addr = eprocess_next_actproclink_addr - PCB;
                    imagefilename  = pcb_addr + ImageFileName;
                    cr3_addr = pcb_addr + CR3;
                    cr3_value = my_memory_dump(env, cr3_addr) ; 
                    pid_addr = pcb_addr +PID; 
                    pid_value = my_memory_dump_printd(env, pid_addr);

                    if(pid_value ==0)
                        break; //break for last process
             
                   printf(  "ProcessName:");
                   my_memory_dump_printc(env, imagefilename);
                   printf(  "PID:%i %lu   ",4,(unsigned long)pid_value);
                   printf("CR3=0x%lu\n", (unsigned long)(cr3_value));
              
                   eprocess_actproclink_addr =  eprocess_next_actproclink_addr ;
             }
                printf( "---------------------------------------------\n");
                
                printf(  "Current vcpu CR3 = 0x%lu (physical addr)\n", (unsigned long)(env->cr[3])); 
             }
}


static plugin_cmd my_term_cmds[] = {
    {
        .name       = "tests",
        .args_type  = "",
        .params     = "",
        .help       = "do some tests",
        .cmd_handler = tests,
    },
};


plugin_interface_t * init_plugin()
{
  if (!(my_log = fopen("plugin.log", "w"))) {
    fprintf(stderr, "cannot create plugin.log\n");
    return NULL;
  }

  my_interface.term_cmds = my_term_cmds;

  my_interface.test = test;
  return &my_interface;
}


