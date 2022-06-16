internal void
set_bg_color(u32 color)
{
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = color;
		}
	}
}

internal void
set_half_bg_color(u32 left_color, u32 right_color, float percentage)
{
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			if (x < (render_state.width / 100.f) * percentage) *pixel++ = left_color;
			else *pixel++ = right_color;
		}
	}
}

internal u32
horizontal_gradient(u32 left_color, u32 right_color, float pos_x, float limit)
{
	if (pos_x < (-limit / 2)) return left_color;
	if (pos_x > (limit / 2)) return right_color;

	u32 result_in_hex = 0;
	float percentage = 100.f * (pos_x + (limit * .5f)) / (limit * 2);
	float color_unit;

	RGB l_color = color_converter(left_color), r_color = color_converter(right_color), result_in_rgb;

	//red comparision
	if (l_color.r > r_color.r)
	{
		color_unit = (l_color.r - r_color.r) / 100;
		result_in_rgb.r = l_color.r - (color_unit * percentage);
		
	}
	else
	{
		color_unit = (l_color.r - r_color.r) / 100;
		result_in_rgb.r = l_color.r + (color_unit * percentage);
	}

	//green comparision
	if (l_color.g > r_color.g)
	{
		color_unit = (l_color.g - r_color.g) / 100;
		result_in_rgb.g = l_color.g - (color_unit * percentage);

	}
	else
	{
		color_unit = (l_color.g - r_color.g) / 100;
		result_in_rgb.g = l_color.g + (color_unit * percentage);
	}

	//blue comparision
	if (l_color.b > r_color.b)
	{
		color_unit = (l_color.b - r_color.b) / 100;
		result_in_rgb.b = l_color.b - (color_unit * percentage);

	}
	else
	{
		color_unit = (l_color.b - r_color.b) / 100;
		result_in_rgb.b = l_color.b + (color_unit * percentage);
	}

	//convertion
	result_in_hex = (result_in_rgb.r << 16) | (result_in_rgb.g << 8) | result_in_rgb.b;

	return result_in_hex;
}

internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void
draw_rect(float pos_x, float pos_y, float rect_lenght, float rect_height, u32 color)
{
	pos_x		*= render_state.height * render_scale;
	pos_y		*= render_state.height * render_scale;
	rect_lenght *= render_state.height * render_scale;
	rect_height *= render_state.height * render_scale;

	pos_x += render_state.width / 2.f;
	pos_y += render_state.height / 2.f;

	//Change to pixels
	int x0 = pos_x - rect_lenght;
	int x1 = pos_x + rect_lenght;
	int y0 = pos_y - rect_height;
	int y1 = pos_y + rect_height;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void //position (x and y), size (lenght and height), colors and condition to this colors appears
draw_rect_if_bg_is(float pos_x, float pos_y, float rect_lenght, float rect_height, u32 color1, u32 cond_color_1, u32 color2, u32 cond_color_2)
{
	pos_x		*= render_state.height * render_scale;
	pos_y		*= render_state.height * render_scale;
	rect_lenght *= render_state.height * render_scale;
	rect_height *= render_state.height * render_scale;

	pos_x += render_state.width / 2.f;
	pos_y += render_state.height / 2.f;

	//Change to pixels
	int x0 = pos_x - rect_lenght;
	int x1 = pos_x + rect_lenght;
	int y0 = pos_y - rect_height;
	int y1 = pos_y + rect_height;

	//DRAW_RECT_IN_PIXELS
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++)
		{
			if(*pixel == cond_color_1) *pixel++ = color1;
			else if (*pixel == cond_color_2) *pixel++ = color2;
		}
	}
}