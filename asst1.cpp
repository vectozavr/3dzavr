#include <iostream>
#include <cmath>

#include <Engine.h>
#include <io/Image.h>

void show_picture_example() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize the SDL2 library\n";
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          680, 480,
                                          0);

    if(!window) {
        std::cout << "Failed to create window\n";
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface) {
        std::cout << "Failed to get the surface from the window\n";
    }

    SDL_Surface *image = SDL_LoadBMP("lena.bmp");

    if(!image) {
        std::cout << "Failed to load image\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
    }

    bool keep_window_open = true;
    while(keep_window_open) {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0) {
            switch(e.type) {
                case SDL_EVENT_QUIT:
                    keep_window_open = false;
                    break;
            }

            SDL_BlitSurface(image, NULL, window_surface, NULL);
            SDL_UpdateWindowSurface(window);
        }
    }
}

void test() {
    uint16_t w = 1920;
    uint16_t h = 1080;

    Image img(w, h);

    for(int j = 0; j < h; j++) {
        for(int i = 0; i < w; i++) {

            double rmax = sqrt(w*w + h*h)/2.0;
            auto intensity = static_cast<uint16_t>(255.0*(1.0 - sqrt((w/2-i)*(w/2-i) + (h/2-j)*(h/2-j))/rmax));

            img.set_pixel(i, j, Color(intensity, intensity, intensity));
        }
    }

    img.save2png("test.png");
}

struct RayCastInfo {
    Vec3D point;
    Vec3D n;
    Color color;
    double distance;
};

enum RAY_CAST_FRAG {
    DEFAULT,
    SHADOW,
    REFLECTION
};

RayCastInfo rayCast(const Vec3D& e, const Vec3D& d, RAY_CAST_FRAG flag = DEFAULT) {

    double t = std::numeric_limits<double>::infinity();
    Vec3D norm;
    Color surfaceColor;

    // Sphere 1:
    Color sphere1Color = Consts::WHITE_COLORS[0];
    Vec3D c1(-1.5, -0.3, 4);
    double R1 = 0.7;

    Vec3D e_c1 = e-c1;
    double d_ec1 = d.dot(e_c1);
    double D1 = std::pow(d_ec1, 2) - d.sqrAbs()*(e_c1.sqrAbs()-R1*R1);
    if(D1 >= 0) {
        double root1_t = (-d_ec1 - std::sqrt(D1))/d.dot(d);
        if(root1_t > Consts::EPS && root1_t < t) {
            t = root1_t;
            norm = (e + d*t - c1)/R1;
            surfaceColor = sphere1Color;
        }
    }


    // Sphere 2:
    Color sphere2Color = Consts::WHITE_COLORS[2];
    Vec3D c2(0.5, 0, 3);
    double R2 = 1;

    Vec3D e_c2 = e-c2;
    double d_ec2 = d.dot(e_c2);
    double D2 = std::pow(d_ec2, 2) - d.sqrAbs()*(e_c2.sqrAbs()-R2*R2);
    if(D2 >= 0) {
        double root2_t = (-d_ec2 - std::sqrt(D2))/d.dot(d);
        if(root2_t > Consts::EPS && root2_t < t) {
            t = root2_t;
            norm = (e + d*t - c2)/R2;
            surfaceColor = sphere2Color;
        }
    }

    // Plane
    Vec3D planeNormal(0, 1, 0);
    Vec3D planePoint(0,-1,0);
    Color planeColor = Consts::WHITE*0.5;
    if(std::abs(d.dot(planeNormal)) > Consts::EPS) {
        double root_t = (planePoint.dot(planeNormal) - e.dot(planeNormal))/d.dot(planeNormal);
        if(root_t > Consts::EPS && root_t < t) {
            t = root_t;
            norm = planeNormal;
            surfaceColor = planeColor;
        }
    }

    // Other figures
    //...

    if(flag == SHADOW) {
        return RayCastInfo{d*t, norm, surfaceColor, d.abs()*t};
    }

    if(t == std::numeric_limits<double>::infinity()) {
        return RayCastInfo{Vec3D(), norm, surfaceColor, std::numeric_limits<double>::infinity()};
    }

    Vec3D v = -d;

    // Light sources

    // Constant minimal illumination from the environment
    Color env = surfaceColor*0.3;

    // Light source 1:
    Color light1Color = Consts::WHITE;
    Vec3D light1dir = Vec3D(1, 1, -1).normalized();

    // Lambertian Illumination 1
    Color lamb1 = light1Color*surfaceColor*std::max(0.0, light1dir.dot(norm))*0.7;

    // Blinn-Phong Illumination 1
    double p = 30;
    Vec3D h1 = (light1dir+v).normalized();
    Color spec1 = Consts::WHITE*light1Color*std::pow(std::max(0.0, h1.dot(norm)), p)*0.5;

    // Shadows 1
    RayCastInfo rc_shadow1 = rayCast(e + d*t, light1dir, SHADOW);
    if(rc_shadow1.distance < std::numeric_limits<double>::infinity()) {
        lamb1 = lamb1*0;
        spec1 = spec1*0;
    }

    // Light source 2:
    Color light2Color = Consts::WHITE*0.5;
    Vec3D light2dir = Vec3D(-3, 4, -1).normalized();

    // Lambertian Illumination 2
    Color lamb2 = light2Color*surfaceColor*std::max(0.0, light2dir.dot(norm))*0.7;

    // Blinn-Phong Illumination 2
    Vec3D h2 = (light2dir+v).normalized();
    Color spec2 = Consts::WHITE*light2Color*std::pow(std::max(0.0, h2.dot(norm)), p)*0.5;

    // Shadows 2
    RayCastInfo rc_shadow2 = rayCast(e + d*t, light2dir, SHADOW);
    if(rc_shadow2.distance < std::numeric_limits<double>::infinity()) {
        lamb2 = lamb2*0;
        spec2 = spec2*0;
    }


    if(t < std::numeric_limits<double>::infinity() && flag != REFLECTION) {
        // Reflections
        Vec3D r = norm*2*v.dot(norm) - v;
        RayCastInfo rc_reflection = rayCast(e + d*t, r, REFLECTION);

        env = env + rc_reflection.color*0.5/(1.0 + rc_reflection.distance/5.0);
    }


    return RayCastInfo{d*t, norm, (env + lamb1 + spec1 + lamb2 + spec2)/(1.0 + d.abs()*t/30.0), d.abs()*t};
}

void perspective() {
    uint16_t w = 1920;
    uint16_t h = 1080;
    double a = (double)w/h;
    Image img(w, h);

    Vec3D cam_pos(0, 0, -1);

    for(int j = 0; j < h; j++) {
        for(int i = 0; i < w; i++) {
            double x = a*(1.0/2.0 - (1.0*i + 0.5)/w);
            double y = 1*(1.0/2.0 - (1.0*j + 0.5)/h);
            Vec3D rd(x, y,0);

            auto rc = rayCast(cam_pos, rd-cam_pos);

            if(rc.distance < std::numeric_limits<double>::infinity()) {
                img.set_pixel(i, j, rc.color);
            } else {
                img.set_pixel(i, j, Color(0, 0, 0));
            }
        }
    }

    img.save2png("perspective.png");
}

void parallel() {
    uint16_t w = 1920;
    uint16_t h = 1080;

    double screenSize = 4;
    double a = (double)w/h;
    Image img(w, h);

    Vec3D cam_pos(0, 1.5, -10);
    Vec3D view_dir(0, -0.1, 1);

    for(int j = 0; j < h; j++) {
        for(int i = 0; i < w; i++) {
            double x = a*(1.0/2.0 - (1.0*i + 0.5)/w);
            double y = 1*(1.0/2.0 - (1.0*j + 0.5)/h);
            Vec3D rd = cam_pos + Vec3D(x, y, 0)*screenSize;

            auto rc = rayCast(rd,view_dir);

            if(rc.distance < std::numeric_limits<double>::infinity()) {
                img.set_pixel(i, j, rc.color);
            } else {
                img.set_pixel(i, j, Color(0, 0, 0));
            }
        }
    }

    img.save2png("parallel.png");
}

int main(){
    perspective();
    parallel();
}