
#include "../cub3d.h"

int	errorik(int flag)
{
	if (flag == M_ERROR)
		ft_putstr_fd("Error: map\n", 2);
	else if (flag == F_ERROR)
		ft_putstr_fd("Error: wrong format of map\n", 2);
	else if (flag == T_ERROR)
		ft_putstr_fd("Error: some trouble with textures\n", 2);
	else if (flag == C_ERROR)
		ft_putstr_fd("Error: some trouble with colors\n", 2);
	else if (flag == E_ERROR)
		ft_putstr_fd("Error: something is missing\n", 2);
	else
		ft_putstr_fd("Error\n", 2);
	return (1);
}



void	ft_dda_algorithm(t_data *data)
{
	while (data->ray->hit == 0)
	{
		if (data->ray->side_dist_x < data->ray->side_dist_y)
		{
			data->ray->side_dist_x += data->ray->delta_dist_x;
			data->ray->map_x +=data->ray->step_x;
			data->ray->side = 0;
		}
		else
		{
			data->ray->side_dist_y += data->ray->delta_dist_y;
			data->ray->map_y +=data->ray->step_y;
			data->ray->side = 0;
		}
		if (data->map[data->ray->map_x][data->ray->map_y] > 0)
		{
			data->ray->hit = 1;
		}
	}
} 

void	calculate_step_init_side_dist_before_dd(t_data *data)
{
		//for x cordinate
		if (data->ray->ray_dir_x < 0)
		{
			data->ray->step_x = -1;
			data->ray->side_dist_x = (data->pos_x - data->ray->map_x) * data->ray->delta_dist_x;
		}
		else
		{
			data->ray->step_x = 1;
			data->ray->side_dist_x = (data->ray->map_x + 1.0 - data->pos_x) * data->ray->delta_dist_x;
		}
		//for y cordinate
		if (data->ray->ray_dir_y< 0)
		{
			data->ray->step_y = -1;
			data->ray->side_dist_y = (data->pos_y - data->ray->map_y) * data->ray->delta_dist_y;
		}
		else
		{
			data->ray->step_y = 1;
			data->ray->side_dist_y = (data->ray->map_y + 1.0 - data->pos_y) * data->ray->delta_dist_y;
		}
} 

void	ft_texture_calculation(t_data	*data)
{
	if (data->ray->side == 0)
		data->ray->wall_x = data->hero->pos_y + data->ray->perp_wall_dist * data->ray->ray_dir_y;
	else
		data->ray->wall_x = data->hero->pos_y + data->ray->perp_wall_dist * data->ray->ray_dir_x;
	data->ray->wall_x -= floor(data->ray->wall_x);
	data->ray->tex_x = (int)(data->ray->wall_x * (double)texWidth);
	if (data->ray->side == 0 && data->ray->ray_dir_x > 0)
		data->ray->tex_x = texWidth - data->ray->tex_x - 1;
	if (data->ray->side == 0 && data->ray->ray_dir_y < 0)
		data->ray->tex_x = texWidth - data->ray->tex_x - 1;	
} 


void	camera_frame_counting(t_data *data)
{
	
	if(data->ray->side == 0)
	{
		data->ray->perp_wall_dist = (data->ray->side_dist_x - data->ray->delta_dist_x);
	}
	else
	{
		data->ray->perp_wall_dist = (data->ray->side_dist_y - data->ray->delta_dist_y);
	}

	// int lineHeight = (int)(h / perpWallDist);
	//       int drawStart = -lineHeight / 2 + h / 2 + pitch;
    //   if(drawStart < 0) drawStart = 0;
    //   int drawEnd = lineHeight / 2 + h / 2 + pitch;
    //   if(drawEnd >= h) drawEnd = h - 1;


	data->game->line_height = (int)(screenHeight / data->ray->perp_wall_dist);
	data->game->draw_start = ((-1 * data->game->line_height) / 2) + screenHeight / 2 + data->ray->pitch;
	if (data->game->draw_start < 0)
		data->game->draw_start = 0;
	data->game->draw_end = data->game->line_height / 2 + screenHeight / 2 + data->ray->pitch;
	if (data->game->draw_end >= screenHeight)
		data->game->draw_end = screenHeight - 1;
}

int choose_rigth_texture(t_data *data)
{
	if (data->ray->side && data->ray->ray_dir_y > 0)
		return(0);
	else if (data->ray->side && data->ray->ray_dir_y < 0)
		return (1);
	else if (!data->ray->side && data->ray->ray_dir_x > 0)
			return(2);
	else if (!data->ray->side && data->ray->ray_dir_x < 0)
		return (3);
	return (-1);
}


void	textures_drawing(t_data *data, int i)
{
	int x;
	char *dst_tex;

	x = choose_rigth_texture(data);
	data->ray->tex_pos = data->game->draw_start - screenHeight / 2 + data->game->line_height / 2 * data->game->step;
	printf("\n\n\n");
	printf("%d\n",data->game->draw_start);
	printf("%d\n",data->game->draw_end);
	while (data->game->draw_start < data->game->draw_end)
	{
		data->ray->tex_y = (int)data->ray->tex_pos & (texWidth - 1);
		data->ray->tex_pos += data->game->step;
		dst_tex = data->game->textures[x].addr + (data->ray->tex_y) * \
				data->game->textures[x].line_length + \
				data->game->tex_x * (data->game->textures[x].bits_per_pixel / 8);
		my_mlx_pixel_put(data, i, data->game->draw_start ,*(unsigned int *)dst_tex);
		data->game->draw_start++;
		printf("%d\n",data->game->draw_start);
	}
}


void	counting_delta_dist(t_data *data, int i)
{
	data->ray->camera_x = 2 * i / (data->widht - 1);
	data->ray->ray_dir_x = data->dir_x + data->cam_x * data->ray->camera_x;
	data->ray->ray_dir_y = data->dir_y + data->cam_y * data->ray->camera_x;
	data->ray->map_x = (int)data->pos_x;
	data->ray->map_y = (int)data->pos_y;
	if (!(data->ray->ray_dir_x))
		data->ray->delta_dist_x = 1e30;
	else
		data->ray->delta_dist_x = fabs(1 / data->ray->ray_dir_x);
	if (!(data->ray->ray_dir_y))
		data->ray->delta_dist_y = 1e30;
	else
		data->ray->delta_dist_y = fabs(1 / data->ray->ray_dir_y);

} // done
void	texture_data_filling(t_data *data)
{
	int width;
	int heigth;
	data->game->textures[0].img = mlx_xpm_file_to_image(data->mlx_ptr, "/Users/marihovh/Desktop/cub_new/textures/mossy.xpm", &width, &heigth);
	data->game->textures[1].img = mlx_xpm_file_to_image(data->mlx_ptr, "/Users/marihovh/Desktop/cub_new/textures/redbrick.xpm", &width, &heigth);
	data->game->textures[2].img = mlx_xpm_file_to_image(data->mlx_ptr, "/Users/marihovh/Desktop/cub_new/textures/wood.xpm", &width, &heigth);
	data->game->textures[3].img = mlx_xpm_file_to_image(data->mlx_ptr, "/Users/marihovh/Desktop/cub_new/textures/path_to_the_north_texture.xpm", &width, &heigth);

}
void	hero_data_filling(t_data *data)
{
	//CHECK HOW THE PROGRAM STARTS -> N,W,E,S
	data->hero->pos_x = 22.0;
	data->hero->pos_y = 11.5;
	data->hero->dir_x = -1.0;
	data->hero->dir_y = 0.0;
	data->hero->plane_x = 0.0;
	data->hero->plane_y = 0.66;
	data->hero->move_speed = 0.1;
	data->hero->rot_speed = 0.3;
}

void	ft_filling(t_data *data)
{
	data->ray = malloc(sizeof(t_ray));
	data->game = malloc(sizeof(t_game));
	data->hero = malloc(sizeof(t_hero));
	data->count = 0;
	data->nb = 0;
	data->to_map = 0;
	data->ray->hit = 0;
	data->ray->pitch = 100;
	data->game->step = 1.0 * 64 / data->game->line_height;
	texture_data_filling(data);
	hero_data_filling(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->textures = malloc(sizeof(char *) * 4);
	data->textures[0] = NULL;
	data->textures[1] = NULL;
	data->textures[2] = NULL;
	data->textures[3] = NULL;
	if (argc == 2)
	{
		if (parcing(data, argv))
			return (1);
		if (init_win(data))
			return (1);
		ft_filling(data);
		if (just_do_it(data))
			return (1);
		mlx_hook(data->win_ptr, 2, 0, key_hook, data);
		mlx_hook(data->win_ptr, 17, (1L << 17), key_cross, data);
		mlx_loop(data->mlx_ptr);
	}
	else
		return (errorik(42));
	return (0);
}