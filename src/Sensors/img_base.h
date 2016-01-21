#ifndef _NAIBRAIN_EYE_IMG_DEPTH
#define _NAIBRAIN_EYE_IMG_DETPH

#pragma once

/****************************************************************
   AUTHOR:		Steven Radomski

   FUNCTION:	Base class for sensor image processing

   NOTES:		N/A

****************************************************************/

class imgb {
	public:
		imgb();
		~imgb();

		virtual void ProcessImg(unsigned char *) = 0;

	private:


};

#endif
