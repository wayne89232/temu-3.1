/*The following magic number is for Windows7 SP1 x64 version.
    You can modify them for other Windows OS version.
    If you would like to find these magic numbers for a specified Windows,
    use Windbg in guest OS or dump whole guest memory into a file and 
    use Volatility framework to analysis it.*/
#define PsActiveProcessHead 0x50
#define PCB 0x188
#define PID  0x180
#define ImageFileName 0x2e0
#define CR3 0x28
#define PPID 0x290
#define PEB 0x338
#define LDR 0x18
#define CreateTime 0x168
#define KDBG_offset 0x10
