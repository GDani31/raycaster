#define IMM2D_IMPLEMENTATION
#define IMM2D_WIDTH 1024
#define IMM2D_HEIGHT 512
#define IMM2D_SCALE 1
#include "immediate2d.h"
#include <math.h>
#define PI 3.14159265359
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

float px = 300;
float py = 300;
float pa = PI;
float pdx = cos(pa)*20;
float pdy = sin(pa)*20;

int mapX = 8;
int mapY = 8;
int mapS = 64;

int map[] = 
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,1,1,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

float dist( float ax , float ay , float bx , float by , float ang )
{
    return ( sqrt( ( bx - ax ) * ( bx - ax ) + ( by - ay ) * ( by - ay ) ) );
}

void drawMap2D()
{
    Color tmp = Black;
    int x, y, x0, y0;

    for( y=0; y < mapY; y++ )
    {
        for( x=0; x < mapX; x++ )
        {
            if( map[y*mapX+x] == 1 ){ tmp = White; } else { tmp = Black; }
            x0 = x*mapS;
            y0 = y*mapS;

            DrawRectangle( x0 , y0 , mapS-3 , mapS-3 , tmp , tmp );

        }
    }

}


void drawPlayer()
{
    DrawCircle( (int)px , (int)py , 5 , Yellow , Yellow );
    DrawLine( px , py , px + pdx , pdy + py , 2 , Yellow );
    //DrawLine( px , py , px+50 , py+50 , 2 , Red );
}

void movePlayer()
{
    char c = LastKey();
    if( c == 'w' ){ px += pdx; py += pdy; }
    if( c == 's' ){ px -= pdx; py -= pdy; }
    if( c == 'a' ){ pa -= 0.1; if( pa < 0 )   { pa += 2*PI; } pdx = cos(pa)*20 ; pdy = sin(pa)*20; }
    if( c == 'd' ){ pa += 0.1; if( pa > 2*PI ){ pa -= 2*PI; } pdx = cos(pa)*20 ; pdy = sin(pa)*20; }

    if( c == Esc ) CloseWindow();
}

void drawRays2D()
{
    int r , mx , my , mp , dof;
    float rx, ry , ra , x0 , y0 , disT;
    Color tmp;

    ra = pa-DR*30; if( ra < 0 ){ ra += 2*PI; } if( ra > 2*PI ){ ra -= 2*PI; }

    for( r = 0 ; r < 60 ; r++ )
    {
        float disH = 1000000 , hx = px , hy = py;
        dof = 0;
        float aTan = -1/tan(ra);
        if( ra > PI ){ ry = (((int)py>>6)<<6)-0.0001; rx = (py-ry) * aTan + px; y0 = -64; x0 = -y0 * aTan; }
        if( ra < PI ){ ry = (((int)py>>6)<<6) + 64  ; rx = (py-ry) * aTan + px; y0 =  64; x0 = -y0 * aTan; }
        if( ra == 0 || ra == PI ){ rx = px; ry = py; dof = 8; }
        while( dof < 8 )
        {
            mx = (int)(rx)>>6; my = (int)(ry)>>6; mp = my * mapX + mx;
            if( mp > 0 && mp < mapX * mapY && map[mp] == 1 ){ dof = 8; hx = rx; hy = ry; disH = dist( px , py , hx , hy, ra ); }
            else{ rx += x0; ry += y0; dof += 1; }
            
        }
        //DrawLine( px , py , rx , ry , 2 , Green );

        float disV = 1000000 , vx = px , vy = py;
        dof = 0;
        float nTan = -tan(ra);
        if( ra > P2 && ra < P3 ){ rx = (((int)px>>6)<<6)-0.0001; ry = (px-rx) * nTan + py; x0 = -64; y0 = -x0 * nTan; }
        if( ra < P2 || ra > P3 ){ rx = (((int)px>>6)<<6) + 64  ; ry = (px-rx) * nTan + py; x0 =  64; y0 = -x0 * nTan; }
        if( ra == 0 || ra == PI ){ rx = px; ry = py; dof = 8; }
        while( dof < 8 )
        {
            mx = (int)(rx)>>6; my = (int)(ry)>>6; mp = my * mapX + mx;
            if( mp > 0 && mp < mapX * mapY && map[mp] == 1 ){ dof = 8; vx = rx; vy = ry; disV = dist( px , py , vx , vy, ra ); }
            else{ rx += x0; ry += y0; dof += 1; }
            
        }
        if( disV < disH ){ rx = vx; ry = vy; disT = disV; tmp = MakeColor( 190 , 0 , 0 ); }
        if( disH < disV ){ rx = hx; ry = hy; disT = disH; tmp = MakeColor( 150 , 0 , 0 ); }
        DrawLine( px , py , rx , ry , 2 , Red );

        float ca = pa - ra; if( ca < 0 ){ ca += 2*PI; } if( ca > 2*PI ){ ca -= 2*PI; } disT = disT * cos( ca );
        float lineH = ( mapS * IMM2D_HEIGHT ) / disT; if( lineH > IMM2D_HEIGHT ){ lineH = IMM2D_HEIGHT; }
        float lineO = IMM2D_HEIGHT/2 - lineH / 2;
        DrawLine( (int)(r*8+(IMM2D_WIDTH/2)+10) , (int)lineO , (int)(r*8+(IMM2D_WIDTH/2)+10) , (int)(lineH+lineO) , 8 , tmp );

        ra += DR; if( ra < 0 ){ ra += 2*PI; } if( ra > 2*PI ){ ra -= 2*PI; }
    }

}

void run()
{
    while( true )
    {
        Wait(5);
        movePlayer();
        Clear(LightGray);
        drawMap2D();
        drawPlayer();
        drawRays2D();



    }
}