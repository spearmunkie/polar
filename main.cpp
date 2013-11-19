#include<SDL.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>

SDL_Surface* screen;
int randInt(int min,int max)
{
    return  (rand()%(int)(max-min+1)+min);
}
struct RGB
{
    int r;
    int g;
    int b;
};

const double DEG_TO_RAD=57.29578;

void pset(int x,int y,const RGB& color)
{
    Uint32 colorSDL = SDL_MapRGB(screen->format, color.r, color.g, color.b);
    Uint32* bufp;
    bufp = (Uint32*)screen->pixels + y * screen->pitch / 4 + x;
    *bufp = colorSDL;
}

void drawThickLine(int x1, int y1, int x2, int y2,int thickness,const RGB &color)
{
    //if we are offscreen
    if (x1<0) x1 = 0;
    if (x1> screen->w - 1) x1= screen->w - 1;

    if (x2<0) x2 = 0;
    if (x2> screen->w - 1) x2= screen->w - 1;

    if (y1<0) y1 = 0;
    if (y1> screen->h - 1) y1= screen->h - 1;

    if (y2<0) y2 = 0;
    if (y2> screen->h - 1) y2= screen->h - 1;

    int deltax = fabs(x2 - x1); //The difference between the x's
    int deltay = fabs(y2 - y1); //The difference between the y's
    int x = x1; //Start x off at the first pixel
    int y = y1; //Start y off at the first pixel
    int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels;
    if(x2 >= x1) //The x-values are increasing
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else //The x-values are decreasing
    {
        xinc1 = -1;
        xinc2 = -1;
    }
    if(y2 >= y1) //The y-values are increasing
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else //The y-values are decreasing
    {
        yinc1 = -1;
        yinc2 = -1;
    }
    if (deltax >= deltay) //There is at least one x-value for every y-value
    {
        xinc1 = 0; //Don't change the x when numerator >= denominator
        yinc2 = 0; //Don't change the y for every iteration
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax; //There are more x-values than y-values
    }
    else //There is at least one y-value for every x-value
    {
        xinc2 = 0; //Don't change the x for every iteration
        yinc1 = 0; //Don't change the y when numerator >= denominator
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay; //There are more y-values than x-values
    }

    for (int curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        for(int thk=0;thk<(int)(thickness/2);thk++)
        {
            pset((x+thk) % screen->w, (y+thk) % screen->h, color);  //Draw the current pixel
            pset((x-thk) % screen->w, (y-thk) % screen->h, color);  //Draw the current pixel
        }
        pset((x) % screen->w, (y) % screen->h, color);  //Draw the current pixel
        num += numadd;  //Increase the numerator by the top of the fraction
        if (num >= den) //Check if numerator >= denominator
        {
            num -= den; //Calculate the new numerator value
            x += xinc1; //Change the x as appropriate
            y += yinc1; //Change the y as appropriate
        }
        x += xinc2; //Change the x as appropriate
        y += yinc2; //Change the y as appropriate
  }
}

void polar(int ang,int n)
{
    RGB tmp;

    tmp.r=randInt(150,255);
    tmp.b=randInt(150,255);
    tmp.g=randInt(150,255);
    int a=250;
    for(int x=0;x<360;x++)
    {
        double r=sin((ang+x)/DEG_TO_RAD*n)*a;
        double x1=cos((x)/DEG_TO_RAD)*r+430;
        double y1=sin((x)/DEG_TO_RAD)*r+250;
        r=sin((x+1)/DEG_TO_RAD*n)*a;
        double x2=cos((x+1+ang)/DEG_TO_RAD)*r+430;
        double y2=sin((x+1+ang)/DEG_TO_RAD)*r+250;

        drawThickLine(x1,y1,x2,y2,1,tmp);
    }
}

void cls()
{
  SDL_FillRect(screen, NULL, 65536 * 0 + 256 * 0+ 0);
}

int main( int argc, char* args[] )
{
    int ang=1;
    srand(time(NULL));
    SDL_Event event;
    int n=1;
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return 1;
    screen = SDL_SetVideoMode(860, 500, 32, SDL_SWSURFACE );
    if( screen == NULL )
        return 1;
    SDL_WM_SetCaption( "Hello World!", NULL );
    int mx,my;
    bool done=false;
    while (!done)
    {
        srand(time(NULL));
        if( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_MOUSEMOTION )
            {
                mx=event.motion.x;
                my=event.motion.y;
            }
            switch( event.key.keysym.sym )
            {
                case SDLK_UP: n++; break;
                case SDLK_DOWN: n--; break;
                default: break;
            }
            if( event.type == SDL_QUIT )
                done=true;
        }
        cls();
        ang+=1;
        ang=ang%360;
        polar(ang,n);
        //drawThickLine(0,0,mx,my,1,tmp);
        if( SDL_Flip( screen ) == -1 ) return 1;
    }
    SDL_QUIT;
    return 0;
}
