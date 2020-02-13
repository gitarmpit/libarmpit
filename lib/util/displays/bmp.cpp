// Reads bmp file and generates c header 

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <stdint.h>

// typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
// typedef uint16_t WORD;    // WORD = unsigned 16 bit value
// typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value
// typedef long LONG;

using namespace std;

#define COLOR565(r,g,b) \
    ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3) )

#pragma pack(push, 2)
typedef struct _WinBMPFileHeader
{
    WORD   FileType;     /* File type, always 4D42h ("BM") */
    DWORD  FileSize;     /* Size of the file in bytes */
    WORD   Reserved1;    /* Always 0 */
    WORD   Reserved2;    /* Always 0 */
    DWORD  BitmapOffset; /* Starting position of image data in bytes */
} WINBMPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 2)

typedef struct _Win3xBitmapHeader
{
    DWORD Size;            /* Size of this header in bytes */
    LONG  Width;           /* Image width in pixels */
    LONG  Height;          /* Image height in pixels */
    WORD  Planes;          /* Number of color planes */
    WORD  BitsPerPixel;    /* Number of bits per pixel */
    /* Fields added for Windows 3.x follow this line */
    DWORD Compression;     /* Compression methods used */
    DWORD SizeOfBitmap;    /* Size of bitmap in bytes */
    LONG  HorzResolution;  /* Horizontal resolution in pixels per meter */
    LONG  VertResolution;  /* Vertical resolution in pixels per meter */
    DWORD ColorsUsed;      /* Number of colors in the image */
    DWORD ColorsImportant; /* Minimum number of important colors */
} WIN3XBITMAPHEADER;
#pragma pack(pop)


typedef struct _Win3xPaletteElement
{
    BYTE Blue;      /* Blue component */
    BYTE Green;     /* Green component */
    BYTE Red;       /* Red component */
    BYTE Reserved;  /* Padding (always 0) */
} WIN3XPALETTEELEMENT;


/*
COLOR     4bit
0 Black
1 Red­Brown
2 Green
3 Brown­Green
4 Dark Blue
5 Purple
6 Blue­Green
7 Dark Grey
8 Light Grey
9 Orange­Red
10 Vivid Green
11 Yellow
12 Blue
13 Pink­Purple
14 Light Blue
15 White 
*/


int main(int argc, char** argv) 
{
    if (argc != 2) 
    {
        printf ("no input file\n");
        exit(1);

    }
    
    WINBMPFILEHEADER hdr1;
    memset (&hdr1, 0, sizeof(hdr1));
    FILE* f = fopen (argv[1], "rb");
    if (f == NULL) 
    {
        printf ("error opening input file\n");
        exit(1);
    }
    

    char* p = argv[1] + strlen (argv[1]);
    while (*p-- != '.')
        ;

    char base[256] = {0};
    
    strcpy (base, argv[1]);
    base [ p - argv[1] + 1] = 0;

    char fname_out[256];
    strcpy (fname_out, base);
    strcat (fname_out, ".cpp");

    int rc = fread(&hdr1, sizeof(hdr1), 1, f);
    WIN3XBITMAPHEADER  hdr2;
    rc = fread(&hdr2, sizeof(hdr2), 1, f);

    FILE* fout = fopen (fname_out, "w");
    if (fout == NULL) 
    {
        printf ("error creating output file\n");
        exit(1);
    }

    printf ("%d bit  %dx%d\n\n", hdr2.BitsPerPixel, hdr2.Width, hdr2.Height);

    fprintf (fout, "//%d bit  %d x %d\n\n", hdr2.BitsPerPixel, hdr2.Width, hdr2.Height);

    fprintf (fout, "#include <stdint.h>\n\n");

    fprintf (fout, "const uint8_t %s[] = {\n", base);
    fseek(f, 0L, SEEK_END);
    DWORD sz = ftell(f);
    rewind(f);
    BYTE b1 = 0; 
    fprintf (fout, "\n    ");
    printf ("sz=%d\n", sz);
    DWORD i = 0;
    for (; i < sz; ++i) 
    {
        int rc = fread (&b1, sizeof(BYTE), 1, f);
        if (rc != 1) 
        {
            printf ("i = %d, pos=%d, rc=%d\n", i, ftell(f), rc);
            perror ("read error");
            exit(1);
        }
        if (i > 0) 
        {
            fprintf (fout, ", ");
            if ((i % 16) == 0) 
            {
                fprintf (fout, "\n    ");
            }
        }
        fprintf (fout, "0x%x", b1);
    }

    fprintf (fout, "\n};");
    fclose (fout);
    printf ("%d bytes written\n", i);
}