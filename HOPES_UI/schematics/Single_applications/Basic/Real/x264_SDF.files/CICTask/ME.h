#include "stdint.h"
#define COST_MAX (1<<28)

struct IntArr4
{
    int data[4];
};
struct Uint8_tArr256
{
    uint8_t data[16*16];
};
struct preddata16
{
    uint8_t data[2][21];
};
struct IntArr9
{
    int data[9];
};
struct Uint8_tArr16
{
    uint8_t data[4*4];
};
struct preddata4
{
    uint8_t data[13];
};
struct IntBlock4x4
{
    int data[4][4];
};
struct Int16_tBlock4x4 
{
    int16_t data[4][4];
};
struct IntArr16
{
    int data[16];
};
struct IntArr15
{
    int data[15];
};
struct mv_mb
{
    int16_t data[2][48][2];
};
struct ref_mb
{
    int8_t data[2][48];
};
struct ME_ref
{
    uint8_t data[4][48*48];
};
struct ME_ref_chroma
{
    uint8_t data[2][24*24];
};
struct Uint8_tArr64
{
    uint8_t data[64];
};
struct IntArr4x2
{
    int data[4][2];
};
struct IntArr2
{
    int data[2];
};
struct IntArr4x4x2
{
    int data[4][4][2];
};
struct IntArr4x2x2
{
    int data[4][2][2];
};
struct IntArr4x4
{
    int data[4][4];
};
struct IntArr2x2
{
    int data[2][2];
};
struct preddata8 
{
    uint8_t data[2][9];
};
struct IntArr48 
{
    int data[48];
};
struct IntArr8x15
{
    int data[8][15];
};
struct IntArr2x4
{
    int data[2][4];
};
struct IntArr16x15
{
    int data[16][15];
};
struct IntBlock16x16
{
    int data[16][16];
};
struct mb_frm 
{ 
    int8_t data[(176+15)/16 * (144+15)/16];
};
struct nzc_frm 
{
    int data[(176+15)/16 * (144+15)/16][16+4+4];
};
struct ref_frm
{
    int8_t data[(176+15)/16 * (144+15)/16 * 4];
};
struct mv_frm 
{
    int16_t data[(176+15)/16 * (144+15)/16 * 2 * 16][2];
};
struct FrameLuma 
{
    uint8_t data[(176+64) * (144+64)];
};
struct FrameChroma 
{
    uint8_t data[(176/2 + 32) * (144/2 + 32)];
};


// added by jwlee declare
typedef struct {
    int result;
    int i_neighbour;
    struct preddata16 preddata16_Y;
    struct preddata8 preddata8_U;
    struct preddata8 preddata8_V;
    struct mv_mb mv_mb;
    struct ref_mb ref_mb;
    int b_fast_intra;
    struct IntArr48 i_pred4x4;
    struct IntArr48 non_zero_count;
    struct ME_ref ME_ref;
    struct ME_ref_chroma ME_ref_chroma;
// by iloy 2008.07.07
#if 0
	unsigned char pad[ 11328 - 11324 ];
#endif
} neighbour_info; // 11328 byte = 16 byte * 708
				  // 11392

typedef struct {
    int inter_do_it; // ! frame_type
    int intra_do_it; // frame_type
    struct Uint8_tArr256 src_block_Y;
    struct Uint8_tArr64 src_block_U;
    struct Uint8_tArr64 src_block_V;
    int i_lambda;
// by iloy 2008.07.07
#if 0
	unsigned char pad[400 -396 ];
#endif
} init_info; // 400 byte = 16 byte * 25
				// 512

typedef struct {
    struct IntArr48 non_zero_count;
    struct preddata16 preddata16_Y;
    struct preddata8 preddata8_U;
    struct preddata8 preddata8_V;
    struct ME_ref ME_ref;
    struct ME_ref_chroma ME_ref_chroma;
    struct IntArr4 i_sub_partition;
    int i_partition;
    struct mv_mb mv_mb;
    struct ref_mb ref_mb;
    int i_type;
    int i_pred16x16;
    int chroma_pred_mode;
    int mb_type;
    struct IntArr48 intra4x4_pred_mode;
	struct IntArr16 luma_4x4[16];
// by iloy 2008.07.07
#if 0
	unsigned char pad[ 12384 - 12372];
#endif
} encoder_send_info;  // 12384 byte = 16 * 774
					// 12416

typedef struct{
    struct mv_mb mv_mb;
    struct ref_mb ref_mb;
    struct IntBlock4x4 i_pred4x4;
    struct Uint8_tArr256 recon_block_4x4;
    int mb_type;
// by iloy 2008.07.07
#if 0
	unsigned char pad[ 816 - 804 ];
#endif
} deblock_send_info; // 816 byte = 16 byte * 51
					// 896
