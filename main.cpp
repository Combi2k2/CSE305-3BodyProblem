#include "include/threadSafeQueue.h"
#include "include/transition.h"
#include "include/utils.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <limits>
#include <Magick++.h>
#include <SDL2/SDL.h>

static int nBodies;
static int nThreads;

static ThreadSafeQueue<std::vector<Body>> frameQueue;
static std::vector<Body> bodies;

extern double SCALE;

void renderSimulation(const std::vector<Body> &bodies, Magick::Image &frame) {
    frame.strokeColor("transparent");
    frame.fillColor("white");
    frame.draw(Magick::DrawableRectangle(0, 0, frame.columns(), frame.rows()));

    for (const Body &body : bodies) {
        double x = SCALE * body.center[0] + frame.columns() / 2;
        double y = SCALE * body.center[1] + frame.rows() / 2;
        double circle_radius = std::min(std::max(body.radius * SCALE, 3.0), 10.0);
        frame.fillColor("black");
        frame.draw(Magick::DrawableCircle(x, y, x + circle_radius, y));
    }
}
void renderFrame(SDL_Renderer* renderer, Magick::Image& frame) {
    // Get the image dimensions
    int width = frame.columns();
    int height = frame.rows();

    // Convert the ImageMagick image to raw pixel data
    std::vector<unsigned char> pixels(width * height * 4); // RGBA
    frame.write(0, 0, width, height, "RGBA", Magick::CharPixel, pixels.data());

    // Create an SDL surface from the pixel data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels.data(), width, height, 32, width * 4, SDL_PIXELFORMAT_RGBA32);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Clear the renderer and draw the texture
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Clean up
    SDL_DestroyTexture(texture);
}

void arg_parse(const std::string &filename) {
    std::ifstream fin(filename.c_str());

    fin >> nBodies;
    fin >> nThreads;

    std::string init_method;
    std::getline(fin, init_method);
    std::getline(fin, init_method);

    bodies.resize(nBodies);

    if (init_method == "RANDOM")        initialize_random(bodies, nBodies);
    if (init_method == "4GALAXIES")     initialize_4galaxies(bodies, nBodies);
    if (init_method == "SOLARSYSTEM")   initialize_solarsystem(bodies, nBodies);
    if (init_method == "MANUAL") {
        for (int i = 0 ; i < nBodies ; ++i) {
            fin >> bodies[i].mass;
            fin >> bodies[i].radius;
            fin >> bodies[i].center[0] >> bodies[i].center[1];
            fin >> bodies[i].speed[0]  >> bodies[i].speed[1];
        }
    }
}

void computeThread() {
    while (true) {
        update(bodies, nThreads);
        frameQueue.push(bodies);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " " << "file\n";
        return 1;
    }
    arg_parse(argv[1]);

    std::thread A = std::thread(&computeThread);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Create an SDL window
    SDL_Window* win = SDL_CreateWindow(
        "N body Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    // Create an SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(win);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Magick::InitializeMagick(nullptr); // <--- added this line
    Magick::Image frame(Magick::Geometry(400, 400), Magick::Color("white"));
    frame.strokeColor("black");
    frame.fillColor("none");

    std::vector<Body> tmp;
    
    while (true) {
        if (frameQueue.pop(tmp)) {
            renderSimulation(tmp, frame);
            renderFrame(renderer, frame);
            SDL_Delay(int(DELTAT * 1000));
        } else if (frameQueue.isFinished()) {
            break;
        }
        // Handle SDL events to prevent the window from freezing
        SDL_Event e;
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(win);
                SDL_Quit();
                return 0;
            }
    }
    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    A.join();

    return 0;
}