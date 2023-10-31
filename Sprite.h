#pragma once
#include "TextureManeger.h"
#include "Vector4.h"
#include "Vector2.h"

enum SpriteMode
{
	Triangle,
	Box
};

struct BoxPosition
{
	Vector4 LeftTop;
	Vector4 RightTop;
	Vector4 LeftBottom;
	Vector4 RightBottom;;

};



class Sprite
{
	Sprite();
	~Sprite();

	void Initilize();

	void TransforMatrix();


	void Updata();

	void Draw();


};

