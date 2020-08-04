#ifndef _H264DECODER_H_
#define _H264DECODER_H_

#define IMG_HEIGHT 144
#define IMG_WIDTH 176
#define BUF_CYCLE 5

#define BLOCK_SIZE      4
#define MB_BLOCK_SIZE   16

#define MAX_MB_NR ((IMG_WIDTH*IMG_HEIGHT) / (MB_BLOCK_SIZE*MB_BLOCK_SIZE))


#define MAX_CODED_FRAME_SIZE 800000  //!< bytes for one frame
#define ET_SIZE 300 	//!< size of error text buffer


//////////////////////////////
// defines.h /////////////////
//////////////////////////////

#define  MAX_LENGTH_POC_CYCLE   10              //max no in type 1 poc cycle

// CAVLC
#define LUMA              0
#define LUMA_INTRA16x16DC 1
#define LUMA_INTRA16x16AC 2

#define TOTRUN_NUM    15
#define RUNBEFORE_NUM  7


//--- block types for CABAC ----
#define LUMA_16DC       0
#define LUMA_16AC       1
#define LUMA_8x8        2
#define LUMA_8x4        3
#define LUMA_4x8        4
#define LUMA_4x4        5
#define CHROMA_DC       6
#define CHROMA_AC       7
#define NUM_BLOCK_TYPES 8


#define _ADAPT_LAST_GROUP_

#define MAX_INFO_WORD  300000               //!< for one frame
#define MAXIMUM_UVLC_CODEWORD_PER_HEADER 20 //!< UVLC codewords per combined picture/slice header maximum
#define TRACE           0                   //!< 0:Trace off 1:Trace on

#define absm(A) ((A)<(0) ? (-(A)):(A))      //!< abs macro, faster than procedure
#define MAX_VALUE       999999              //!< used for start value for some variables
#define Clip1(a)            ((a)>255?255:((a)<0?0:(a)))
#define Clip3(min,max,val) (((val)<(min))?(min):(((val)>(max))?(max):(val)))

#define P8x8    8
#define I4MB    9
#define I16MB   10
#define IBLOCK  11
#define SI4MB   12
#define MAXMODE 13

#define IS_INTRA(MB)    ((MB)->mb_type==I4MB  || (MB)->mb_type==I16MB)
#define IS_NEWINTRA(MB) ((MB)->mb_type==I16MB)
#define IS_OLDINTRA(MB) ((MB)->mb_type==I4MB)
#define IS_INTER(MB)    ((MB)->mb_type!=I4MB  && (MB)->mb_type!=I16MB)
#define IS_INTERMV(MB)  ((MB)->mb_type!=I4MB  && (MB)->mb_type!=I16MB       && (MB)->mb_type!=0)
#define IS_DIRECT(MB)   ((MB)->mb_type==0     && (img->   type==    B_SLICE ))
#define IS_COPY(MB)     ((MB)->mb_type==0     && (img->   type==P_SLICE || img->type==SP_SLICE))
#define IS_P8x8(MB)     ((MB)->mb_type==P8x8)


// Quantization parameter range

#define MIN_QP          0
#define MAX_QP          51


#define NO_INTRA_PMODE  9        //!< #intra prediction modes
/* 4x4 intra prediction modes */
#define VERT_PRED             0
#define HOR_PRED              1
#define DC_PRED               2
#define DIAG_DOWN_LEFT_PRED   3
#define DIAG_DOWN_RIGHT_PRED  4
#define VERT_RIGHT_PRED       5
#define HOR_DOWN_PRED         6
#define VERT_LEFT_PRED        7
#define HOR_UP_PRED           8

// 16x16 intra prediction modes
#define VERT_PRED_16    0
#define HOR_PRED_16     1
#define DC_PRED_16      2
#define PLANE_16        3

// 8x8 chroma intra prediction modes
#define DC_PRED_8       0
#define HOR_PRED_8      1
#define VERT_PRED_8     2
#define PLANE_8         3


#define INIT_FRAME_RATE 30
#define EOS             1                       //!< End Of Sequence
#define SOP             2                       //!< Start Of Picture
#define SOS             3                       //!< Start Of Slice

#define EOS_MASK        0x01       //!< mask for end of sequence (bit 1)
#define ICIF_MASK       0x02       //!< mask for image format (bit 2)
#define QP_MASK         0x7C       //!< mask for quant.parameter (bit 3->7)
#define TR_MASK         0x7f80     //!< mask for temporal referance (bit 8->15)

#define DECODING_OK     0
#define SEARCH_SYNC     1
#define PICTURE_DECODED 2

#define MIN_PIX_VAL     0           //!< lowest legal values for 8 bit sample
#define MAX_PIX_VAL     255         //!< highest legal values for 8 bit sample

#define max(a, b)      ((a) > (b) ? (a) : (b))  //!< Macro returning max value
#define min(a, b)      ((a) < (b) ? (a) : (b))  //!< Macro returning min value
#define mmax(a, b)      ((a) > (b) ? (a) : (b)) //!< Macro returning max value
#define mmin(a, b)      ((a) < (b) ? (a) : (b)) //!< Macro returning min value
#define clamp(a,b,c) ( (a)<(b) ? (b) : ((a)>(c)?(c):(a)) )    //!< clamp a to the range of [b;c]


#define MVPRED_MEDIAN   0
#define MVPRED_L        1
#define MVPRED_U        2
#define MVPRED_UR       3

#define DECODE_COPY_MB  0
#define DECODE_MB       1

#define BLOCK_MULTIPLE      (MB_BLOCK_SIZE/BLOCK_SIZE)

#define MAX_SYMBOLS_PER_MB  1200  //!< Maximum number of different syntax elements for one MB

#define MAX_PART_NR   3      /*!< Maximum number of different data partitions.
                                Some reasonable number which should reflect
                                what is currently defined in the SE2Partition
                                map (elements.h) */

// Interim File Format: define the following macro to identify which version is 
//                      used in the implementation

#define WORKING_DRAFT_MAJOR_NO 0  // inidicate the working draft version number
#define WORKING_DRAFT_MINOR_NO 4
#define INTERIM_FILE_MAJOR_NO 0   // indicate interim file format version number
#define INTERIM_FILE_MINOR_NO 1

//Start code and Emulation Prevention need this to be defined in identical manner at encoder and decoder
#define ZEROBYTES_SHORTSTARTCODE 2 //indicates the number of zero bytes in the short start-code prefix


// NALUCOMMON_H
#define MAXRBSPSIZE 64000

#define NALU_TYPE_SLICE   1
#define NALU_TYPE_DPA     2
#define NALU_TYPE_DPB     3
#define NALU_TYPE_DPC     4
#define NALU_TYPE_IDR     5
#define NALU_TYPE_SEI     6
#define NALU_TYPE_SPS     7
#define NALU_TYPE_PPS     8
#define NALU_TYPE_PD      9
#define NALU_TYPE_FILL    10

#define NALU_PRIORITY_HIGHEST     3
#define NALU_PRIORITY_HIGH        2
#define NALU_PRIRITY_LOW          1
#define NALU_PRIORITY_DISPOSABLE  0
// NALUCOMMON_H

typedef unsigned char byte;    //!<  8 bit unsigned
typedef int           int32;
typedef unsigned int  u_int32;

#ifdef FALSE
	#undef FALSE
#endif

#ifdef TRUE
	#undef TRUE
#endif

//! Boolean Type
typedef enum { FALSE, TRUE } Boolean;
#define Boolean int


/////////////////////////////////
// global.h /////////////////////
/////////////////////////////////

//! definition of H.26L syntax elements
typedef enum {
  SE_HEADER,
  SE_PTYPE,
  SE_MBTYPE,
  SE_REFFRAME,
  SE_INTRAPREDMODE,
  SE_MVD,
  SE_CBP_INTRA,
  SE_LUM_DC_INTRA,
  SE_CHR_DC_INTRA,
  SE_LUM_AC_INTRA,
  SE_CHR_AC_INTRA,
  SE_CBP_INTER,
  SE_LUM_DC_INTER,
  SE_CHR_DC_INTER,
  SE_LUM_AC_INTER,
  SE_CHR_AC_INTER,
  SE_DELTA_QUANT_INTER,
  SE_DELTA_QUANT_INTRA,
  SE_BFRAME,
  SE_EOS,
  SE_MAX_ELEMENTS //!< number of maximum syntax elements, this MUST be the last one!
} SE_type;        // substituting the definitions in element.h

typedef enum {
  INTER_MB,
  INTRA_MB_4x4,
  INTRA_MB_16x16
} IntraInterDecision;

typedef enum {
  BITS_TOTAL_MB,
  BITS_HEADER_MB,
  BITS_INTER_MB,
  BITS_CBP_MB,
  BITS_COEFF_Y_MB,
  BITS_COEFF_UV_MB,
  MAX_BITCOUNTER_MB
} BitCountType;

typedef enum {
  NO_SLICES,
  FIXED_MB,
  FIXED_RATE,
  CALLBACK,
  FMO
} SliceMode;


typedef enum {
  UVLC,
  CABAC
} SymbolMode;


typedef enum {
  FRAME,
  TOP_FIELD,
  BOTTOM_FIELD
} PictureType;           //!< New enum for field processing

typedef enum {
  P_SLICE = 0,
  B_SLICE,
  I_SLICE,
  SP_SLICE,
  SI_SLICE
} SliceType;


/***********************************************************************
 * N e w   D a t a    t y p e s   f o r    T M L
 ***********************************************************************
 */

//! Bitstream
typedef struct
{
  int    frame_bitoffset;    //!< actual position in the codebuffer, bit-oriented, UVLC only
  int    bitstream_length;   //!< over codebuffer legnth, byte oriented, UVLC only
  byte   streamBuffer[MAX_CODED_FRAME_SIZE];  //!< actual codebuffer for read bytes
  //int    ei_flag;            //!< error indication, 0: no error, else unspecified error
} Bitstream;

//! Slice
typedef struct
{
  int                 LFDisableIdc;     //!< Disable loop filter on slice
  int                 LFAlphaC0Offset;  //!< Alpha and C0 offset for filtering slice
  int                 LFBetaOffset;     //!< Beta offset for filtering slice
} Slice;

//! Syntaxelement
typedef struct syntaxelement
{
  int    type;   //!< type of syntax element for data part
  int    value1; //!< numerical value of syntax element
  int    value2; //!< for blocked symbols, e.g. run/level
  int    len;    //!< length of code
  int    inf;    //!< info part of UVLC code
  unsigned int bitpattern;  //!< UVLC bitpattern

  //! for mapping of UVLC to syntaxElement
  void  (*mapping)(int len, int info, int *value1, int *value2);
} SyntaxElement;

//! Macroblock
typedef struct macroblock
{
  int           qp;
  int           slice_nr;
  int           delta_quant;          //!< for rate control
  struct macroblock   *mb_available[3][3]; /*!< pointer to neighboring MBs in a 3x3 window of current MB, which is located at [1][1]
  NULL pointer identifies neighboring MBs which are unavailable */

  // some storage of macroblock syntax elements for global access
  int           mb_type;
  int           mvd[2][BLOCK_MULTIPLE][BLOCK_MULTIPLE][2];      //!< indices correspond to [forw,backw][block_y][block_x][x,y]
  int           cbp, cbp_blk ;
  unsigned long cbp_bits;

  int           i16mode;
  int           b8mode[4];
  int           b8pdir[4];
  int           ei_flag;

  int           c_ipred_mode;       //!< chroma intra prediction mode
  int           mb_field;
} Macroblock;

typedef struct
{
  int type;
  int qp;
  int num_ref_pic_active_fwd;       //!< number of forward reference
  int num_ref_pic_active_bwd;       //!< number of backward reference

} SliceParams;


// image parameters
typedef struct img_par
{
  int number;                          //<! frame number
  int pn;                              //<! short term picture number
  int tr;                       //<! temporal reference, 8 bit, wrapps at 255
  int qp;                       //<! quant for the current frame
  //int qpsp;                     //<! quant for SP-picture predicted frame
  //int sp_switch;                //<! 1 for switching sp, 0 for normal sp
  int direct_type;              //<! 1 for Spatial Direct, 0 for Temporal
  int type;                     //<! image type INTER/INTRA
//  int mb_y;
//  int mb_x;
//  int block_y;
//  int pix_y;
//  int pix_x;
//  int pix_c_y;
//  int block_x;
//  int pix_c_x;

  int allrefzero;
//  int mpr[16][16];                            //<! predicted block

//  int m7[16][16];                             //<! final 4x4 block. Extended to 16x16 for ABT
//  int cof[4][6][4][4];                        //<! correction coefficients from predicted
//  int cofu[4];
  int constrained_intra_pred_flag;            //<! if 1, prediction only from other Intra MBs
//  int cod_counter;                            //<! Current count of number of skipped macroblocks in a row

  int structure;                               //<! Identify picture structure type

  // For MB level frame/field coding
  int mb_frame_field_flag;
  int mb_field;

  int disposable_flag;                          //!< flag for disposable frame, 1:disposable
  int num_ref_pic_active_fwd;                   //!< number of forward reference
  int num_ref_pic_active_bwd;                   //!< number of backward reference

  // JVT-D095, JVT-D097
  int num_slice_groups_minus1; 
  int mb_allocation_map_type; 
  int top_left_mb; 
  int bottom_right_mb; 
  int slice_group_change_direction; 
  int slice_group_change_rate_minus1; 
  int slice_group_change_cycle;
  // End JVT-D095, JVT-D097

  // JVT-D101
  int redundant_slice_flag; 
  int redundant_pic_cnt; 

  int explicit_B_prediction;



  // End JVT-D101
  // POC200301: from unsigned int to int
           int toppoc;      //poc for this top field // POC200301
           int bottompoc;   //poc of bottom field of frame
  //         int framepoc;    //poc of this frame // POC200301
  unsigned int frame_num;   //frame_num for this frame
  unsigned int field_pic_flag;
  unsigned int bottom_field_flag;
  
  //the following should probably go in sequence parameters
  // unsigned int log2_max_frame_num_minus4;
  unsigned int pic_order_cnt_type;
  // for poc mode 0, POC200301
  // unsigned int log2_max_pic_order_cnt_lsb_minus4;  
  // for poc mode 1, POC200301
  unsigned int delta_pic_order_always_zero_flag;
           int offset_for_non_ref_pic;
           int offset_for_top_to_bottom_field;
  unsigned int num_ref_frames_in_pic_order_cnt_cycle;
           int offset_for_ref_frame[MAX_LENGTH_POC_CYCLE];

  // POC200301
  //the following is for slice header syntax elements of poc
  // for poc mode 0.
  unsigned int pic_order_cnt_lsb;
           int delta_pic_order_cnt_bottom;
  // for poc mode 1.
           int delta_pic_order_cnt[2];

  // ////////////////////////
  // for POC mode 0:
    signed int PicOrderCntMsb;
  unsigned int PrevPicOrderCntLsb;
    signed int CurrPicOrderCntMsb;
  // for POC mode 1:
  unsigned int AbsFrameNum;
    signed int ExpectedPicOrderCnt, PicOrderCntCycleCnt, FrameNumInPicOrderCntCycle;
  unsigned int PreviousFrameNum, FrameNumOffset, ExpectedDeltaPerPicOrderCntCycle;
  unsigned int Previousfield_pic_flag,Previousbottom_field_flag,Previousnal_reference_idc;
  //unsigned int Previousdelta_pic_order_cnt[2], PreviousPOC, ThisPOC, FirstFieldType;
  unsigned int Previousdelta_pic_order_cnt[2], FirstFieldType;
  // /////////////////////////


  //weighted prediction
  unsigned int weighted_pred_flag;
  unsigned int weighted_bipred_idc;
  unsigned int luma_log_weight_denom;
  unsigned int chroma_log_weight_denom;
  unsigned int apply_weights;

  //the following should probably go in picture parameters
  unsigned int pic_order_present_flag;
  
  //the following are sent in the slice header
  int idr_flag;
  int idr_pic_id;
  int MaxFrameNum;

  unsigned PicWidthInMbs;
  unsigned PicHeightInMapUnits;
  unsigned FrameHeightInMbs;
  unsigned PicHeightInMbs;
  unsigned PicSizeInMbs;
  unsigned FrameSizeInMbs;

  int no_output_of_prior_pics_flag;
  int adaptive_ref_pic_buffering_flag;

  int model_number;

} ImageParameters;



typedef struct {
  int      LFDisableIdc;     //!< Disable loop filter on slice
  int      LFAlphaC0Offset;  //!< Alpha and C0 offset for filtering slice
  int      LFBetaOffset;     //!< Beta offset for filtering slice
} DeblockParams;



//char errortext[ET_SIZE];
/*
void error(char *text, int code);
void no_mem_exit(char *where);
*/

/////////////////////
// vlc.h ////////////
/////////////////////

// UVLC mapping
/*
void linfo_ue(int len, int info, int *value1, int *dummy);
void linfo_se(int len, int info, int *value1, int *dummy);

int  GetVLCSymbol (byte buffer[],int totbitoffset,int *info, int bytecount);
int readSyntaxElement_FLC(SyntaxElement *sym, Bitstream *currStream);
int GetBits (byte buffer[],int totbitoffset,int *info, int bytecount, 
             int numbits);
*/

#endif
