#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#undef main

struct block
{
    int wall;
    int open;
    int closed;
    int f;
    int h;
    int g;
    int gTemp;
    int prevX;
    int prevY;
};

struct path
{
    int x;
    int y;
};

int heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("my nuts", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 450, 450, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    srand(time(NULL));

    struct block grid[30][30];

    int done = 0;

    int arr[30 * 30] = {
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        };


    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            grid[i][j].wall = (rand() % 3); //arr[i * 30 + j]; 
            grid[i][j].closed = 0;
            grid[i][j].f = 0;
            grid[i][j].g = 0;
            grid[i][j].gTemp = 0;
            grid[i][j].prevX = 0;
            grid[i][j].prevY = 0;

            if(i == 0 && j == 0)
            {
                grid[i][j].open = 1;
                grid[i][j].wall = 0;
                grid[i][j].h = heuristic(i, j, 29, 29);
                grid[i][j].f = grid[i][j].g + grid[i][j].h + 1;
            }
            else if(i == 29 && j == 29)
            {
                grid[i][j].wall = 0;
            }
            else
            {
                grid[i][j].open = 0;
            }
        }
    }

    while (1)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }

        int winX = 0, winY = 0;

        if(done != 1)
        {
            for(int i = 0; i < 30; i++)
            {
                for(int j = 0; j < 30; j++)
                {
                    if(grid[j][i].open == 1)
                    {
                        if(grid[j][i].f < grid[winX][winY].f)
                        {
                            winX = j;
                            winY = i;
                        }
                    }
                }
            }
        }
        
        if(winX == 29 && winY == 29)
        {
            printf("Done");
            done = 1;
        }

        grid[winX][winY].open = 0;
        grid[winX][winY].closed = 1;

        if(winX < 29)
        {
            if(grid[winX + 1][winY].closed != 1 && grid[winX + 1][winY].wall != 1)
            {
                grid[winX + 1][winY].gTemp = grid[winX][winY].g + 1;
                if(grid[winX + 1][winY].gTemp < grid[winX + 1][winY].g)
                {
                    grid[winX + 1][winY].g = grid[winX + 1][winY].gTemp;
                }
                else
                {
                    grid[winX + 1][winY].g = grid[winX + 1][winY].gTemp;
                    grid[winX + 1][winY].open = 1;
                }

                grid[winX + 1][winY].h = heuristic(winX + 1, winY, 29, 29);
                grid[winX + 1][winY].f = grid[winX + 1][winY].g + grid[winX + 1][winY].h;
                grid[winX + 1][winY].prevX = winX;
                grid[winX + 1][winY].prevY = winY;
            }
        }
        if(winX > 0)
        {
            if(grid[winX - 1][winY].closed != 1 && grid[winX - 1][winY].wall != 1)
            {
                grid[winX - 1][winY].gTemp = grid[winX][winY].g + 1;
                if(grid[winX - 1][winY].gTemp < grid[winX - 1][winY].g)
                {
                    grid[winX - 1][winY].g = grid[winX - 1][winY].gTemp;
                }
                else
                {
                    grid[winX - 1][winY].g = grid[winX - 1][winY].gTemp;
                    grid[winX - 1][winY].open = 1;
                }

                grid[winX - 1][winY].h = heuristic(winX - 1, winY, 29, 29);
                grid[winX - 1][winY].f = grid[winX - 1][winY].g + grid[winX - 1][winY].h;
                grid[winX - 1][winY].prevX = winX;
                grid[winX - 1][winY].prevY = winY;
            }
        }
        if(winY < 29)
        {
            if(grid[winX][winY + 1].closed != 1 && grid[winX][winY + 1].wall != 1)
            {
                grid[winX][winY + 1].gTemp = grid[winX][winY].g + 1;
                if(grid[winX][winY + 1].gTemp < grid[winX][winY + 1].g)
                {
                    grid[winX][winY + 1].g = grid[winX][winY + 1].gTemp;
                }
                else
                {
                    grid[winX][winY + 1].g = grid[winX][winY + 1].gTemp;
                    grid[winX][winY + 1].open = 1;
                }

                grid[winX][winY + 1].h = heuristic(winX, winY + 1, 29, 29);
                grid[winX][winY + 1].f = grid[winX][winY + 1].g + grid[winX][winY + 1].h;
                grid[winX][winY + 1].prevX = winX;
                grid[winX][winY + 1].prevY = winY;
            }
        }
        if(winY > 0)
        {
            if(grid[winX][winY - 1].closed != 1 && grid[winX][winY - 1].wall != 1)
            {
                grid[winX][winY - 1].gTemp = grid[winX][winY].g + 1;
                if(grid[winX][winY - 1].gTemp < grid[winX][winY - 1].g)
                {
                    grid[winX][winY - 1].g = grid[winX][winY - 1].gTemp;
                }
                else
                {
                   grid[winX][winY - 1].g = grid[winX][winY - 1].gTemp;
                   grid[winX][winY - 1].open = 1;
                }

                grid[winX][winY - 1].h = heuristic(winX, winY - 1, 29, 29);
                grid[winX][winY - 1].f = grid[winX][winY - 1].g + grid[winX][winY - 1].h;
                grid[winX][winY - 1].prevX = winX;
                grid[winX][winY - 1].prevY = winY;
            }
        }

        SDL_RenderClear(renderer);

        for(int i = 0; i < 30; i++)
        {
            for(int j = 0; j < 30; j++)
            {
                SDL_Rect rect;
                rect.x = j * 15;
                rect.y = i * 15;
                rect.w = 14;
                rect.h = 14;

                if(grid[i][j].wall != 1)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                }
                if(grid[i][j].open == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                }
                if(grid[i][j].closed == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                }
                    
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}