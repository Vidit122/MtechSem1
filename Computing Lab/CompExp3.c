#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_INPUT 256

void parse_and_render(SDL_Renderer *renderer, TTF_Font *font, const char *expr,
                      int x, int y, SDL_Color color);


// Function to render text at (x, y) with given font and color
void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Function to render subscript expression: sub(base, subscript)
void render_subscript(SDL_Renderer *renderer, TTF_Font *font, const char *expr,
                      int x, int y, SDL_Color color) {
    // Expecting input like sub(x,2)

    // Find positions of '(' and ','
    const char *openParen = strchr(expr, '(');
    const char *comma = strchr(expr, ',');
    const char *closeParen = strrchr(expr, ')');

    if (!openParen || !comma || !closeParen || comma < openParen) {
        // Invalid format, just print it raw
        render_text(renderer, font, expr, x, y, color);
        return;
    }

    // Extract base and subscript text
    char base[128], sub[128];
    strncpy(base, openParen + 1, comma - openParen - 1);
    base[comma - openParen - 1] = '\0';
    strncpy(sub, comma + 1, closeParen - comma - 1);
    sub[closeParen - comma - 1] = '\0';

    // Render base normally
    int baseWidth = 0, baseHeight = 0;
    TTF_SizeText(font, base, &baseWidth, &baseHeight);
    render_text(renderer, font, base, x, y, color);

    // Render subscript with smaller font
    TTF_Font *smallFont = TTF_OpenFont("DejaVuSans.ttf", 14); // smaller size
    if (!smallFont) smallFont = font; // fallback if font not found

    int subWidth = 0, subHeight = 0;
    TTF_SizeText(smallFont, sub, &subWidth, &subHeight);

    // Place subscript slightly lower and smaller
    int subX = x + baseWidth;
    int subY = y + (baseHeight / 3);

    render_text(renderer, smallFont, sub, subX, subY, color);

    if (smallFont != font) {
        TTF_CloseFont(smallFont);
    }
}


// Recursive parser for simple expressions (powers, fractions, derivatives, integrals)
void render_expression(SDL_Renderer* renderer, TTF_Font* font, const char* expr, int x, int y, SDL_Color color) {
    int i = 0, offsetX = 0;


    while (expr[i] != '\0') {
        if (strncmp(&expr[i], "pow(", 4) == 0) { // Handle powers
            i += 4; // skip "pow("

            // ---- Parse Base Expression ----
            int parenCount = 0;
            char baseExpr[128] = "";
            int j = 0;
            while (expr[i] != ',' || parenCount > 0) {
                if (expr[i] == '(') parenCount++;
                if (expr[i] == ')') parenCount--;
                baseExpr[j++] = expr[i++];
            }
            baseExpr[j] = '\0';
            i++; // skip the comma

            // ---- Parse Exponent Expression ----
            parenCount = 0;
            char expExpr[128] = "";
            j = 0;
            while (expr[i] != ')' || parenCount > 0) {
                if (expr[i] == '(') parenCount++;
                if (expr[i] == ')') parenCount--;
                expExpr[j++] = expr[i++];
            }
            expExpr[j] = '\0';
            i++; // skip ')'

            // ---- Render Base ----
            int baseStartX = x + offsetX;
            int baseStartY = y;
            parse_and_render(renderer, font, baseExpr, baseStartX, baseStartY, color);

            // Measure base width
            int baseW = 0, baseH = 0;
            TTF_SizeUTF8(font, baseExpr, &baseW, &baseH);

            // ---- Render Exponent (recursive) ----
            parse_and_render(renderer, font, expExpr, baseStartX + baseW, baseStartY - baseH/2, color);

            offsetX += baseW + 20; // spacing after pow()

            
        } else if (strncmp(&expr[i], "frac(", 5) == 0) { // Handle fractions
            i += 5; // skip "frac("
            char num[32] = "", denom[32] = "";
            int j = 0;
            while (expr[i] != ',' && expr[i] != '\0') num[j++] = expr[i++];
            num[j] = '\0';
            i++; // skip comma
            j = 0;
            while (expr[i] != ')' && expr[i] != '\0') denom[j++] = expr[i++];
            denom[j] = '\0';
            i++; // skip ')'

            // Render numerator
            render_text(renderer, font, num, x + offsetX, y, color);

            SDL_Rect numRect;
            TTF_SizeUTF8(font, num, &numRect.w, &numRect.h);

            // --- Draw line (set draw color explicitly) ---
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawLine(renderer,
                x + offsetX, y + numRect.h + 2,
                x + offsetX + numRect.w, y + numRect.h + 2);

            // Render denominator (slightly below line)
            render_text(renderer, font, denom, x + offsetX, y + numRect.h + 5, color);

            offsetX += numRect.w + 15;
        }

        else if (strncmp(&expr[i], "int", 3) == 0) {
            i += 3;
            char lower[32] = "", upper[32] = "";

            // Parse lower limit
            if (expr[i] == '_') {
                i++; // skip '_'
                if (expr[i] == '{') {
                    i++;
                    int j=0;
                    while (expr[i] != '}' && expr[i] != '\0') lower[j++] = expr[i++];
                    lower[j] = '\0';
                    if (expr[i] == '}') i++;
                }
            }

            // Parse upper limit
            if (expr[i] == '^') {
                i++; // skip '^'
                if (expr[i] == '{') {
                    i++;
                    int j=0;
                    while (expr[i] != '}' && expr[i] != '\0') upper[j++] = expr[i++];
                    upper[j] = '\0';
                    if (expr[i] == '}') i++;
                }
            }

            // Expect '(' next
            if (expr[i] == '(') i++;
                // Render ∫ symbol
                render_text(renderer, font, "∫", x + offsetX, y, color);

                // Render upper limit above integral
                if (strlen(upper) > 0) {
                    render_text(renderer, font, upper, x + offsetX + 10, y - 20, color);
                }

                // Render lower limit below integral
                if (strlen(lower) > 0) {
                    render_text(renderer, font, lower, x + offsetX + 10, y + 20, color);
                }

                offsetX += 40; // space after integral


        } else if (strncmp(&expr[i], "diff(", 5) == 0) { // Handle derivative
            i += 5;
            char inner[64] = "";
            int j = 0;
            while (expr[i] != ')' && expr[i] != '\0') inner[j++] = expr[i++];
            inner[j] = '\0';
            i++;

            render_text(renderer, font, "d/dx(", x + offsetX, y, color);
            SDL_Rect diffRect; TTF_SizeUTF8(font, "d/dx(", &diffRect.w, &diffRect.h);
            render_expression(renderer, font, inner, x + offsetX + diffRect.w, y, color);
            render_text(renderer, font, ")", x + offsetX + diffRect.w + strlen(inner)*10, y, color);
            offsetX += diffRect.w + strlen(inner)*5 + 10;
        } 
        
        else if (strncmp(&expr[i], "sum(", 4) == 0) { 
            i += 4; // skip "sum("
            char lower[32] = "", upper[32] = "", body[128] = "";
            int j = 0;

            // parse lower limit
            while (expr[i] != ',' && expr[i] != '\0') lower[j++] = expr[i++];
            lower[j] = '\0'; 
            i++; j = 0;

            // parse upper limit
            while (expr[i] != ',' && expr[i] != '\0') upper[j++] = expr[i++];
            upper[j] = '\0';
            i++; j = 0;

            // parse body
            while (expr[i] != ')' && expr[i] != '\0') body[j++] = expr[i++];
            body[j] = '\0';
            if (expr[i] == ')') i++;

            // Render Σ symbol
            render_text(renderer, font, "Σ", x + offsetX, y, color);

            // Render limits
            render_text(renderer, font, lower, x + offsetX, y + 20, color);  // below
            render_text(renderer, font, upper, x + offsetX, y - 20, color);  // above

            SDL_Rect sigmaRect;
            TTF_SizeUTF8(font, "Σ", &sigmaRect.w, &sigmaRect.h);
            offsetX += sigmaRect.w + 10;

            // Render body (recursive so it can have pow, frac, etc.)
            render_expression(renderer, font, body, x + offsetX, y, color);

        } 
        else if (strncmp(&expr[i], "sin(", 4) == 0) {
            i += 4;
            char inside[64] = "";
            int j = 0;
            while (expr[i] != ')' && expr[i] != '\0') inside[j++] = expr[i++];
            inside[j] = '\0';
            if (expr[i] == ')') i++;

            render_text(renderer, font, "sin", x + offsetX, y, color);
            SDL_Rect trigRect;
            TTF_SizeUTF8(font, "sin", &trigRect.w, &trigRect.h);
            offsetX += trigRect.w + 5;

            render_text(renderer, font, inside, x + offsetX, y, color);
            TTF_SizeUTF8(font, inside, &trigRect.w, &trigRect.h);
            offsetX += trigRect.w + 15;
        } 
        else if (strncmp(&expr[i], "cos(", 4) == 0) {
            i += 4;
            char inside[64] = "";
            int j = 0;
            while (expr[i] != ')' && expr[i] != '\0') inside[j++] = expr[i++];
            inside[j] = '\0';
            if (expr[i] == ')') i++;

            render_text(renderer, font, "cos", x + offsetX, y, color);
            SDL_Rect trigRect;
            TTF_SizeUTF8(font, "cos", &trigRect.w, &trigRect.h);
            offsetX += trigRect.w + 5;

            render_text(renderer, font, inside, x + offsetX, y, color);
            TTF_SizeUTF8(font, inside, &trigRect.w, &trigRect.h);
            offsetX += trigRect.w + 15;
        }

        else if (strncmp(expr, "sub(", 4) == 0) {
            render_subscript(renderer, font, expr, x, y, color);
            return;
        }

        else { // Render regular characters
            char temp[2] = { expr[i], '\0' };
            render_text(renderer, font, temp, x + offsetX, y, color);
            int w, h;
            TTF_SizeUTF8(font, temp, &w, &h);
            offsetX += w + 2;
            i++;
        }
    }
}

void parse_and_render(SDL_Renderer *renderer, TTF_Font *font, const char *expr,
                      int x, int y, SDL_Color color) {
    render_expression(renderer, font, expr, x, y, color);
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Math Renderer", 100, 100,WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont("DejaVuSerif.ttf", 28);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color black = {0, 0, 0, 255};
    SDL_Color red   = {200, 0, 0, 255};

    char input[MAX_INPUT] = "";
    int running = 1;
    char submitted[MAX_INPUT] = ""; // store last submitted expression
    SDL_StartTextInput();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            if (e.type == SDL_TEXTINPUT) {
                if (strlen(input) + strlen(e.text.text) < MAX_INPUT - 1)
                    strcat(input, e.text.text);
            }

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(input) > 0)
                    input[strlen(input) - 1] = '\0';

                if (e.key.keysym.sym == SDLK_RETURN) {
                    // Save input to submitted buffer
                    strncpy(submitted, input, MAX_INPUT);
                    submitted[MAX_INPUT - 1] = '\0';
                    input[0] = '\0'; // clear input for next typing
                }
            }
        }

        // Clear screen once per frame
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render live typing at top
        if (strlen(input) > 0) {
            SDL_Surface *surf = TTF_RenderText_Solid(font, input, black);
            if (surf) {
                SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_Rect dest = {50, 30, surf->w, surf->h};
                SDL_RenderCopy(renderer, tex, NULL, &dest);
                SDL_DestroyTexture(tex);
                SDL_FreeSurface(surf);
            }
        }

        // Render last submitted expression below
        if (strlen(submitted) > 0)
            parse_and_render(renderer, font, submitted, 50, 150, red);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }



    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
