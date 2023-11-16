/******************************************************************************
 											
                filename:			ext2.c			
                written by:     	Orit			
                reviewed by:    	Guy B		
 											
 ******************************************************************************/
#include <stdio.h>      /*  perror(), printf(), fopen(), fclose(), 
                            fprintf()                                         */
#include <sys/types.h>  /*  open(), lseek()                                   */
#include <sys/stat.h>   /*  open()                                            */
#include <fcntl.h>      /*  open()                                            */
#include <unistd.h>     /*  close(), read(), lseek()                          */
#include <string.h>     /*  strncmp()                                         */
#include <assert.h>     /*  assert()                                          */

#include "ext2_fs.h"    /*  struct ext2_super_block, struct ext2_group_desc,
                            struct ext2_inode, struct ext2_dir_entry_2        */
#include "ext2.h"

/******************************************************************************/
/*					defines, consts and typedef declarations:				  */
#define OUR_BLOCK_SIZE 1024
#define BLOCK_BUFFER_SIZE OUR_BLOCK_SIZE + 1

#define INDIRECT_BLOCK_PTR_ARR_SIZE 256 /*  OUR_BLOCK_SIZE / size of ptr in 
                                                             ext2 (4)       */

static size_t size_of_inode_struct = sizeof(struct ext2_inode);
static size_t size_of_sb_struct = sizeof(struct ext2_super_block);
static size_t size_of_gd_struct = sizeof(struct ext2_group_desc);

typedef unsigned int (*indirect_func_t)(int fd, unsigned char *block_buffer, 
                                            unsigned int tot_file_size, 
                                            unsigned int cur_file_size, 
                                            unsigned int indirect_num);

/******************************************************************************/
/*		        		Static functions declarations:		        		  */
/*Main static functions:*/
static int GetAndPrintSuperBlock(int fd, struct ext2_super_block *sb);
static void GetAndPrintGroupDescriptors(int fd, struct ext2_super_block *sb, 
                                        struct ext2_group_desc *gd);
static int FindAndPrintFile(int fd, struct ext2_super_block *sb, 
                                struct ext2_group_desc *gd, 
                                const char *file_path);

/*Calculate block offset from vdisk start:*/
static int OurBlockOffset(int block_num);

/*Read Functions:*/
static void ReadGroupDescriptor(int fd, struct ext2_super_block *sb, 
                                struct ext2_group_desc *gd,
                                unsigned int inode_no);
static void ReadInode(int fd, unsigned int inode_no,
                        struct ext2_super_block *sb,
                        struct ext2_group_desc *gd, 
                        struct ext2_inode *iter_inode);
static void ReadBlock(int fd, struct ext2_inode *iter_inode, 
                        unsigned char *block_buffer, unsigned int block_index);
static void ReadNestedBlock(int fd, unsigned int *block_buffer_int_ptr, 
                                unsigned char *nested_block_buffer, 
                                unsigned int ptr_index);

/*Find functions:*/
static unsigned int FindFileInodNum(int fd, struct ext2_super_block *sb, 
                                    struct ext2_group_desc *gd, 
                                    const char *file_path);
static size_t FindFileNameLen(const char *iter_file_path);
static unsigned int FindInodeOfRightFileName(struct ext2_inode *iter_inode, 
                                                const char *iter_file_path, 
                                                size_t file_name_len, int fd);
static unsigned int FindInodeInDirectBlocks(unsigned char *block_buffer,
                                            size_t file_name_len, 
                                            const char *iter_file_path);
static unsigned int FindInodeInIndirectBlock(unsigned char *block_buffer,
                                                size_t file_name_len, 
                                                const char *iter_file_path, 
                                                int fd, 
                                                unsigned int indirect_num);

/*Print functions:*/
static void PrintSuperBlock(struct ext2_super_block *sb);
static void PrintGroupDescriptors(struct ext2_super_block *sb,
                                    struct ext2_group_desc *gd);
static void PrintFile(int fd, struct ext2_inode *iter_inode);
static unsigned int PrintFromIndirectBlock(int fd, unsigned char *block_buffer, 
                                            unsigned int tot_file_size, 
                                            unsigned int cur_file_size, 
                                            unsigned int indirect_num);
static void PrintBlock(unsigned char *block_buffer, 
                                unsigned int cur_file_size);
static unsigned int PrintBlockEnvelop(int fd, unsigned char *block_buffer, 
                                    unsigned int tot_file_size, 
                                    unsigned int cur_file_size, 
                                    unsigned int indirect_num);

/******************************************************************************/
/*								The function:						          */
int PrintFileAndStat(const char *vdisk_path, const char *file_path)
{
    struct ext2_super_block sb;
    struct ext2_group_desc gd;
    int fd = 0;
    int status = 0;

    assert(NULL != vdisk_path);
    assert(NULL != file_path);

    fd = open(vdisk_path, O_RDONLY);
    if(0 > fd)
    {
        perror("Couldn't open vdisk_path");
        return PRINT_FILE_AND_STAT_BAD_VDISK_PATH;
    }

    status = GetAndPrintSuperBlock(fd, &sb);
    if(PRINT_FILE_AND_STAT_SUCCESS != status)
    {
        return status;
    }
    
    GetAndPrintGroupDescriptors(fd, &sb, &gd);
    status = FindAndPrintFile(fd, &sb, &gd, file_path);
    if(PRINT_FILE_AND_STAT_SUCCESS != status)
    {
        return status;
    }

    close(fd);

    return PRINT_FILE_AND_STAT_SUCCESS;
}

/******************************************************************************/
/*		        		Static functions definitions:		        		  */
/*Main static functions:*/
static int GetAndPrintSuperBlock(int fd, struct ext2_super_block *sb)
{
    lseek(fd, OUR_BLOCK_SIZE, SEEK_SET);
    read(fd, sb, size_of_sb_struct);
    if(EXT2_SUPER_MAGIC != sb->s_magic)
    {
        perror("Error in trying to find super block");
        return PRINT_FILE_AND_STAT_WRONG_FILE_SYSTEM;
    }
    PrintSuperBlock(sb);
}

static void GetAndPrintGroupDescriptors(int fd, struct ext2_super_block *sb, 
                                        struct ext2_group_desc *gd)
{
    ReadGroupDescriptor(fd, sb, gd, 0);
    PrintGroupDescriptors(sb, gd);
}

static int FindAndPrintFile(int fd, struct ext2_super_block *sb, 
                                struct ext2_group_desc *gd, 
                                const char *file_path)
{
    unsigned int inode_no = 0;
    struct ext2_inode iter_inode;
    
    assert(NULL != sb);
    assert(NULL != gd);
    assert(NULL != file_path);
    
    inode_no = FindFileInodNum(fd, sb, gd, file_path);

    if(0 == inode_no)
    {
        return PRINT_FILE_AND_STAT_COULD_NOT_FIND_FILE;
    }
    ReadGroupDescriptor(fd, sb, gd, inode_no);
    ReadInode(fd, inode_no, sb, gd, &iter_inode);
    PrintFile(fd, &iter_inode);

    return PRINT_FILE_AND_STAT_SUCCESS;
}

/*Calculate block offset from vdisk start:*/
static int OurBlockOffset(int block_num)
{
    return (block_num * OUR_BLOCK_SIZE);
}

/*Read Functions:*/
static void ReadGroupDescriptor(int fd, struct ext2_super_block *sb, 
                                struct ext2_group_desc *gd,
                                unsigned int inode_no)
{
    lseek(fd, OUR_BLOCK_SIZE + (inode_no / sb->s_inodes_per_group) * 
            size_of_gd_struct + size_of_sb_struct, SEEK_SET);
    read(fd, gd, size_of_gd_struct);
}

static void ReadInode(int fd, unsigned int inode_no, 
                        struct ext2_super_block *sb, 
                        struct ext2_group_desc *gd, 
                        struct ext2_inode *iter_inode)
{
    assert(NULL != gd);
    assert(NULL != iter_inode);

    lseek(fd, OurBlockOffset(gd->bg_inode_table) + 
            (inode_no % sb->s_inodes_per_group - 1) * size_of_inode_struct, SEEK_SET);
    read(fd, iter_inode, size_of_inode_struct);
}

static void ReadBlock(int fd, struct ext2_inode *iter_inode, 
                        unsigned char *block_buffer, unsigned int block_index)
{
    lseek(fd, OurBlockOffset(iter_inode->i_block[block_index]), SEEK_SET);
    read(fd, block_buffer, OUR_BLOCK_SIZE);
}

static void ReadNestedBlock(int fd, unsigned int *block_buffer_int_ptr, 
                                unsigned char *nested_block_buffer, 
                                unsigned int ptr_index)
{
    lseek(fd, OurBlockOffset(block_buffer_int_ptr[ptr_index]), SEEK_SET);
    read(fd, nested_block_buffer, OUR_BLOCK_SIZE);
}

/*Find functions:*/
static unsigned int FindFileInodNum(int fd, struct ext2_super_block *sb, 
                                    struct ext2_group_desc *gd, 
                                    const char *file_path)
{
    struct ext2_inode iter_inode;
    const char *iter_file_path = file_path;
    size_t file_name_len = 0;
    unsigned int inode_no = 2;
    
    while(0 < inode_no && '\0' != *iter_file_path)
    {
        ++iter_file_path;
        ReadGroupDescriptor(fd, sb, gd, inode_no);
        ReadInode(fd, inode_no, sb, gd, &iter_inode);
        file_name_len = FindFileNameLen(iter_file_path);
        inode_no = FindInodeOfRightFileName(&iter_inode, iter_file_path, 
                                            file_name_len, fd);
        iter_file_path += file_name_len;
    }

    return inode_no;
}

static size_t FindFileNameLen(const char *iter_file_path)
{
    size_t counter = 0;

    assert(NULL != iter_file_path);

    while('/' != *iter_file_path && '\0' != *iter_file_path)
    {
        ++iter_file_path;
        ++counter;
    }
    
    return counter;
}

static unsigned int FindInodeOfRightFileName(struct ext2_inode *iter_inode, 
                                                const char *iter_file_path, 
                                                size_t file_name_len, int fd)
{
    unsigned int num_of_blocks = 0;
    unsigned int maybe_inode = 0;
    unsigned int i = 0;
    unsigned char block_buffer[BLOCK_BUFFER_SIZE] = {0};

    assert(NULL != iter_inode);
    assert(NULL != iter_file_path);

    num_of_blocks = iter_inode->i_blocks;
    for(i = 0; 0 == maybe_inode && num_of_blocks < i && EXT2_NDIR_BLOCKS < i; 
        ++i)
    {
        ReadBlock(fd, iter_inode, block_buffer, i);
        maybe_inode = FindInodeInDirectBlocks(block_buffer, file_name_len, 
                                            iter_file_path);
    }
    for(; 0 == maybe_inode && num_of_blocks < i && i < EXT2_N_BLOCKS; ++i)
    {
        ReadBlock(fd, iter_inode, block_buffer, i);
        maybe_inode = FindInodeInIndirectBlock(block_buffer, file_name_len, 
                                                iter_file_path, fd, 
                                                i - EXT2_NDIR_BLOCKS);
    }

    return maybe_inode;
}

static unsigned int FindInodeInDirectBlocks(unsigned char *block_buffer,
                                            size_t file_name_len, 
                                            const char *iter_file_path)
{
    struct ext2_dir_entry_2 *de_iter = NULL;

    assert(NULL != iter_file_path);

    for(de_iter = (struct ext2_dir_entry_2 *)block_buffer; 
        0 < de_iter->inode && 
        ((size_t)de_iter - (size_t)block_buffer) < OUR_BLOCK_SIZE; 
        de_iter = (struct ext2_dir_entry_2 *)((char *)de_iter + 
                    de_iter->rec_len))
    {
        if(file_name_len == de_iter->name_len && 
            0 == strncmp(iter_file_path, de_iter->name, file_name_len))
        {
            return de_iter->inode;
        }
    }

    return 0;
}

static unsigned int FindInodeInIndirectBlock(unsigned char *block_buffer,
                                                    size_t file_name_len, 
                                                    const char *iter_file_path, 
                                                    int fd, 
                                                    unsigned int indirect_num)
{
    unsigned int *disk_adrr_ptr = (unsigned int *)block_buffer;
    unsigned int maybe_inode = 0;
    int i = 0;
    unsigned char nested_block_buffer[BLOCK_BUFFER_SIZE] = {0};

    for(i = 0; maybe_inode == 0 && i < INDIRECT_BLOCK_PTR_ARR_SIZE; ++i)
    {
        ReadNestedBlock(fd, disk_adrr_ptr, nested_block_buffer, i);
        if(0 == indirect_num)
        {
            maybe_inode = FindInodeInDirectBlocks(nested_block_buffer, 
                                                    file_name_len, 
                                                    iter_file_path);
        }
        else
        {
            maybe_inode = FindInodeInIndirectBlock(nested_block_buffer, 
                                                    file_name_len, 
                                                    iter_file_path, fd, 
                                                    indirect_num - 1);
        }
    }

    return maybe_inode;
}

/*Print functions:*/
static void PrintSuperBlock(struct ext2_super_block *sb)
{
    printf("Super block:\n");
    printf("Inodes count - %u\n", sb->s_inodes_count);
    printf("Blocks count - %u\n", sb->s_blocks_count);
    printf("Reserved blocks count - %u\n", sb->s_r_blocks_count);
    printf("Free blocks count - %u\n", sb->s_free_blocks_count);
    printf("Free inodes count - %u\n", sb->s_free_inodes_count);
    printf("First Data Block - %u\n", sb->s_first_data_block);
    printf("Block size - %u\n", sb->s_log_block_size);
    printf("Fragment size - %u\n", sb->s_log_frag_size);
    printf("# Blocks per group - %u\n", sb->s_blocks_per_group);
    printf("# Fragments per group - %u\n", sb->s_frags_per_group);
    printf("# Inodes per group - %u\n", sb->s_inodes_per_group);
    printf("Mount time - %u\n", sb->s_mtime);
    printf("Write time - %u\n", sb->s_wtime);
    printf("Mount count - %u\n", sb->s_mnt_count);
    printf("Maximal mount count - %u\n", sb->s_max_mnt_count);
    printf("Magic signature - %u\n", sb->s_magic);
    printf("File system state - %u\n", sb->s_state);
    printf("Behaviour when detecting errors - %u\n", sb->s_errors);
    printf("Minor revision level - %u\n", sb->s_minor_rev_level);
    printf("Time of last check - %u\n", sb->s_lastcheck);
    printf("Max. time between checks - %u\n", sb->s_checkinterval);
    printf("OS - %u\n", sb->s_creator_os);
    printf("Revision level - %u\n", sb->s_rev_level);
    printf("Default uid for reserved blocks - %u\n", sb->s_def_resuid);
    printf("Default gid for reserved blocks - %u\n\n", sb->s_def_resgid);
}

static void PrintGroupDescriptors(struct ext2_super_block *sb, 
                                    struct ext2_group_desc *gd)
{
    struct ext2_group_desc *iter_gd = gd;
    size_t gd_count = 1 + (sb->s_blocks_count - 1) / sb->s_blocks_per_group;
    size_t i = 0;
    
    printf("Group descriptors:\n");
    for(i = 0; i < gd_count; ++i, ++iter_gd)
    {
        printf("Group descriptor no. %lu:\n", i + 1);
        printf("Blocks bitmap block - %u\n", iter_gd->bg_block_bitmap);
        printf("Inodes bitmap block - %u\n", iter_gd->bg_inode_bitmap);
        printf("Inodes table block - %u\n", iter_gd->bg_inode_table);
        printf("Free blocks count - %u\n", iter_gd->bg_free_blocks_count);
        printf("Free inodes count - %u\n", iter_gd->bg_free_inodes_count);
        printf("Directories count - %u\n\n", iter_gd->bg_used_dirs_count);
    }
}

static void PrintFile(int fd, struct ext2_inode *iter_inode)
{
    unsigned char block_buffer[BLOCK_BUFFER_SIZE] = {0};
    unsigned int tot_file_size = 0;
    unsigned int cur_file_size = 0;
    unsigned int num_of_blocks = 0;
    unsigned int i = 0;

    assert(NULL != iter_inode);

    tot_file_size = iter_inode->i_size;

    printf("Printing file:\n");
    for(i = 0; tot_file_size < cur_file_size && EXT2_NDIR_BLOCKS < i; ++i)
    {
        ReadBlock(fd, iter_inode, block_buffer, i);
        PrintBlock(block_buffer, cur_file_size);
        cur_file_size += OUR_BLOCK_SIZE;
    }
    for(;cur_file_size < tot_file_size && i < EXT2_N_BLOCKS; ++i)
    {
        ReadBlock(fd, iter_inode, block_buffer, i);
        cur_file_size = PrintFromIndirectBlock(fd, block_buffer, tot_file_size, 
                                                cur_file_size, 
                                                i - EXT2_NDIR_BLOCKS);
    }
    printf("\n");
}

static unsigned int PrintFromIndirectBlock(int fd, unsigned char *block_buffer, 
                                            unsigned int tot_file_size, 
                                            unsigned int cur_file_size, 
                                            unsigned int indirect_num)
{
    static indirect_func_t indirect_print[3] = 
    {
        PrintBlockEnvelop, 
        PrintFromIndirectBlock, 
        PrintFromIndirectBlock
    };
    unsigned int *block_buffer_int_ptr = (unsigned int *)block_buffer;
    unsigned int i = 0;
    unsigned char nested_block_buffer[BLOCK_BUFFER_SIZE] = {0};

    assert(NULL != block_buffer);

    for(i = 0; cur_file_size < tot_file_size && 
        i < INDIRECT_BLOCK_PTR_ARR_SIZE; ++i)
    {
        ReadNestedBlock(fd, block_buffer_int_ptr, nested_block_buffer, i);
        cur_file_size = indirect_print[indirect_num](fd, nested_block_buffer, 
                                                        tot_file_size, 
                                                        cur_file_size, 
                                                        indirect_num - 1);
    }

    return cur_file_size;
}

static void PrintBlock(unsigned char *block_buffer, 
                                unsigned int cur_file_size)
{
    assert(NULL != block_buffer);

    printf("%s", block_buffer);
}

static unsigned int PrintBlockEnvelop(int fd, unsigned char *block_buffer, 
                                    unsigned int tot_file_size, 
                                    unsigned int cur_file_size, 
                                    unsigned int indirect_num)
{
    assert(NULL != block_buffer);

    (void)fd;
    (void)tot_file_size;
    (void)indirect_num;

    PrintBlock(block_buffer, cur_file_size);
    
    return (OUR_BLOCK_SIZE + cur_file_size);
}