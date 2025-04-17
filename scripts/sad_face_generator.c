#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_SPHERES 60000
#define FACE_RADIUS 1000
#define EYE_RADIUS 100
#define EYE_X_OFFSET 300
#define EYE_Y_OFFSET 300
#define MOUTH_RADIUS 400
#define MOUTH_Y_OFFSET -200
#define MIN_DIAM 2.0
#define MAX_DIAM 20.0
#define SPACING_FACTOR 1.5
#define MAX_ATTEMPTS 10000

typedef struct {
    double x, y, z;
    double diameter;
    int r, g, b;
    double reflection;
    double refraction_coef;
    double refraction_idx;
} Sphere;

// Function to check if a new sphere collides with existing ones
int is_valid_position(double x, double z, double radius, Sphere *spheres, int count)
{
    for (int i = 0; i < count; i++) {
        double dx = spheres[i].x - x;
        double dz = spheres[i].z - z;
        double distance = sqrt(dx * dx + dz * dz);
        if (distance < SPACING_FACTOR * (radius + (spheres[i].diameter / 2))) {
            return 0; // Collision detected
        }
    }
    return 1;
}

// Function to generate random double within a range
double random_double(double min, double max)
{
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Function to determine if a point is inside the face but outside eyes and mouth
int is_valid_smiley_position(double x, double z)
{
    double dist_from_center = sqrt(x * x + z * z);
    if (dist_from_center > FACE_RADIUS) return 0; // Outside the face disk

    // Eyes
    double left_eye_dist = sqrt(pow(x - EYE_X_OFFSET, 2) + pow(z - EYE_Y_OFFSET, 2));
    double right_eye_dist = sqrt(pow(x + EYE_X_OFFSET, 2) + pow(z - EYE_Y_OFFSET, 2));
    if (left_eye_dist < EYE_RADIUS || right_eye_dist < EYE_RADIUS) return 0; // Inside eyes

    // Mouth (only bottom half of a circle)
    double mouth_dist = sqrt(x * x + pow(z - MOUTH_Y_OFFSET, 2));
    if (mouth_dist < MOUTH_RADIUS && z < MOUTH_Y_OFFSET) return 0; // Inside mouth curve

    return 1;
}

// Function to generate spheres
void generate_spheres(Sphere *spheres)
{
    int count = 0;
    srand(time(NULL));

    while (count < NUM_SPHERES) {
        int attempts = 0;
        while (attempts < MAX_ATTEMPTS) {
            double x = random_double(-FACE_RADIUS, FACE_RADIUS);
            double z = random_double(-FACE_RADIUS, FACE_RADIUS);
            double diameter = random_double(MIN_DIAM, MAX_DIAM);
            double radius = diameter / 2;

            if (is_valid_smiley_position(x, z) && is_valid_position(x, z, radius, spheres, count)) {
                spheres[count].x = x;
                spheres[count].y = radius;  // Ensuring the sphere touches the plane at y=0
                spheres[count].z = z;
                spheres[count].diameter = diameter;
                spheres[count].r = rand() % 256;
                spheres[count].g = rand() % 256;
                spheres[count].b = rand() % 256;
                spheres[count].reflection = random_double(0, 1);
                spheres[count].refraction_coef = random_double(0, 1);
                spheres[count].refraction_idx = random_double(1.0, 2.5);
                count++;
                if (count % 1000 == 0) {
                    printf("Spheres generated: %d\n", count);
                }
                break;
            }
            attempts++;
        }
    }
    printf("Total spheres generated: %d\n", count);
}

// Function to save spheres to a file
void save_spheres_to_file(Sphere *spheres)
{
    FILE *file = fopen("spheres.txt", "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "+=======================================================================================================================================+\n");
    fprintf(file, "| SPHERE \n");
    fprintf(file, "+============+==============+=============+==============+=============+=============+=============================+\n");
    fprintf(file, "| ID         | coord        | diam        | RGB          | refl        | refr coef   | refr idx     |\n");
    fprintf(file, "+============+==============+=============+==============+=============+=============+=============================+\n");

    for (int i = 0; i < NUM_SPHERES; i++) {
        fprintf(file, " sp           %.2f,%.2f,%.2f    %.2f    %d,%d,%d    %.2f    %.2f    %.2f\n",
                spheres[i].x, spheres[i].y, spheres[i].z,
                spheres[i].diameter, spheres[i].r, spheres[i].g, spheres[i].b,
                spheres[i].reflection, spheres[i].refraction_coef, spheres[i].refraction_idx);
    }

    fclose(file);
    printf("Generated %d spheres and saved to file.\n", NUM_SPHERES);
}

int main()
{
    Sphere *spheres = malloc(NUM_SPHERES * sizeof(Sphere));
    if (!spheres) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    generate_spheres(spheres);
    save_spheres_to_file(spheres);

    free(spheres);
    return 0;
}
