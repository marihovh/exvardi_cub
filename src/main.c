
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
	while (data->hit == 0)
	{
		if (data->side_dist_x < data->side_dist_y)
		{
			data->side_dist_x += data->delta_dist_x;
			data->map_x +=data->step_x;
			data->side = 0;
		}
		else
		{
			data->side_dist_y += data->delta_dist_y;
			data->map_y +=data->step_y;
			data->side = 0;
		}
		printf("fix anel x y-@ texerov poxel\n");
		if (data->map[data->map_x][data->map_y] > 0)
		{
			data->hit = 1;
		}

	}
} 

void	calculate_step_init_side_dist_before_dd(t_data *data)
{
		data->hit = 0;
		if (data->ray_dir_x < 0)
		{
			data->step_x = -1;
			data->side_dist_x = (data->pos_x - data->map_x) * data->delta_dist_x;
		}
		else
		{
			data->step_x = 1;
			data->side_dist_x = (data->map_x + 1.0 - data->pos_x) * data->delta_dist_x;
		}
		if (data->ray_dir_y < 0)
		{
			data->step_y = -1;
			data->side_dist_y = (data->pos_y - data->map_y) * data->delta_dist_y;
		}
		else
		{
			data->step_y = 1;
			data->side_dist_y = (data->map_y + 1.0 - data->pos_y) * data->delta_dist_y;
		}
} 

void	ft_texture_calculation(t_data	*data)
{
	double wall_x;

	wall_x = 0.0;
	if (data->side == 0)
		data->wall_x = data->pos_y + data->perp_wall_dist * data->ray_dir_y;
	else
		data->wall_x = data->pos_y + data->perp_wall_dist * data->ray_dir_x;
	wall_x -= floor(wall_x);
	data->tex_x = (int)(wall_x * (double)texWidth);
	if (data->side == 0 && data->ray_dir_x > 0)
		data->tex_x = texWidth - data->tex_x - 1;
	if (data->side == 0 && data->ray_dir_y < 0)
		data->tex_x = texWidth - data->tex_x - 1;	
} 


void	camera_frame_counting(t_data *data)
{
	
	if(data->side == 0)
		data->perp_wall_dist = (data->side_dist_x - data->delta_dist_x);
	else
		data->perp_wall_dist = (data->side_dist_y - data->delta_dist_y);
	data->line_height = (int)(screenHeight / data->perp_wall_dist);
	printf("\n----line heigth->         %i\n",data->line_height);
	data->draw_start = -data->line_height/2 + screenHeight + data->pitch;
	if (data->draw_start < 0)
		data->draw_start = 0;
	data->draw_end = screenHeight / 2 + data->line_height/2 + data->pitch;
	if (data->draw_end >= screenHeight)
		data->draw_end = screenHeight - 1;
}

int choose_rigth_texture(t_data *data)
{
	if (data->side && data->ray_dir_y > 0)
		return(0);
	else if (data->side && data->ray_dir_y < 0)
		return (1);
	else if (!data->side && data->ray_dir_x > 0)
			return(2);
	else if (!data->side && data->ray_dir_x < 0)
		return (3);
	return (-1);
}


void	textures_drawing(t_data *data, int i)
{
	int x;
	char *dst_tex;

	x = choose_rigth_texture(data);
	data->tex_pos = data->draw_start - screenHeight / 2 + data->line_height / 2 * data->step;
	while (data->draw_start < data->draw_end)
	{
		data->tex_y = (int)data->tex_pos & (texWidth - 1);
		data->tex_pos += data->step;
		dst_tex = data->texturess[x].addr + (data->tex_y) * \
				data->texturess[x].line_length + \
				data->tex_x * (data->texturess[x].bits_per_pixel / 8);
		my_mlx_pixel_put(data, i, data->draw_start ,*(unsigned int *)dst_tex);
		data->draw_start++;
	}
}

void	counting_delta_dist(t_data *data, int i)
{
	data->camera_x = 2 * i / (double)data->widht -  1;
	data->ray_dir_x = data->dir_x + data->cam_x * data->camera_x;
	data->ray_dir_y = data->dir_y + data->cam_y * data->camera_x;
	data->map_x = (int)data->pos_x;
	data->map_y = (int)data->pos_y;
	if ((data->ray_dir_x) == 0)
		data->delta_dist_x = 1e30;
	else
		data->delta_dist_x = fabs(1 / data->ray_dir_x);
	if ((data->ray_dir_y) == 0)
		data->delta_dist_y = 1e30;
	else
		data->delta_dist_y = fabs(1 / data->ray_dir_y);

} // done
void	texture_data_filling(t_data *data)
{
	int width;
	int heigth;
	data->texturess[0].img = mlx_xpm_file_to_image(data->mlx->ptr, "/Users/marihovh/Desktop/cub_new/textures/mossy.xpm", &width, &heigth);
	data->texturess[1].img = mlx_xpm_file_to_image(data->mlx->ptr, "/Users/marihovh/Desktop/cub_new/textures/redbrick.xpm", &width, &heigth);
	data->texturess[2].img = mlx_xpm_file_to_image(data->mlx->ptr, "/Users/marihovh/Desktop/cub_new/textures/wood.xpm", &width, &heigth);
	data->texturess[3].img = mlx_xpm_file_to_image(data->mlx->ptr, "/Users/marihovh/Desktop/cub_new/textures/path_to_the_north_texture.xpm", &width, &heigth);
	int i;

	i = 0;
	while (i < 4)
	{
		data->texturess[0].addr = mlx_get_data_addr(data->texturess[i].img,\
		&data->texturess[i].bits_per_pixel, &data->texturess[i].line_length,\
		&data->texturess[i].endian);
		i++;
	}
}
void	chose_gamer_path(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->size)
	{
		j = 0;
		//petqa loop-i mej data->size i poxaren amenaerkar toxy-i leny yntrenq
		while (j < data->size)
		{
			if (ft_strrchr(("NWSE"),data->map[i][j]))
				{
					data->hero->x = i;
					data->pos_x = i; // erevi sa petq chi ga
					data->pos_y = j; // erevi sa petq chi ga
					data->hero->y = j;
					data->hero->view = data->map[i][j];
				}
			j++;
		}
		i++;
	}
	// vortex a nayum
	// yst dra voorshenq
}
void	chose_gamer_path2(t_data *data)
{
	printf("\nhero's path -> %i\n",data->hero->view);
}

void	hero_data_filling(t_data *data)
{
	data->hero = malloc(sizeof(t_hero));
	//CHECK HOW THE PROGRAM STARTS -> N,W,E,S
	chose_gamer_path(data);
	chose_gamer_path2(data);

	// data->pos_x = 22.0;
	// data->pos_y = 11.5;
	data->dir_x = -1.0;
	data->dir_y = 0.0;
	data->plane_x = 0.0;
	data->plane_y = 0.66;
	data->move_speed = 0.1;
	data->rot_speed = 0.3;
}

void	ft_filling(t_data *data)
{
	data->count = 0;
	data->nb = 0;
	data->to_map = 0;
	data->hit = 0;
	data->pitch = 100;
	data->draw_end = 0;
	data->draw_start = 0;
	data->step = 1.0 * 64 / data->line_height;
	hero_data_filling(data);
	texture_data_filling(data);
}
int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->textures = malloc(sizeof(char *) * 4);
	data->mlx = malloc(sizeof(t_mlx));
	data->textures[0] = NULL;
	data->textures[1] = NULL;
	data->textures[2] = NULL;
	data->textures[3] = NULL;
	data->to_map = 0;
	data->map_h = 0;
	if (argc == 2)
	{
		if (parcing(data, argv))
			return (1);
		if (init_win(data))
			return (1);
		ft_filling(data);
		mlx_loop_hook(data->mlx->ptr, &just_do_it, data);
		mlx_hook(data->mlx->win, 2, 1L << 3 , key_hook, data);
		mlx_hook(data->mlx->win, 17, (1L << 17), key_cross, NULL);
		mlx_loop(data->mlx->ptr);
	}
	else
		return (errorik(42));
	return (0);
}