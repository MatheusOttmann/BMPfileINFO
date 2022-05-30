#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <errno.h>

struct BMPHeader 
{             
  uint16_t  type;             
  uint32_t  size;            
  uint16_t  reserved1;        
  uint16_t  reserved2;       
  uint32_t  offset;     
} __attribute__((packed));

struct DMPheader
{
  uint32_t  headerSize;             
  uint32_t  width;            
  uint32_t  height;        
  uint16_t  colorPlane;       
  uint16_t  bitsPerPix;  

}
__attribute__((packed));




int findSizeDir(const char folder[]);
long int findSize(char file_name[]);
int validateBMP(const char file_name[], long int foundSize);
struct DMPheader readingFile( const char file_name[], struct DMPheader fileInfo);
void printCVS(const char folder[], FILE *file);

void main()
{
    FILE *fpt;
    fpt = fopen("MyFile.csv", "w+");
    printf( "Enter the path of the Folder (must be in ./notation, must be a folder in working directory) : \n");
    char str[100];
    scanf("%s", str);
    printf("Path is %s\n", str);
    int numbOfFolders = findSizeDir(str);
    printf("This folder contains %d files\n", numbOfFolders);
    fprintf(fpt,"Name | Size(in KB) | Width (in B) | Height(in B)\n");
    printCVS(str, fpt); 
}


void printCVS(const char folder[], FILE *file)
{
    char folderFirst[100];
    strncpy(folderFirst, folder, sizeof(folderFirst));
    struct DMPheader information;            
    struct dirent *de; 
    DIR *dr = opendir(folder);
    if (dr == NULL)
    {
        printf("Could not open current directory\n"); 
        return;
    }
    while ((de = readdir(dr)) != NULL)
    {
            if(de-> d_type != DT_DIR)
            { 
                printf("File name is %s\n", de->d_name);  
                strcat(folderFirst, "/");
                strcat(folderFirst, de->d_name);
                printf("FIle name concanated is %s \n", folderFirst);           
                long int size = findSize(folderFirst);
                if(validateBMP(folderFirst, size) != 1)
                {
                    printf("BMP is corrupt\n");
                }
                else
                {
                    printf("The size of image is %lu\n", size);
                    information = readingFile(folderFirst, information);
                    fprintf(file, "%s   | %lu   | %u   | %u   \n",de->d_name, size, information.width, information.height); 
                }
                strncpy(folderFirst, folder, sizeof(folderFirst));                      
            }       
        }
    closedir(dr); 

}

int validateBMP( const char file_name[] , long int foundSize)
{

    FILE* fp = fopen(file_name , "r");
    struct BMPHeader header;
    size_t returnValue = fread(&header, sizeof(header), 1, fp); 
    if(returnValue == 1)
    {
        if(foundSize == header.size)
        {
            return 1;
        }
        else
            return -1;
    }
    else
        return -1;    
    fclose(fp);
}

struct DMPheader readingFile( const char file_name[], struct DMPheader fileInfo)
{
    struct DMPheader empty;
    FILE* fp = fopen(file_name , "r");
    fseek(fp, 14, SEEK_SET);
    size_t returnValue = fread(&fileInfo, sizeof(fileInfo), 1, fp); 
    if(returnValue == 1)
    {
        printf("The height of the file is %u\n" , fileInfo.height);
        printf("The width of the file is %u \n", fileInfo.width);
        fclose(fp);
        return fileInfo;
    }
    else
    {
        fclose(fp);
        return empty;
    }
}



long int findSize(char file_name[])
{
    FILE* fp = fopen(file_name, "r");

    if (fp == NULL)
     {
        printf("Error \n");
        return -1;
    }
  
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
  
    fclose(fp);
  
    return res;
}


int findSizeDir(const char folder[])
{
    struct dirent *de; 
    DIR *dr = opendir(folder);
    int count = 0;
  
    if (dr == NULL)
    {
        printf("Could not open current directory");
        return 0;
    }
    while ((de = readdir(dr)) != NULL)
    {
            if(de-> d_type != DT_DIR)
            { 
                count++;
            }       
        }
    closedir(dr); 
    return count;   
}
