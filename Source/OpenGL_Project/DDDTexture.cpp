#include "DDDTexture.h"

DDDTexture::DDDTexture()
{
	data3D = new GLubyte[textureWidth * textureHeight * textureDepth * 4];


	GeneratePattern3D();
	GenerateData3D();
	LoadTexture3D();

}

DDDTexture::~DDDTexture()
{
}

void DDDTexture::GeneratePattern3D()
{

	for (int x = 0; x < textureWidth; x++)
	{
		for (int y = 0; y < textureHeight; y++)
		{
			for (int z = 0; z < textureDepth; z++)
			{

				texturePattern3D[x][y][z] = (y / 20) % 2 ? 0 : 1;

			}

		}
	}


}

void DDDTexture::GenerateData3D()
{

	for (int x = 0; x < textureWidth; x++)
	{
		for (int y = 0; y < textureHeight; y++)
		{
			for (int z = 0; z < textureDepth; z++)
			{

				int index = x * (textureWidth * textureHeight * 4) + y * (textureHeight * 4) + z * 4;

				if (texturePattern3D[x][y][z] == 1)
				{


					data3D[index + 0] = (GLubyte)255;
					data3D[index + 1] = (GLubyte)0;
					data3D[index + 2] = (GLubyte)0;
					data3D[index + 3] = (GLubyte)255;

				}
				else
				{

					data3D[index + 0] = (GLubyte)255;
					data3D[index + 1] = (GLubyte)255;
					data3D[index + 2] = (GLubyte)255;
					data3D[index + 3] = (GLubyte)255;



				}

			}

		}
	}

}

void DDDTexture::LoadTexture3D()
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, textureWidth, textureHeight, textureDepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, textureWidth, textureHeight, textureDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data3D);


}
