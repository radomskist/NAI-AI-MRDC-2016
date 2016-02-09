#ifndef NAI_IMG_STRUCT
#define NAI_IMG_STRUCT

//TODO overload bracket operators and make all data protected
struct nimg {
	nimg() {
		data = 0;
	}
	nimg(unsigned int swidth, unsigned int sheight, unsigned int sdepth) { 
		width = swidth;
		height = sheight;
		depth = sdepth;
		resolution = height*width*depth;
		data = new unsigned char[resolution];
		}
	~nimg() { if(data) delete[] data; }

	unsigned char *data;
	unsigned int height, width, depth, resolution;
	unsigned int flags;

};
#endif
