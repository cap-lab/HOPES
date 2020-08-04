#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef struct 
{                                                                                                                     
   byte data[(IMG_HEIGHT)][(IMG_WIDTH)];
} Frame;

typedef struct
{
    byte data[(IMG_HEIGHT)/2][(IMG_WIDTH)/2];
} HalfFrame;

typedef struct 
{ 
    int x; 
    int y; 
    int refframe; 
} MotionVector;

typedef struct
{
    Macroblock mbdata[(IMG_WIDTH/16)*(IMG_HEIGHT/16)];
    int refFrameArr[IMG_HEIGHT/4][IMG_WIDTH/4];
    int mv[IMG_WIDTH/4 + 4][IMG_HEIGHT/4][3];
} FrameInfo;

typedef struct {                                                                                        
    Frame Y;
    HalfFrame U;
    HalfFrame V;
} YUVFrame;  

typedef struct {                                                                                        
    Frame frame;
    FrameInfo frameinfo; 
    DeblockParams deblockparams;                                                                                   
} ResultFrame;  

typedef struct framepacket{                                                                                        
    Bitstream currStream;                                                                                          
    SliceParams sliceparams;                                                                                       
    DeblockParams deblockparams;                                                                                   
} FramePacket;  

typedef struct mbpacket_y{
    int block[4][4];
    int mode;
    int mode16;
    int available[4];
    MotionVector mv;
    int ret;
} MacroBlockPacket_Y;

typedef struct mbpacket_uv{
    int block[4][4];
    int mode;
    MotionVector mv[4];
    int ret[4];
} MacroBlockPacket_UV;

typedef struct frame_y{
	MacroBlockPacket_Y blocks[396 * 4];
    DeblockParams deblockparams;
    FrameInfo frameinfo;
} FramePacket_Y;

typedef struct frame_uv{
	MacroBlockPacket_UV blocks[396];
} FramePacket_UV;

#endif
