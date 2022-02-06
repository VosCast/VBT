#ifndef AAC_DLL_H
#define AAC_DLL_H

#include <stdint.h>
#include <fdk-aac/aacenc_lib.h>

typedef AACENC_ERROR(*aacEncOpenPtr)(HANDLE_AACENCODER*, UINT, UINT);
extern aacEncOpenPtr aacEncOpen_VBT;

typedef AACENC_ERROR(*aacEncoder_SetParamPtr)(HANDLE_AACENCODER, AACENC_PARAM, UINT);
extern aacEncoder_SetParamPtr aacEncoder_SetParam_VBT;

typedef AACENC_ERROR(*aacEncEncodePtr)(HANDLE_AACENCODER, const AACENC_BufDesc*, const AACENC_BufDesc*, const AACENC_InArgs*, AACENC_OutArgs*);
extern aacEncEncodePtr aacEncEncode_VBT;

typedef AACENC_ERROR(*aacEncInfoPtr)(HANDLE_AACENCODER, AACENC_InfoStruct*);
extern aacEncInfoPtr aacEncInfo_VBT;

typedef AACENC_ERROR(*aacEncClosePtr)(HANDLE_AACENCODER*);
extern aacEncClosePtr aacEncClose_VBT;

#endif //AAC_DLL_H
