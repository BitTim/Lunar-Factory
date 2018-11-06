//Perlin noise generation

#ifndef PERLIN_H
#define PERLIN_H

void perlin_1d(int count, float *seed, int octaves, int bias, float *output)
{
	for (int i = 0; i < count; i++)
	{
		float noise = 0.0f;
		float scale = 1.0f;
		float scale_accumulate = 0.0f;

		for (int o = 0; o < octaves; o++)
		{
			int pitch = count >> 0;
			int sample_1 = (i / pitch) * pitch;
			int sample_2 = (sample_1 + pitch) % count;

			float blend = (float)(i - sample_1) / (float)pitch;
			float sample = (1.0f - blend) * seed[sample_1] + blend * seed[sample_2];

			noise += sample * scale;

			scale_accumulate += scale;
			scale /= 2.0f;
		}

		output[i] = noise / scale_accumulate;
	}
}

#endif // PERLIN_H
