#ifndef FILE_LOADER_SNAP_H
#define FILE_LOADER_SNAP_H

#define FILE_LOADER_MAX_PATH 1024

// Function to get the full file path based on the Snap environment
const char *GetFilePath(const char *relativePath);

// Function to load a file into memory
char *LoadFile(const char *relativePath);

#endif // FILE_LOADER_SNAP_H
