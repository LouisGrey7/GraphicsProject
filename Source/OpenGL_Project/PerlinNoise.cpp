#include "PerlinNoise.h"
#include "stb_image_write.h"

double PerlinNoise::RandomValue(int x, int y)
{
	
	int seed = x + y * 31;
	srand(seed);

	double RandomValue;
	int Noise = x + y * seed;
	Noise = (Noise << 13) ^ Noise;

	int T = (Noise * (Noise * Noise * 15731 + 789221) + 1376312589) & 0x7fffffff;

	RandomValue = 1.0f - (double)T * 0.93132257461548515625e-9;

	return RandomValue;
}

double PerlinNoise::Smooth(int x, int y)
{

	double Corners = (RandomValue(x - 1, y - 1) + 
					RandomValue(x + 1, y - 1) + 
					RandomValue(x - 1, y + 1) + 
					RandomValue(x + 1, y + 1)) / 16.0f;

	double Sides = (RandomValue(x - 1, y) + 
					RandomValue(x + 1, y) + 
					RandomValue(x, y - 1) + 
					RandomValue(x, y + 1)) / 8.0f;

	double Center = (RandomValue(x, y)) / 4.0f;



	return Corners + Sides + Center;
}

double PerlinNoise::LinearInterpolate(double point1, double point2, double Fract)
{
	return	(point1 * (1 - Fract) + point2 * Fract);
}

double PerlinNoise::CosineInterpolate(double point1, double point2, double Fract)
{

	double Fract2 = (1 - cos(Fract * 3.14159f)) / 2;

	return (point1 * (1 - Fract2) + point2 * Fract2);
}

double PerlinNoise::SmoothInterpolate(double x, double y)
{
	int truncatedX = (int)x;
	int truncatedY = (int)y;



	double fractX = x - (double)truncatedX;
	double fractY = y - (double)truncatedY;

	double V1 = Smooth(truncatedX, truncatedY);
	double V2 = Smooth(truncatedX + 1, truncatedY);
	double V3 = Smooth(truncatedX, truncatedY + 1);
	double V4 = Smooth(truncatedX + 1, truncatedY + 1);


	double Interpolate_1 = CosineInterpolate(V1, V2, (double)fractX);
	double Interpolate_2 = CosineInterpolate(V3, V4, (double)fractX);


	double Final = CosineInterpolate(Interpolate_1, Interpolate_2, fractY);

	
	return Final;

}

double PerlinNoise::TotalNoisePerPoint(int x, int y)
{

	int octaves = 6;
	float waveLength = 128.0f;
	float gain = 0.5f;
	float lacunarity = 2.0f;

	float maxValue = 0.0f;
	double total = 0.0f;


	for (int i = 0; i < octaves; i++)
	{
		float frequency = (float)pow(lacunarity, i) / waveLength;
		float amplitude = (float)pow(gain, i);
		maxValue += amplitude;

		total += SmoothInterpolate(x * frequency, y * frequency) * amplitude;
	}


	return (total / maxValue);
}

void PerlinNoise::GenerateNoise()
{
	const int width = 512;
	const int height = 512;

	double** noise = new double* [height];

	for (int i = 0; i < height; i++) 
	{
		noise[i] = new double[width];
	}

	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			double value = TotalNoisePerPoint(x, y);
			noise[y][x] = (value + 1.0) * 127.5;
		}
	}

	unsigned char* pixels = new unsigned char[width * height];

	for (int i = 0; i < width * height; i++) 
	{
		pixels[i] = static_cast<unsigned char>(noise[i / width][i % width]);
	}

    // Save pixels to a raw file
	std::ofstream RawFile("Resources/HeightMaps/pnMap.raw", std::ios_base::binary);

	if (RawFile)
	{

		char* c = (char*)&pixels[0];
		RawFile.write((char*)&pixels[0], (std::streamsize)((int)width * (int)height));
		RawFile.close();

	}

    // Save pixels to a jpg
    stbi_write_jpg("Resources/Textures/pnMap.jpg", width, height, 1, pixels, width * sizeof(unsigned char));

	std::cout << "Noise Generated" << std::endl;

    // Free Memory
    for (int i = 0; i < height; i++) 
	{
        delete[] noise[i];
    }

    delete[] noise;
    delete[] pixels;
}

