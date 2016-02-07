#ifndef NAI_IMG_STRUCT
#define NAI_IMG_STRUCT

//TODO overload bracket operators and make all data protected
struct nimg {
	nimg() {

	}
	nimg(unsigned int swidth, unsigned int sheight, unsigned int sdepth) { 
		width = swidth;
		height = sheight;
		depth = sdepth;
		resolution = height*width*depth;
		data = new unsigned char[resolution];
		}
	~nimg() { delete[] data; }

	unsigned char *data;
	unsigned int height, width, depth, resolution;

private:



};
#endif
