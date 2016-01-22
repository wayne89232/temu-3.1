#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tsk/libtsk.h>

// when compiling file using TSK library: gcc filename.c -o filename -ltsk
typedef struct{
   TSK_FS_FILE *file;
   TSK_DADDR_T mft_entry_addr;
   int curr_mft_idx;
} NTFS_ADDR_MFT_ENTRY;
// fiwalk function goes through MFT table until approches the matched file, and get its address
static TSK_WALK_RET_ENUM
cb_get_metadata_addr(TSK_FS_FILE *fs_file, TSK_OFF_T a_off, TSK_DADDR_T
addr,
      char *buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void* ptr)
{
   NTFS_ADDR_MFT_ENTRY *addr_mfte = (NTFS_ADDR_MFT_ENTRY*)ptr;

   addr_mfte->curr_mft_idx += 4;  

   if(addr_mfte->file->meta->addr < addr_mfte->curr_mft_idx){

      addr_mfte->mft_entry_addr = addr;

      return TSK_WALK_STOP;
   }
   return TSK_WALK_CONT;
}

int main(int argc, char **argv1)
{
    TSK_IMG_INFO *img_info;
    TSK_TCHAR **argv;
    TSK_FS_INFO *fs_info;
    TSK_FS_FILE *fs_file;
    TSK_DADDR_T *addr;
    uint8_t attr_check;

#ifdef TSK_WIN32
    // On Windows, get the wide arguments (?)
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
#else
    argv = (TSK_TCHAR **) argv1;
#endif
    // open the disk image
    img_info =
        tsk_img_open_sing((const TSK_TCHAR *) argv[1],
        TSK_IMG_TYPE_DETECT, 0);
    if (img_info == NULL) {
        fprintf(stderr, "Error opening\n");
        tsk_error_print(stderr);
        exit(1);
    }
    //open file system
    fs_info =
        tsk_fs_open_img(img_info, 206848*512, TSK_FS_TYPE_NTFS_DETECT);    
    if (fs_info == NULL) {
        fprintf(stderr, "Error opening file system\n");
        tsk_error_print(stderr);
        exit(1);
    }
    //open file
    fs_file = tsk_fs_file_open(fs_info, NULL, (const char*) argv[2]);

    NTFS_ADDR_MFT_ENTRY addr_mfte;

    TSK_FS_FILE *mft_file;
    
    /* Open the $MFT file, which has metadata address 0 */
    mft_file = tsk_fs_file_open_meta(fs_info, NULL, 0);
    addr_mfte.file = fs_file;
    addr_mfte.curr_mft_idx = 0;

    /* Process the $MFT file */
    if((tsk_fs_file_walk(mft_file, TSK_FS_FILE_WALK_FLAG_NONE,
                        cb_get_metadata_addr,(void*)&addr_mfte) != 0)){
        printf("fiwalk != 0\n");
    }

    printf("%" PRId64 "\n", addr_mfte.mft_entry_addr);

    if(mft_file == NULL){
         fprintf(stderr, "Error opening file\n");
         tsk_error_print(stderr);
         exit(1);
     }
    tsk_fs_file_close(fs_file);
    tsk_fs_close(fs_info);
    tsk_img_close(img_info);
    return 0;
}