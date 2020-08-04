#define   SYNC_WORD         (long) 0xfff
#define   SYNC_WORD_LNGTH   12

#include <stdio.h>

/* General Definitions */

#define	FAR /*far*/

#ifdef  MS_DOS
#define         FLOAT                   double
#else
#define         FLOAT                   float
#endif

#ifndef FALSE
# define        FALSE                   0
#endif
#ifndef TRUE
# define        TRUE                    1
#endif
#define         NULL_CHAR               '\0'

#define         MAX_U_32_NUM            0xFFFFFFFF
#ifndef PI
#define         PI                      3.14159265358979
#endif

/*#define       PI4                     PI/4*/
/*#define       PI64                    PI/64*/
#define         PI4                     0.78539816339744
#define         PI12                    0.261799388
#define         PI36                    0.087266463
#define         PI64                    0.049087385
#define         LN_TO_LOG10             0.2302585093

#define         VOL_REF_NUM             0
#define         MAC_WINDOW_SIZE         24

#define         MONO                    1
#define         STEREO                  2
#define         BITS_IN_A_BYTE          8
#define         WORD                    16
#define         MAX_NAME_SIZE           81
#define         SBLIMIT                 32
#define         SSLIMIT                 18
#define         FFT_SIZE                1024
#define         HAN_SIZE                512
#define         SCALE_BLOCK             12
#define         SCALE_RANGE             64
#define         SCALE                   32767   /* was 32768 */
#define         CRC16_POLYNOMIAL        0x8005

/* MPEG Header Definitions - ID Bit Values */
#define         MPEG_AUDIO_ID           1
#define         MPEG_PHASE2_LSF         0       /* 1995-07-11 SHN */

/* MPEG Header Definitions - Mode Values */

#define         MPG_MD_STEREO           0
#define         MPG_MD_JOINT_STEREO     1
#define         MPG_MD_DUAL_CHANNEL     2
#define         MPG_MD_MONO             3

/* Mode Extention */

#define         MPG_MD_LR_LR             0
#define         MPG_MD_LR_I              1
#define         MPG_MD_MS_LR             2
#define         MPG_MD_MS_I              3
/* AIFF Definitions */

#define IFF_ID_FORM 0x464f524d /* "FORM" */
#define IFF_ID_AIFF 0x41494646 /* "AIFF" */
#define IFF_ID_COMM 0x434f4d4d /* "COMM" */
#define IFF_ID_SSND 0x53534e44 /* "SSND" */
#define IFF_ID_MPEG 0x4d504547 /* "MPEG" */

#define AIFF_FORM_HEADER_SIZE 12
#define AIFF_SSND_HEADER_SIZE 16


/* Select float or double precision*/
#ifdef FSINGLE
# define        REAL            float
#else
# define        REAL            double
#endif

#define         MINIMUM         4    /* Minimum size of the buffer in bytes */
#define         MAX_LENGTH      32   /* Maximum length of word written or
                                        read from bit stream */
#define         READ_MODE       0
#define         WRITE_MODE      1
#define         ALIGNING        8
#define         BINARY          0
#define         ASCII           1
#ifndef BS_FORMAT
#define         BS_FORMAT       ASCII /* BINARY or ASCII = 2x bytes */
#endif

#ifndef MIN
#define         MIN(A, B)       ((A) < (B) ? (A) : (B))
#endif
#ifndef MAX
#define         MAX(A, B)       ((A) > (B) ? (A) : (B))
#endif

struct MP3Data {
    long int data[SBLIMIT+1][SSLIMIT];
};

struct MP3DeQData {
    REAL data[SBLIMIT][SSLIMIT];
};

/***********************************************************************
*
*  Global Type Definitions
*
***********************************************************************/
typedef struct {
        char    encoded_file_name[MAX_NAME_SIZE];
        char    decoded_file_name[MAX_NAME_SIZE];
        int  need_aiff;
        int  need_esps;
        int  write_to_file;
        int  topSb;
        /* Added Johan H*/
        int  verbose;
        int  forkoff;
} Arguments_t;

/* Structure for Reading Layer II Allocation Tables from File */

typedef struct {
    unsigned int    steps;
    unsigned int    bits;
    unsigned int    group;
    unsigned int    quant;
} sb_alloc, *alloc_ptr;

typedef sb_alloc        al_table[SBLIMIT][16];

/* Header Information Structure */

typedef struct {
    int version;
    int lay;
    int error_protection;
    int bitrate_index;
    int sampling_frequency;
    int padding;
    int extension;
    int mode;
    int mode_ext;
    int copyright;
    int original;
    int emphasis;
} layer, *the_layer;

/* Parent Structure Interpreting some Frame Parameters in Header */

typedef struct {
    layer       header;        /* raw header information */
    int         actual_mode;    /* when writing IS, may forget if 0 chs */
    al_table    alloc;         /* bit allocation table read in */
    int         tab_num;        /* number of table as loaded */
    int         stereo;         /* 1 for mono, 2 for stereo */
    int         jsbound;        /* first band of joint stereo coding */
    int         sblimit;        /* total number of sub bands */
} frame_params;

/* Double and SANE Floating Point Type Definitions */

typedef struct  IEEE_DBL_struct {
    unsigned long   hi;
    unsigned long   lo;
} IEEE_DBL;

typedef struct  SANE_EXT_struct {
    unsigned long   l1;
    unsigned long   l2;
    unsigned short  s1;
} SANE_EXT;

/* "bit_stream.h" Type Definitions */

typedef struct  bit_stream_struc {
    FILE        *pt;            /* pointer to bit stream device */
    unsigned char buf[4096];         /* bit stream buffer */
    int         buf_size;       /* size of buffer (in number of bytes) */
    long        totbit;         /* bit counter of bit stream */
    int         buf_byte_idx;   /* pointer to top byte in buffer */
    int         buf_bit_idx;    /* pointer to top bit of top byte in buffer */
    int         mode;           /* bit stream open in read or write mode */
    int         eob;            /* end of buffer index */
    int         eobs;           /* end of bit stream flag */
    char        format;

    /* format of file in rd mode (BINARY/ASCII) */
} Bit_stream_struc;

/* Layer III side information. */
typedef struct {
        unsigned part2_3_length;
        unsigned big_values;
        unsigned global_gain;
        unsigned scalefac_compress;
        unsigned window_switching_flag;
        unsigned block_type;
        unsigned mixed_block_flag;
        unsigned table_select[3];
        unsigned subblock_gain[3];
        unsigned region0_count;
        unsigned region1_count;
        unsigned preflag;
        unsigned scalefac_scale;
        unsigned count1table_select;
} my_gr_info ;


typedef struct {
        unsigned part2_3_length;
        unsigned big_values;
        unsigned global_gain;
        unsigned scalefac_compress;
        unsigned window_switching_flag;
        unsigned block_type;
        unsigned mixed_block_flag;
        unsigned table_select[3];
        unsigned subblock_gain[3];
        unsigned region0_count;
        unsigned region1_count;
        unsigned preflag;
        unsigned scalefac_scale;
        unsigned count1table_select;
} gr_info_s;

typedef struct {
        unsigned main_data_begin;
        unsigned private_bits;
        struct {
		unsigned scfsi[4];
		gr_info_s gr[2];
        } ch[2];
} III_side_info_t;

typedef struct {
        int l[23];            /* [cb] */
        int s[3][13];         /* [window][cb] */
} III_scalefac_t[2];  /* [ch] */

struct MP3SampleData {
    short data[SSLIMIT][SBLIMIT];
};


typedef struct {
    struct MP3Data output;
    III_scalefac_t scaleFactor;
    gr_info_s groupInformation;
    frame_params frameHeader;
    int channel;
} Packet1;

typedef struct {
    struct MP3DeQData output;
    III_scalefac_t scaleFactor;
    gr_info_s groupInformation;
    frame_params frameHeader;
    int channel;
} Packet3;

typedef struct {
    struct MP3SampleData output[2];
    frame_params frameHeader;
} Packet4;
