#ifndef NAI_IMG_STRUCT
#define NAI_IMG_STRUCT
struct nimg {unsigned int height, width, depth; unsigned char *data;
nimg() { data = 0; }};
#endif
