#include "simplefs-ops.h"

extern struct filehandle_t file_handle_array[MAX_OPEN_FILES];

int simplefs_create(char *filename) {
        if (strlen(filename) > MAX_NAME_STRLEN) {
                return -1;
        }

        struct inode_t inode;
        for (int i = 0; i < NUM_INODES; i++) {
                simplefs_readInode(i, &inode);
                if (inode.status == INODE_IN_USE && strcmp(inode.name, filename) == 0) {
                        return -1;
                }
        }

        int inodenum = simplefs_allocInode();
        if (inodenum == -1) {
                return -1;
        }

        simplefs_readInode(inodenum, &inode);
        inode.status = INODE_IN_USE;
        strncpy(inode.name, filename, MAX_NAME_STRLEN);
        inode.file_size = 0;
        for (int i = 0; i < MAX_FILE_SIZE; i++) {
                inode.direct_blocks[i] = -1;
        }

        simplefs_writeInode(inodenum, &inode);

        return inodenum;
}

void simplefs_delete(char *filename) {
        struct inode_t inode;
        int inodenum = -1;

        for (int i = 0; i < NUM_INODES; i++) {
                simplefs_readInode(i, &inode);
                if (inode.status == INODE_IN_USE && strcmp(inode.name, filename) == 0) {
                        inodenum = i;
                        break;
                }
        }

        if (inodenum == -1) {
                return;
        }

        for (int i = 0; i < MAX_FILE_SIZE; i++) {
                if (inode.direct_blocks[i] != -1) {
                        simplefs_freeDataBlock(inode.direct_blocks[i]);
                }
        }

        simplefs_freeInode(inodenum);
}

int simplefs_open(char *filename) {
        struct inode_t inode;
        int inodenum = -1;

        for (int i = 0; i < NUM_INODES; i++) {
                simplefs_readInode(i, &inode);
                if (inode.status == INODE_IN_USE && strcmp(inode.name, filename) == 0) {
                        inodenum = i;
                        break;
                }
        }

        if (inodenum == -1) {
                return -1;
        }

        int file_handle = -1;
        for (int i = 0; i < MAX_OPEN_FILES; i++) {
                if (file_handle_array[i].inode_number == -1) {
                        file_handle = i;
                        break;
                }
        }

        if (file_handle == -1) {
                return -1;
        }

        file_handle_array[file_handle].inode_number = inodenum;
        file_handle_array[file_handle].offset = 0;

        return file_handle;
}

void simplefs_close(int file_handle) {
        if (file_handle < 0 || file_handle >= MAX_OPEN_FILES ||
            file_handle_array[file_handle].inode_number == -1) {
                return;
        }

        file_handle_array[file_handle].inode_number = -1;
        file_handle_array[file_handle].offset = 0;
}

int simplefs_read(int file_handle, char *buf, int nbytes) {
        if (file_handle < 0 || file_handle >= MAX_OPEN_FILES ||
            file_handle_array[file_handle].inode_number == -1) {
                return -1;
        }

        int inodenum = file_handle_array[file_handle].inode_number;
        int offset = file_handle_array[file_handle].offset;

        struct inode_t inode;
        simplefs_readInode(inodenum, &inode);

        if (offset + nbytes > inode.file_size) {
                return -1;
        }

        int start_block = offset / BLOCKSIZE;
        int start_offset = offset % BLOCKSIZE;
        int end_block = (offset + nbytes - 1) / BLOCKSIZE;
        int end_offset = (offset + nbytes - 1) % BLOCKSIZE;

        if (end_block >= MAX_FILE_SIZE) {
                return -1;
        }

        int bytes_read = 0;
        char block_data[BLOCKSIZE];

        if (start_block == end_block) {
                simplefs_readDataBlock(inode.direct_blocks[start_block], block_data);
                memcpy(buf, block_data + start_offset, nbytes);
                bytes_read = nbytes;
        } else {
                simplefs_readDataBlock(inode.direct_blocks[start_block], block_data);
                int bytes_from_first = BLOCKSIZE - start_offset;
                memcpy(buf, block_data + start_offset, bytes_from_first);
                bytes_read += bytes_from_first;

                for (int i = start_block + 1; i < end_block; i++) {
                        simplefs_readDataBlock(inode.direct_blocks[i], block_data);
                        memcpy(buf + bytes_read, block_data, BLOCKSIZE);
                        bytes_read += BLOCKSIZE;
                }

                simplefs_readDataBlock(inode.direct_blocks[end_block], block_data);
                int bytes_from_last = end_offset + 1;
                memcpy(buf + bytes_read, block_data, bytes_from_last);
                bytes_read += bytes_from_last;
        }

        return 0;
}

int simplefs_write(int file_handle, char *buf, int nbytes) {
        if (file_handle < 0 || file_handle >= MAX_OPEN_FILES ||
            file_handle_array[file_handle].inode_number == -1) {
                return -1;
        }

        int inodenum = file_handle_array[file_handle].inode_number;
        int offset = file_handle_array[file_handle].offset;

        struct inode_t inode;
        simplefs_readInode(inodenum, &inode);

        int start_block = offset / BLOCKSIZE;
        int start_offset = offset % BLOCKSIZE;
        int end_block = (offset + nbytes - 1) / BLOCKSIZE;
        int end_offset = (offset + nbytes - 1) % BLOCKSIZE;

        if (end_block >= MAX_FILE_SIZE) {
                return -1;
        }

        int new_blocks[MAX_FILE_SIZE] = {-1, -1, -1, -1};
        int num_new_blocks = 0;

        for (int i = start_block; i <= end_block; i++) {
                if (inode.direct_blocks[i] == -1) {
                        int new_block = simplefs_allocDataBlock();
                        if (new_block == -1) {
                                for (int j = 0; j < num_new_blocks; j++) {
                                        if (new_blocks[j] != -1) {
                                                simplefs_freeDataBlock(new_blocks[j]);
                                        }
                                }
                                return -1;
                        }
                        inode.direct_blocks[i] = new_block;
                        new_blocks[num_new_blocks++] = new_block;
                }
        }

        int bytes_written = 0;
        char block_data[BLOCKSIZE];

        if (start_block == end_block) {
                simplefs_readDataBlock(inode.direct_blocks[start_block], block_data);
                memcpy(block_data + start_offset, buf, nbytes);
                simplefs_writeDataBlock(inode.direct_blocks[start_block], block_data);
                bytes_written = nbytes;
        } else {
                simplefs_readDataBlock(inode.direct_blocks[start_block], block_data);
                int bytes_to_first = BLOCKSIZE - start_offset;
                memcpy(block_data + start_offset, buf, bytes_to_first);
                simplefs_writeDataBlock(inode.direct_blocks[start_block], block_data);
                bytes_written += bytes_to_first;

                for (int i = start_block + 1; i < end_block; i++) {
                        memcpy(block_data, buf + bytes_written, BLOCKSIZE);
                        simplefs_writeDataBlock(inode.direct_blocks[i], block_data);
                        bytes_written += BLOCKSIZE;
                }

                simplefs_readDataBlock(inode.direct_blocks[end_block], block_data);
                int bytes_to_last = end_offset + 1;
                memcpy(block_data, buf + bytes_written, bytes_to_last);
                simplefs_writeDataBlock(inode.direct_blocks[end_block], block_data);
                bytes_written += bytes_to_last;
        }

        if (offset + nbytes > inode.file_size) {
                inode.file_size = offset + nbytes;
        }

        simplefs_writeInode(inodenum, &inode);

        return 0;
}

int simplefs_seek(int file_handle, int nseek) {
        if (file_handle < 0 || file_handle >= MAX_OPEN_FILES ||
            file_handle_array[file_handle].inode_number == -1) {
                return -1;
        }

        int inodenum = file_handle_array[file_handle].inode_number;
        int current_offset = file_handle_array[file_handle].offset;

        struct inode_t inode;
        simplefs_readInode(inodenum, &inode);

        int new_offset = current_offset + nseek;

        if (new_offset < 0 || new_offset > inode.file_size) {
                return -1;
        }

        file_handle_array[file_handle].offset = new_offset;

        return 0;
}

