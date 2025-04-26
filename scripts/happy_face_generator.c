/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   happy_face_generator.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:52:57 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 19:17:47 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Program generates a set of non-overlapping spheres positioned within a     */
/* predefined *happy* smiley face shape in the XZ plane. Each sphere is       */
/* assigned random visual and optical properties, such as color, reflection,  */
/* and refraction. The generated sphere data is saved to a formatted text     */
/* file.                                                                      */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_SPHERES             100000
#define FACE_RADIUS             1000
#define EYE_RADIUS              100
#define EYE_X_OFFSET            300
#define EYE_Y_OFFSET            -400
#define MOUTH_RADIUS            400
#define MOUTH_Y_OFFSET          300
#define MIN_DIAM                1.5
#define MAX_DIAM                15.0
#define SPACING_FACTOR          1.5
#define MAX_ATTEMPTS            10000

typedef struct s_sphere
{
    double      x;
    double      y;
    double      z;
    double      diameter;
    int         r;
    int         g;
    int         b;
    double      reflection;
    double      refraction_coef;
    double      refraction_idx;
}   t_sphere;

int is_colliding(double x, double z, double radius, t_sphere *spheres, int count)
{
    int     i;
    double  dx;
    double  dz;
    double  distance;
    
    i = -1;
    while (++i < count)
    {
        dx = spheres[i].x - x;
        dz = spheres[i].z - z;
        distance = sqrt(dx * dx + dz * dz);
        if (distance < SPACING_FACTOR * (radius + (spheres[i].diameter / 2)))
            return (1);
    }
    return (0);
}

double  get_random_double_within(double min, double max)
{
    return (min + ((double)rand() / RAND_MAX) * (max - min));
}

int is_inside_smiley_shape(double x, double z)
{
    double  dist_from_center;
    double  left_eye_dist;
    double  right_eye_dist;
    double  mouth_dist;

    dist_from_center = sqrt(x * x + z * z);
    if (dist_from_center > FACE_RADIUS)
        return (0);
    left_eye_dist = sqrt(pow(x - EYE_X_OFFSET, 2) + pow(z - EYE_Y_OFFSET, 2));
    right_eye_dist = sqrt(pow(x + EYE_X_OFFSET, 2) + pow(z - EYE_Y_OFFSET, 2));
    if (left_eye_dist < EYE_RADIUS || right_eye_dist < EYE_RADIUS)
        return (0);
    mouth_dist = sqrt(x * x + pow(z - MOUTH_Y_OFFSET, 2));
    if (mouth_dist < MOUTH_RADIUS && z > MOUTH_Y_OFFSET)
        return (0);
    return (1);
}

void    generate_spheres(t_sphere *spheres)
{
    int     count;
    int     attempts;
    double  x;
    double  y;
    double  z;
    double  diameter;
    double  radius;

    count = 0;
    srand(time(NULL));
    while (count < NUM_SPHERES)
    {
        attempts = 0;
        while (attempts < MAX_ATTEMPTS)
        {
            x = get_random_double_within(-FACE_RADIUS, FACE_RADIUS);
            z = get_random_double_within(-FACE_RADIUS, FACE_RADIUS);
            diameter = get_random_double_within(MIN_DIAM, MAX_DIAM);
            radius = diameter / 2;
            if (is_inside_smiley_shape(x, z) && !is_colliding(x, z, radius, spheres, count))
            {
                spheres[count].x = x;
                spheres[count].y = radius;
                spheres[count].z = z;
                spheres[count].diameter = diameter;
                spheres[count].r = 255;
                spheres[count].g = 255;
                spheres[count].b = 0;
                spheres[count].reflection = get_random_double_within(0, 1);
                spheres[count].refraction_coef = get_random_double_within(0, 1);
                spheres[count].refraction_idx = get_random_double_within(1.0, 2.5);
                count++;
                if (count % 1000 == 0)
                    printf("Spheres generated: %d\n", count);
                break ;
            }
            attempts++;
        }
    }
    printf("Total spheres generated: %d\n", count);
}

void    save_spheres_to_file(t_sphere *spheres)
{
    FILE    *file;
    int     i;

    file = fopen("spheres.txt", "w");
    if (!file)
    {
        printf("Error opening file!\n");
        return ;
    }
    fprintf(file, "+=======================================================================================================================================+\n");
    fprintf(file, "| SPHERE \n");
    fprintf(file, "+============+==============+=============+==============+=============+=============+=============================+\n");
    fprintf(file, "| ID         | coord        | diam        | RGB          | refl        | refr coef   | refr idx     |\n");
    fprintf(file, "+============+==============+=============+==============+=============+=============+=============================+\n");
    i = -1;
    while (++i < NUM_SPHERES)
    {
        fprintf(file, " sp           %.2f,%.2f,%.2f    %.2f    %d,%d,%d    %.2f    %.2f    %.2f\n",
                spheres[i].x, spheres[i].y, spheres[i].z,
                spheres[i].diameter, spheres[i].r, spheres[i].g, spheres[i].b,
                spheres[i].reflection, spheres[i].refraction_coef, spheres[i].refraction_idx);
    }
    fclose(file);
    printf("Generated %d spheres and saved to file.\n", NUM_SPHERES);
}

int main(void)
{
    t_sphere    *spheres;

    spheres = malloc(NUM_SPHERES * sizeof(t_sphere));
    if (!spheres)
    {
        printf("Memory allocation failed!\n");
        return (1);
    }
    generate_spheres(spheres);
    save_spheres_to_file(spheres);
    free(spheres);
    return (0);
}
