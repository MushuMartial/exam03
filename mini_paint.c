#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

int width;
int height;
char background;
char **tab;

char type;
float x;
float y;
float radius;
char color;

int res;
FILE *fd;
float sqr;

int msg_error(FILE *fd, int error)
{
	if (fd)
		fclose(fd);
	if (error == 1)
		write(1, "Error: argument\n", 16);
	else if (error == 2)
	{
		write (1, "Error: Operation file corrupted\n" ,32);
		return(1);
	}
	else
	{
		for(int i = 0; i < height ; i++)
		{
			write(1, tab[i], width);
			write(1, "\n", 1);
		}
	}
	return (error);
}

int main(int argc, char **argv)
{
	fd = NULL;

	if (argc != 2)
		return (msg_error(fd, 1));
	
	if ((fd = fopen(argv[1], "r")) == NULL)
		return(msg_error(fd, 2));
	
	if ((res = fscanf(fd, "%d %d %c", &width, &height, &background)) != 3)
		return(msg_error(fd, 2));
	
	if (width <= 300 && width > 0 && height <= 300 && height > 0)
	{
		tab = malloc(height * sizeof(char *)); 
		for (int i = 0; i < height ; i++)
		{
			tab[i] = malloc(width * sizeof(char));
			memset(tab[i], background, width);
		}
		while (1)
		{
			res = fscanf(fd, " %c %f %f %f %c", &type, &x ,&y ,&radius ,&color);
			if(res == -1)
				return(msg_error(fd, 0));
			else if (res != 5 || radius <= 0 || (type != 'c' && type != 'C'))
				break;
			for (int line = 0; line < height; line++)
			{ 
				for (int col = 0; col< width; col++)
				{
					sqr = sqrtf((col - x )*(col - x) + (line - y)*(line - y));
					if(sqr <= radius)
					{
						if (type == 'c' && sqr+1 > radius)
							tab[line][col] = color;
						else if (type == 'C')
							tab[line][col] = color;
					}
				}
			}
		}
	}
	return(msg_error(fd, 2));
}
