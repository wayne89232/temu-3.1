#define PsActiveProcessHead 0x50
#define PCB 0x188
#define PID  0x180
#define handle_table  0x200
#define handle_file  0x50
#define ImageFileName 0x2e0
#define CR3 0x28
#define PPID 0x290
#define PEB 0x338
#define LDR 0x18
#define CreateTime 0x168
#define KDBG_offset 0x10
#define pool_offset 0x10

void PSlist(Monitor *mon,hwaddr KDBG_addr);
uint64_t findKDBG(void);
uint64_t my_memory_dump(hwaddr addr);
char* f2p_mapping(char* fname);