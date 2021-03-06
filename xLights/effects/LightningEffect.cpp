#include "LightningEffect.h"
#include "LightningPanel.h"

#include "../sequencer/Effect.h"
#include "../RenderBuffer.h"
#include "../UtilClasses.h"

#include "../../include/lightning-16.xpm"
#include "../../include/lightning-24.xpm"
#include "../../include/lightning-32.xpm"
#include "../../include/lightning-48.xpm"
#include "../../include/lightning-64.xpm"

LightningEffect::LightningEffect(int id) : RenderableEffect(id, "Lightning", lightning_16, lightning_24, lightning_32, lightning_48, lightning_64)
{
    //ctor
}

LightningEffect::~LightningEffect()
{
    //dtor
}

wxPanel *LightningEffect::CreatePanel(wxWindow *parent) {
    return new LightningPanel(parent);
}

#define DOWN 0
#define UP 1
#define RIGHT 2
#define LEFT 3
void LightningEffect::Render(Effect *effect, const SettingsMap &SettingsMap, RenderBuffer &buffer) {
    int Number_Bolts = SettingsMap.GetInt("SLIDER_Number_Bolts", 1);
    int Number_Segments = SettingsMap.GetInt("SLIDER_Number_Segments", 1);
    bool ForkedLightning = SettingsMap.GetBool("CHECKBOX_ForkedLightning");
    int topX = SettingsMap.GetInt("SLIDER_Lightning_TopX", 0);
    int topY = SettingsMap.GetInt("SLIDER_Lightning_TopY", 0);
    int botX = SettingsMap.GetInt("SLIDER_Lightning_BOTX", 100);
    int botY = SettingsMap.GetInt("SLIDER_Lightning_BOTY", 100);
    
    if (Number_Bolts == 0) {
        Number_Bolts = 1;
    }
    if (Number_Segments == 0) {
        Number_Segments = 1;
    }
    xlColor color;
    int x1,x2,y1,y2,x3,y3,xc,yc,step,ystep;
    int i,j,segment,StepSegment, xoffset;
    int curState = (buffer.curPeriod - buffer.curEffStartPer);
    int DIRECTION=UP;
    
    
    ystep=buffer.BufferHt/10;
    xc=buffer.BufferWi/2;
    yc=buffer.BufferHt/2;
    
    i=0;
    
    step=5;
    StepSegment=buffer.BufferHt/Number_Bolts;
    segment=curState%Number_Bolts;
    segment*=4;
    if(segment>Number_Bolts) segment=Number_Bolts;
    if(curState>Number_Bolts)  segment=Number_Bolts;
    buffer.palette.GetColor(0, color);
    if(DIRECTION==DOWN) {
        x1=xc + topX;
        y1=buffer.BufferHt - topY;
    } else if(DIRECTION==UP) {
        x1=xc + topX;
        y1=topY;
    }
    
    xoffset=curState/2;
    xoffset=curState*botX/10.0;
    for(i=0; i<=segment; i++) {
        //0  x2=bolt[i].x1;
        //  y2=bolt[i].y1;
        j=rand()+1;
        if(DIRECTION==UP || DIRECTION==DOWN) {
            if(i%2==0) { // Every even segment will alternate direction
                if(rand()%2==0) // target x is to the left
                    x2 = xc + topX - (j%Number_Segments);
                else // but randomely we reverse direction, also make it a larger jag
                    x2 = xc + topX + (2*j%Number_Segments);
            } else { // odd segments will
                if(rand()%2==0) // move to the right
                    x2 = xc + topX + (j%Number_Segments);
                else // but sometimes move 3 units to left.
                    x2 = xc + topX - (3*j%Number_Segments);
            }
            if(DIRECTION==DOWN)
                y2 = buffer.BufferHt-(i*StepSegment) - topY;
            else if(DIRECTION==UP)
                y2 = (i*StepSegment) + topY;
        } else if(DIRECTION==LEFT || DIRECTION==RIGHT) {
            
        }
        LightningDrawBolt(buffer, x1+xoffset,y1,x2+xoffset,y2,color,curState);
        if(ForkedLightning) {
            if(i>(segment/2)) {
                
                if(i%2==1) {
                    if(rand()%2==1)
                        x3 = xc + topX - (j%Number_Segments);
                    else  x3 = xc + topX + (2*j%Number_Segments);
                } else {
                    if(rand()%2==1)
                        x3 = xc + topX + (j%Number_Segments);
                    else
                        x3 = xc + topX - (3*j%Number_Segments);
                }
                y3 = buffer.BufferHt-(i*StepSegment) - topY - rand()%5;
                LightningDrawBolt(buffer, x1+xoffset,y1,x3+xoffset,y2,color,curState);
            }
        }
        x1=x2;
        y1=y2;
    }
}

void LightningEffect::LightningDrawBolt(RenderBuffer &buffer, const int x0_, const int y0_, const int x1_, const int y1_,  xlColor& color ,int curState) {
    int x0 = x0_;
    int x1 = x1_;
    int y0 = y0_;
    int y1 = y1_;
    xlColor color2;
    HSVValue hsv = color.asHSV();
    HSVValue hsv2 = color2.asHSV();
    //   if(x0<0 || x1<0 || y1<0 || y0<0) return;
    
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    color2=color;
    // color2.red=color2.green=color2.blue=200;
    //   int frame_startfade = 2*20; // 2 seconds full brightness
    //   int frame_fadedone = 5*20; // 3 seconds to fade o
    
    color = hsv;
    
    hsv2.value = hsv.value * 0.90;
    color2 = hsv2;
    color2 = hsv;
    for(;;) {
        buffer.SetPixel(x0,y0, color);
        buffer.SetPixel(x0-1,y0, color2);
        buffer.SetPixel(x0+1,y0, color2);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}
