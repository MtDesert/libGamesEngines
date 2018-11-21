#ifndef BITMAP_H
#define BITMAP_H

#include"DataBlock.h"

/**
 * @brief The Bitmap_32bit class is to store image data,each pixel is 32bit(unsigned long)
 *
 * About Color:
 * Bitmap_32bit can store color format RGBA or ARGB or BGRA and so on
 * It always 32bit, no matter what format it is
 * It determined by user, Bitmap_32bit do not care about it
 * That means, you can dynamic change the color format as you like
 *
 * About Coordinate:
 * Bitmap_32bit can use Math Coordinate(Up is Y+) or Screen Cordiante(Down is Y+)
 * You can change the value of member variable coordinateType,it only effect getColor() and setColor()
 */
class Bitmap_32bit:public DataBlock{
public:
	Bitmap_32bit();
	~Bitmap_32bit();
	//new & delete
	bool newBitmap(uint width,uint height);
	bool deleteBitmap();
	//size
	uint getWidth()const;
	uint getHeight()const;

	//coordiante type
	enum CoordinateType{
		CoordinateType_Math,
		CoordinateType_Screen
	};
	CoordinateType coordinateType;

	bool getColor(uint x,uint y,uint32 &color)const;
	bool setColor(uint x,uint y,uint32 &color);
private:
	uint width;
	uint height;
};
#endif // BITMAP_H