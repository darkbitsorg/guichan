/*
 * Code that sets up an SDL2 application with Guichan using the
 * Guichan SDL2 back end.
 */

#include <guichan.hpp>
#include <guichan/sdl2.hpp>

namespace sdl2
{
    bool running = true;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // All back ends contain objects to make Guichan work on a
    // specific target - in this case SDL2 - and they are a Graphics
    // object to make Guichan able to draw itself using SDL2, an
    // input objec to make Guichan able to get user input using SDL2
    // and an ImageLoader object to make Guichan able to load images
    // using SDL2.
    gcn::SDL2Graphics* graphics;
    gcn::SDL2Input* input;
    gcn::SDL2ImageLoader* imageLoader;

    /**
     * Initialises the SDL2 application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    void init()
    {
        // We simply initialise SDL2 as we would do with any SDL2 application.
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Guichan SDL2 example",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  640, 480, 0);
        // Guichan draws with a renderer, and images become textures of it.
        renderer = SDL_CreateRenderer(window, -1, 0);
        // Text is delivered to the SDL2Input object through text input
        // events, which we make sure are enabled.
        SDL_StartTextInput();

        // Now it's time to initialise the Guichan SDL2 back end.

        // The image loader needs the renderer to create textures for it.
        imageLoader = new gcn::SDL2ImageLoader(renderer);
        // The ImageLoader Guichan should use needs to be passed to the Image object
        // using a static function.
        gcn::Image::setImageLoader(imageLoader);
        // The Graphics object draws with the renderer.
        graphics = new gcn::SDL2Graphics(renderer);
        input = new gcn::SDL2Input();

        // Now we create the Gui object to be used with this SDL2 application.
        globals::gui = new gcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with SDL2 implementations of these objects hence making Guichan
        // able to utilise SDL2.
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the SDL2 application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    /**
     * Runs the SDL2 application.
     */
    void run()
    {
        // The main loop
        while(running)
        {
            // Check user input
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                }
                else if(event.type == SDL_QUIT)
                {
                    running = false;
                }

                // After we have manually checked user input with SDL2 for
                // any attempt by the user to halt the application we feed
                // the input to Guichan by pushing the input to the Input
                // object.
                input->pushInput(event);
            }
            // Now we let the Gui object perform its logic.
            globals::gui->logic();
            // We clear the screen before letting the Gui object draw itself.
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            globals::gui->draw();
            // Finally we present what has been drawn.
            SDL_RenderPresent(renderer);
        }
    }
}
