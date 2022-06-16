#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

//colors
u32 internal
white_backup = 0xB3B6B7,
black_backup = 0x17202A;

u32
white = white_backup,
black = black_backup;

static int paddlet_lenght = 2, paddlet_height = 12;
float paddlet_left_pos_y = 0.f, paddlet_right_pos_y = 0.f;
float paddlet_pos_x = 80.f;
float paddlet_left_d, paddlet_right_d;

static float ball_size = 1;
float ball_pos_x = 0.f, ball_pos_y = 0.f;
float ball_dp_x, ball_dp_y = 0.f;

bool pause = false, add_point = false, first_movement = true;
int score = 0, max_score = 3;
float screen_percentage = 50.f, actual_percentage;

internal bool
is_able_to_go_up(float pos_y, int half_height)
{
	if ((pos_y + half_height) >= 50.f) return false;
	else return true;
}

internal bool
is_able_to_go_down(float pos_y, int half_height)
{

	if ((pos_y - half_height) <= -50.f) return false;
	else return true;
}

internal void
reset()
{
	ball_pos_x = 0;
	ball_pos_y = 0;
	ball_dp_x *= -1;
	ball_dp_y = 0;
	pause = false;
}

internal void
restart()
{
	white = white_backup;
	black = black_backup;

	paddlet_left_pos_y = 0;
	paddlet_right_pos_y = 0;

	screen_percentage = 50.f;
	score = 0;

	ball_dp_x = 0;
	first_movement = true;

	reset();
}

internal void
left_win()
{
	if (ball_pos_x > 500) restart();

	if (black != white)
	{
		black = color_transformer(black, white);
		return;
	}
}

internal void
right_win()
{
	if (ball_pos_x < -500) restart();

	if (black != white)
	{
		white = color_transformer(white, black);
		return;
	}
}

internal void
update(s8 side, float dt)
{
	if (side == 'l' && ball_pos_x < 150 ||
		side == 'r' && ball_pos_x > -150)
		return;

	pause = true;

	if (add_point)
	{
		switch (side)
		{
		case 'r':
			score--;
			add_point = false;
			break;
		case 'l':
			score++;
			add_point = false;
			break;
		default:
			add_point = false;
			break;
		}
	}

	actual_percentage = score + max_score;

	if (side == 'r' && screen_percentage <= ((100.f / ((float)max_score * 2.f)) * actual_percentage) ||
		side == 'l' && screen_percentage >= ((100.f / ((float)max_score * 2.f)) * actual_percentage))
	{
		if (side == 'l' && ball_pos_x < 300 ||
			side == 'r' && ball_pos_x > -300)
			return;

		if (score == max_score) left_win();
		else if (score == -max_score) right_win();
		else reset();
		return;
	}

	if (side == 'l')
	{
		screen_percentage += 5.f * dt;
		return;
	}else if (side == 'r')
	{
		screen_percentage -= 5.f * dt;
		return;
	}
	
	return;
}

internal void
simulate_game(Input* input, float dt)
{
	set_half_bg_color(black, white, screen_percentage);
	float speed = 150.f,
		  paddlet_left_speed = 0.f,
		  paddlet_right_speed = 0.f;

	//inputs 1.1
	if (is_down(BUTTON_UP) && is_able_to_go_up(paddlet_right_pos_y, paddlet_height))
		paddlet_right_speed += speed;
	if (is_down(BUTTON_DOWN) && is_able_to_go_down(paddlet_right_pos_y, paddlet_height))
		paddlet_right_speed -= speed;
	paddlet_right_pos_y += paddlet_right_speed * dt;

	if (is_down(BUTTON_W) && is_able_to_go_up(paddlet_left_pos_y, paddlet_height))
		paddlet_left_speed += speed;
	if (is_down(BUTTON_S) && is_able_to_go_down(paddlet_left_pos_y, paddlet_height))
		paddlet_left_speed -= speed;
	paddlet_left_pos_y += paddlet_left_speed * dt;

	if (is_down(BUTTON_ESC)) running = false;
	if (is_down(BUTTON_R)) restart();

	//who will start with the ball
	if (first_movement)
	{
		if (is_down(BUTTON_UP) || (is_down(BUTTON_DOWN)))
		{
			ball_dp_x = -100.f;
			first_movement = false;
		}
		if (is_down(BUTTON_W) || (is_down(BUTTON_S)))
		{
			ball_dp_x = 100.f;
			first_movement = false;
		}
	}
	
	//the visual elements
	{
		draw_rect_if_bg_is(ball_pos_x, ball_pos_y, ball_size, ball_size, black, white, white, black); //ball
		draw_rect_if_bg_is(-paddlet_pos_x, paddlet_left_pos_y, paddlet_lenght, paddlet_height, white, white, white, black); //left paddlet
		draw_rect_if_bg_is(paddlet_pos_x, paddlet_right_pos_y, paddlet_lenght, paddlet_height, black, white, black, black); //right paddlet
	}

	//physics
	{
		ball_pos_x += ball_dp_x * dt;
		ball_pos_y += ball_dp_y * dt;
	}

	///ball detections
	{
		if (ball_pos_x > 0) //right paddlet collision
		{
			//local variables
			float
				b_right_side = ball_pos_x + ball_size,
				b_left_side = ball_pos_x - ball_size,
				b_top_side = ball_pos_y + ball_size,
				b_bottom_side = ball_pos_y - ball_size,

				p_r_right_side = paddlet_pos_x + paddlet_lenght,
				p_r_left_side = paddlet_pos_x - paddlet_lenght,
				p_r_top_side = paddlet_right_pos_y + paddlet_height,
				p_r_bottom_side = paddlet_right_pos_y - paddlet_height;

			if (b_right_side > p_r_left_side &&
				b_left_side < p_r_right_side &&
				b_top_side > p_r_bottom_side &&
				b_bottom_side < p_r_top_side)
			{
				ball_pos_x = p_r_left_side - ball_size;
				ball_dp_x *= -1;
				if (paddlet_right_speed != 0) ball_dp_y = (ball_pos_y - paddlet_left_pos_y) + paddlet_right_speed;
				ball_dp_y += ball_pos_y - paddlet_right_pos_y;
			}
		}
		else //left paddlet collision
		{
			//local variables
			float
				b_right_side = ball_pos_x + ball_size,
				b_left_side = ball_pos_x - ball_size,
				b_top_side = ball_pos_y + ball_size,
				b_bottom_side = ball_pos_y - ball_size,

				p_l_right_side = -paddlet_pos_x + paddlet_lenght,
				p_l_left_side = -paddlet_pos_x - paddlet_lenght,
				p_l_top_side = paddlet_left_pos_y + paddlet_height,
				p_l_bottom_side = paddlet_left_pos_y - paddlet_height;

			if (b_left_side < p_l_right_side &&
				b_right_side > p_l_left_side &&
				b_top_side > p_l_bottom_side &&
				b_bottom_side < p_l_top_side)
			{
				ball_pos_x = p_l_right_side + ball_size;
				ball_dp_x *= -1;
				if(paddlet_left_speed != 0) ball_dp_y = (ball_pos_y - paddlet_left_pos_y) + paddlet_left_speed;
				ball_dp_y += ball_pos_y - paddlet_left_pos_y;
			}
		}

		//screen top and down collision
		if (!is_able_to_go_up(ball_pos_y, ball_size))
		{
			ball_pos_y = 50.f - ball_size;
			ball_dp_y *= -1;
		} else if (!is_able_to_go_down(ball_pos_y, ball_size))
		{
			ball_pos_y = -50.f + ball_size;
			ball_dp_y *= -1;
		}

		//screen sides collision
		if (ball_pos_x + ball_size > 100)
		{
			if (!pause) add_point = true;
			update('l', dt);
		} else if (ball_pos_x - ball_size < -100)
		{
			if (!pause) add_point = true;
			update('r', dt);
		}
	}
}