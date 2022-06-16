typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

inline int
clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

struct RGB {
	s16 r;
	s16 g;
	s16 b;
};

struct RGB color_converter(u32 color_in_hex)
{
	struct RGB rgbColor;
	rgbColor.r = ((color_in_hex >> 16) & 0xFF);  // Extract the RR byte
	rgbColor.g = ((color_in_hex >> 8) & 0xFF);   // Extract the GG byte
	rgbColor.b = ((color_in_hex) & 0xFF);        // Extract the BB byte

	return rgbColor;
}

u32 //color to be changed, color base
color_transformer(u32 color1, u32 color2)
{
	RGB color1_in_rgb = color_converter(color1);
	RGB color2_in_rgb = color_converter(color2);
	
	if (color1_in_rgb.r > color2_in_rgb.r) color1_in_rgb.r--;
	else color1_in_rgb.r++;
	if (color1_in_rgb.g > color2_in_rgb.g) color1_in_rgb.g--;
	else color1_in_rgb.g++;
	if (color1_in_rgb.b > color2_in_rgb.b) color1_in_rgb.b--;
	else color1_in_rgb.b++;

	u32 result = (color1_in_rgb.r << 16) | (color1_in_rgb.g << 8) | color1_in_rgb.b;
	return result;
}

/*
u32 percentage_colors_between(RGB color1, RGB color2, float percentage)
{
	RGB result;
	float r = (color2.r - color1.r) / 100,
		  g = (color2.g - color1.g) / 100,
		  b = (color2.b - color1.b) / 100;

	result.r = color1.r + (r * percentage);
	result.g = color1.g + (g * percentage);
	result.b = color1.b + (b * percentage);

	return result;
}*/
