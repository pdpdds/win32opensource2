
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


#include <FreeImage.h>




#include <FreeImage.h>

#include <SDL2/SDL.h>

char* parse_args_get_filename(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILENAME\n", argv[0]);
        exit(2);
    }
    char* filename = argv[1];
    return filename;
}

/** Initialise a FreeImage bitmap and return a pointer to it. */
FIBITMAP* get_freeimage_bitmap(char* filename) {
    FREE_IMAGE_FORMAT filetype = FreeImage_GetFileType(filename, 0);
    FIBITMAP* freeimage_bitmap = FreeImage_Load(filetype, filename, 0);
    return freeimage_bitmap;
}

/** Initialise a SDL surface and return a pointer to it.
 *
 *  This function flips the FreeImage bitmap vertically to make it compatible
 *  with SDL's coordinate system.
 *
 *  If the input image is in grayscale a custom palette is created for the
 *  surface.
 */
SDL_Surface* get_sdl_surface(FIBITMAP* freeimage_bitmap, int is_grayscale) {

    // Loaded image is upside down, so flip it.
    FreeImage_FlipVertical(freeimage_bitmap);

    SDL_Surface* sdl_surface = SDL_CreateRGBSurfaceFrom(
        FreeImage_GetBits(freeimage_bitmap),
        FreeImage_GetWidth(freeimage_bitmap),
        FreeImage_GetHeight(freeimage_bitmap),
        FreeImage_GetBPP(freeimage_bitmap),
        FreeImage_GetPitch(freeimage_bitmap),
        FreeImage_GetRedMask(freeimage_bitmap),
        FreeImage_GetGreenMask(freeimage_bitmap),
        FreeImage_GetBlueMask(freeimage_bitmap),
        0
    );

    if (sdl_surface == NULL) {
        fprintf(stderr, "Failed to create surface: %s\n", SDL_GetError());
        exit(1);
    }

    if (is_grayscale) {
        // To display a grayscale image we need to create a custom palette.
        SDL_Color colors[256];
        int i;
        for (i = 0; i < 256; i++) {
            colors[i].r = colors[i].g = colors[i].b = i;
        }
        SDL_SetPaletteColors(sdl_surface->format->palette, colors, 0, 256);
    }

    return sdl_surface;
}

/** Initialise a SDL window and return a pointer to it. */
SDL_Window* get_sdl_window(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL couldn't initialise: %s.\n", SDL_GetError());
        exit(1);
    }

    SDL_Window* sdl_window;
    sdl_window = SDL_CreateWindow("Image",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS);

    return sdl_window;
}

/** Display the image by rendering the surface as a texture in the window. */
void render_image(SDL_Window* window, SDL_Surface* surface) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "Failed to load image as texture\n");
        exit(1);
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

/** Loop until a key is pressed. */
void event_loop() {
    int done = 0;
    SDL_Event e;
    while (!done) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                done = 1;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    char* filename = parse_args_get_filename(argc, argv);
    FIBITMAP* freeimage_bitmap = get_freeimage_bitmap(filename);

    int is_grayscale = 0;
    if (FreeImage_GetColorType(freeimage_bitmap) == FIC_MINISBLACK) {
        // Single channel so ensure image is compressed to 8-bit.
        is_grayscale = 1;
        FIBITMAP* tmp_bitmap = FreeImage_ConvertToGreyscale(freeimage_bitmap);
        FreeImage_Unload(freeimage_bitmap);
        freeimage_bitmap = tmp_bitmap;
    }

    int width = FreeImage_GetWidth(freeimage_bitmap);
    int height = FreeImage_GetHeight(freeimage_bitmap);
    SDL_Window* sdl_window = get_sdl_window(width, height);
    SDL_Surface* sdl_surface = get_sdl_surface(freeimage_bitmap, is_grayscale);

    render_image(sdl_window, sdl_surface);
    event_loop();

    FreeImage_Unload(freeimage_bitmap);
    SDL_FreeSurface(sdl_surface);
    return 0;
}