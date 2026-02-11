#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void parse_and_render(SDL_Renderer *renderer, TTF_Font *font, const char *expr,
                      int x, int y, SDL_Color color);


void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void render_subscript(SDL_Renderer *renderer, TTF_Font *font, const char *expr,
                      int x, int y, SDL_Color color) {

    const char *openParen = strchr(expr, '(');
    const char *comma = strchr(expr, ',');
    const char *closeParen = strrchr(expr, ')');

    if (!openParen || !comma || !closeParen || comma < openParen) {
        render_text(renderer, font, expr, x, y, color);
        return;
    }

    char base[128], sub[128];
    strncpy(base, openParen + 1, comma - openParen - 1);
    base[comma - openParen - 1] = '\0';
    strncpy(sub, comma + 1, closeParen - comma - 1);
    sub[closeParen - comma - 1] = '\0';

    int baseWidth = 0, baseHeight = 0;
    TTF_SizeText(font, base, &baseWidth, &baseHeight);
    render_text(renderer, font, base, x, y, color);

    TTF_Font *smallFont = TTF_OpenFont("DejaVuSans.ttf", 14); 
    if (!smallFont) smallFont = font; 

    int subWidth = 0, subHeight = 0;
    TTF_SizeText(smallFont, sub, &subWidth, &subHeight);

    int subX = x + baseWidth;
    int subY = y + (baseHeight / 3);

    render_text(renderer, smallFont, sub, subX, subY, color);

    if (smallFont != font) {
        TTF_CloseFont(smallFont);
    }
}


void render_expression(SDL_Renderer* renderer, TTF_Font* font, const char* expr, int x, int y, SDL_Color color) {
    int i = 0, offsetX = 0;


    while (expr[i] != '\0') {

        // POWER
        if (strncmp(&expr[i], "pow(", 4) == 0) {
            i += 4;
            int parenCount = 0;
            char baseExpr[128] = "";
            int j = 0;
            while (expr[i] != ',' || parenCount > 0) {
                if (expr[i] == '(') parenCount++;
                if (expr[i] == ')') parenCount--;
                baseExpr[j++] = expr[i++];
            }
            baseExpr[j] = '\0';
            i++;

            parenCount = 0;
            char expExpr[128] = "";
            j = 0;
            while (expr[i] != ')' || parenCount > 0) {
                if (expr[i] == '(') parenCount++;
                if (expr[i] == ')') parenCount--;
                expExpr[j++] = expr[i++];
            }
            expExpr[j] = '\0';
            i++; 

            int baseStartX = x + offsetX;
            int baseStartY = y;
            parse_and_render(renderer, font, baseExpr, baseStartX, baseStartY, color);

            int baseW = 0, baseH = 0;
            TTF_SizeUTF8(font, baseExpr, &baseW, &baseH);

            parse_and_render(renderer, font, expExpr, baseStartX + baseW, baseStartY - baseH/2, color);

            offsetX += baseW + 20;

            
        }  

        //FRACTION
        else if (strncmp(&expr[i], "frac(", 5) == 0) { 
            i += 5;
            char num[32] = "", denom[32] = "";
            int j = 0;
            while (expr[i] != ',' && expr[i] != '\0') num[j++] = expr[i++];
            num[j] = '\0';
            i++;
            j = 0;
            while (expr[i] != ')' && expr[i] != '\0') denom[j++] = expr[i++];
            denom[j] = '\0';
            i++;

            render_text(renderer, font, num, x + offsetX, y, color);

            SDL_Rect numRect;
            TTF_SizeUTF8(font, num, &numRect.w, &numRect.h);

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawLine(renderer,
                x + offsetX, y + numRect.h + 2,
                x + offsetX + numRect.w, y + numRect.h + 2);

            render_text(renderer, font, denom, x + offsetX, y + numRect.h + 5, color);

            offsetX += numRect.w + 15;
        }


        //INTEGRATION
        else if (strncmp(&expr[i], "int", 3) == 0) {
            i += 3;
            char lower[32] = "", upper[32] = "";

            if (expr[i] == '_') {
                i++; 
                if (expr[i] == '{') {
                    i++;
                    int j=0;
                    while (expr[i] != '}' && expr[i] != '\0') lower[j++] = expr[i++];
                    lower[j] = '\0';
                    if (expr[i] == '}') i++;
                }
            }

            if (expr[i] == '^') {
                i++;
                if (expr[i] == '{') {
                    i++;
                    int j=0;
                    while (expr[i] != '}' && expr[i] != '\0') upper[j++] = expr[i++];
                    upper[j] = '\0';
                    if (expr[i] == '}') i++;
                }
            }

            if (expr[i] == '(') i++;
                render_text(renderer, font, "∫", x + offsetX, y, color);
                if (strlen(upper) > 0) {
                    render_text(renderer, font, upper, x + offsetX + 10, y - 20, color);
                }

                if (strlen(lower) > 0) {
                    render_text(renderer, font, lower, x + offsetX + 10, y + 20, color);
                }

                offsetX += 40;


        } 
        
        //DIFFERENTIATION
        else if (strncmp(&expr[i], "diff(", 5) == 0) { 
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
            offsetX += diffRect.w + strlen(inner)*10 + 10;
        } 
        

        //SUMMATION
        else if (strncmp(&expr[i], "sum(", 4) == 0) { 
            i += 4; 
            char lower[32] = "", upper[32] = "", body[128] = "";
            int j = 0;

            while (expr[i] != ',' && expr[i] != '\0') lower[j++] = expr[i++];
            lower[j] = '\0'; 
            i++; j = 0;
            while (expr[i] != ',' && expr[i] != '\0') upper[j++] = expr[i++];
            upper[j] = '\0';
            i++; j = 0;

            while (expr[i] != ')' && expr[i] != '\0') body[j++] = expr[i++];
            body[j] = '\0';
            if (expr[i] == ')') i++;

            render_text(renderer, font, "Σ", x + offsetX, y, color);

            render_text(renderer, font, lower, x + offsetX, y + 20, color);  // below
            render_text(renderer, font, upper, x + offsetX, y - 20, color);  // above

            SDL_Rect sigmaRect;
            TTF_SizeUTF8(font, "Σ", &sigmaRect.w, &sigmaRect.h);
            offsetX += sigmaRect.w + 10;

            render_expression(renderer, font, body, x + offsetX, y, color);

        } 

        // SIN(X)
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


        //COS(X)
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


        //SUBSCRIPT 
        else if (strncmp(expr, "sub(", 4) == 0) {
            render_subscript(renderer, font, expr, x, y, color);
            return;
        }

        else { 
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

    SDL_Window* window = SDL_CreateWindow("Math Renderer", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont("DejaVuSerif.ttf", 24);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color color = {0, 0, 0, 255};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    render_expression(renderer, font, "pow(x,pow(2,3))", 50, 50, color);

    SDL_RenderPresent(renderer);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = 1;
        }
        SDL_Delay(100);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
