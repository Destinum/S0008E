#include <iostream>
#include <fstream>
#include "PathTracerIncludes/sphere.h"
#include "PathTracerIncludes/hitable_list.h"
#include "PathTracerIncludes/camera.h"
#include "PathTracerIncludes/material.h"
#include "float.h"

vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;

    if (world->hit(r, 0.001, MAXFLOAT, rec))
    { 
        ray scattered;
        vec3 attenuation;

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * color(scattered, world, depth + 1);

        else
            return vec3(0, 0, 0);

        //vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        //return 0.5*color(ray(rec.p, target - rec.p), world);
    }

    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene(int ListLength) {

    int i = 0;
    hitable **list = new hitable*[ListLength + 1];

    list[i++] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    float Size = 0.2;
    int Boundry = sqrt(ListLength) / 2;
    int a = - Boundry;
    int b = a;

    while (i <= ListLength) {

        //float Size = 0.4 * drand48();
        float choose_mat = drand48();
        vec3 center(a + 0.9 * drand48(), Size, b + 0.9 * drand48()); 

        if (choose_mat < 0.7) {  // diffuse
            list[i++] = new sphere(center, Size, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
        }
        else if (choose_mat < 0.90) { // metal
            list[i++] = new sphere(center, Size, new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
        }
        else {  // glass
            list[i++] = new sphere(center, Size, new dielectric(1.5));
        }
        
        a++;
        if (a >= Boundry)
        {
            b++;
            a = - Boundry;
        }

    }

    return new hitable_list(list,i);
}


int main()
{
    //int Fisk = 5;
    //std::cout << "Fisk: " << Fisk << std::endl;
    //return Fisk;

    std::ofstream img ("picture.pgm");

    int nx = 100;       //400       //Image Width
    int ny = 50;       //200       //Image Height
    int ns = 10;       //200
    img << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *world = random_scene(500);

    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

    for (int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);

            for(int s = 0; s < ns; s++)
            {

                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);

                ray r = cam.get_ray(u, v);
                
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            img << ir << " " << ig << " " << ib << "\n";
        }
    }
}