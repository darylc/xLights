/***************************************************************
 * Name:      RgbEffects.cpp
 * Purpose:   Implements RGB effects
 * Author:    Matt Brown (dowdybrown@yahoo.com)
 * Created:   2012-12-23
 * Copyright: 2012 by Matt Brown
 * License:
     This file is part of xLights.

    xLights is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xLights is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xLights.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************/
#include <cmath>
#include "RenderBuffer.h"
#include "Effect.h"

inline void unshare(wxObject &o) {
    if (o.GetRefData() != nullptr) {
        o.UnShare();
    }
}

DrawingContext::DrawingContext(int BufferWi, int BufferHt) : nullBitmap(wxNullBitmap)
{
    unshare(nullBitmap);
    image = new wxImage(BufferWi > 0 ? BufferWi : 1, BufferHt > 0 ? BufferHt : 1);
#if wxUSE_GRAPHICS_CONTEXT
    image->SetAlpha();
    for(wxCoord x=0; x<BufferWi; x++) {
        for(wxCoord y=0; y<BufferHt; y++) {
            image->SetAlpha(x, y, wxIMAGE_ALPHA_TRANSPARENT);
        }
    }
#endif
    bitmap = new wxBitmap(*image);
    dc = new wxMemoryDC(*bitmap);


    //make sure we UnShare everything that is being held onto
    //also use "non-normal" defaults to avoid "==" issue that
    //would keep it from using the non-shared versions
    wxFont font(*wxITALIC_FONT);
    unshare(font);
    dc->SetFont(font);

    wxBrush brush(*wxYELLOW_BRUSH);
    unshare(brush);
    dc->SetBrush(brush);
    dc->SetBackground(brush);

    wxPen pen(*wxGREEN_PEN);
    unshare(pen);
    dc->SetPen(pen);

#ifndef LINUX
    wxColor c(12, 25, 3);
    unshare(c);
    dc->SetTextBackground(c);

    wxColor c2(0, 35, 5);
    unshare(c2);
    dc->SetTextForeground(c2);
#endif

    dc->SelectObject(nullBitmap);
    delete bitmap;
    bitmap = nullptr;
#if wxUSE_GRAPHICS_CONTEXT
    gc = nullptr;
#endif
}


DrawingContext::~DrawingContext() {
#if wxUSE_GRAPHICS_CONTEXT
    if (gc != nullptr) {
        delete gc;
    }
#endif
    if (dc != nullptr) {
        delete dc;
    }
    if (bitmap != nullptr) {
        delete bitmap;
    }
    if (image != nullptr) {
        delete image;
    }
}
void DrawingContext::Clear(bool forceGraphicsContext) {
#if wxUSE_GRAPHICS_CONTEXT
    if (gc != nullptr) {
        delete gc;
        gc = nullptr;
    }
#endif

    dc->SelectObject(nullBitmap);
    if (bitmap != nullptr) {
        delete bitmap;
    }
    image->Clear();
#if wxUSE_GRAPHICS_CONTEXT
    #ifdef __WXMSW__
        if (forceGraphicsContext)
        {
            image->SetAlpha();
            for(wxCoord x=0; x<image->GetWidth(); x++) {
                for(wxCoord y=0; y<image->GetHeight(); y++) {
                    image->SetAlpha(x, y, wxIMAGE_ALPHA_TRANSPARENT);
                }
            }
            bitmap = new wxBitmap(*image, 32);
        }
        else
        {
            bitmap = new wxBitmap(*image);
        }
    #else
        image->SetAlpha();
        for(wxCoord x=0; x<image->GetWidth(); x++) {
            for(wxCoord y=0; y<image->GetHeight(); y++) {
                image->SetAlpha(x, y, wxIMAGE_ALPHA_TRANSPARENT);
            }
        }
        bitmap = new wxBitmap(*image, 32);
    #endif
#else
    bitmap = new wxBitmap(*image);
#endif

    dc->SelectObject(*bitmap);

#if wxUSE_GRAPHICS_CONTEXT
    #ifdef LINUX
        gc = wxGraphicsContext::Create(*image);
    #else
        gc = wxGraphicsContext::Create(*dc);
    #endif // LINUX
    #ifdef __WXMSW__
        if (forceGraphicsContext)
        {
            gc->SetAntialiasMode(wxANTIALIAS_NONE);
        }
        else
        {
            // KW: It would be nice if there was an equivalent way to set anti aliasing here on the dc
            //dc->SetAntialiasMode(wxANTIALIAS_NONE);
        }
    #else
        gc->SetAntialiasMode(wxANTIALIAS_NONE);
    #endif
    gc->SetInterpolationQuality(wxInterpolationQuality::wxINTERPOLATION_FAST);
    gc->SetCompositionMode(wxCompositionMode::wxCOMPOSITION_SOURCE);
    //gc->SetCompositionMode(wxCompositionMode::wxCOMPOSITION_OVER);
#endif
}

wxImage *DrawingContext::FlushAndGetImage() {
#if wxUSE_GRAPHICS_CONTEXT
    if (gc != nullptr) {
        gc->Flush();
        delete gc;
        gc = nullptr;
    }
#endif
#ifndef LINUX
    dc->SelectObject(nullBitmap);
    *image = bitmap->ConvertToImage();
    dc->SelectObject(*bitmap);
#endif // LINUX
    return image;
}

void DrawingContext::SetFont(wxFont &font, const xlColor &color) {
#if wxUSE_GRAPHICS_CONTEXT
    #ifdef __WXMSW__
        dc->SetFont(font);
        dc->SetTextForeground(color.asWxColor());
    #else
        gc->SetFont(font, color.asWxColor());
    #endif
#else
    dc->SetFont(font);
    dc->SetTextForeground(color.asWxColor());
#endif
}

void DrawingContext::SetPen(wxPen &pen) {
#if wxUSE_GRAPHICS_CONTEXT
    gc->SetPen(pen);
#else
    dc->SetPen(pen);
#endif
}

wxGraphicsPath DrawingContext::CreatePath()
{
#if wxUSE_GRAPHICS_CONTEXT
    return gc->CreatePath();
#else
    #error Graphics Paths require wxUSE_GRAPHICS_CONTEXT
#endif
}

void DrawingContext::StrokePath(wxGraphicsPath& path)
{
#if wxUSE_GRAPHICS_CONTEXT
    gc->StrokePath(path);
#else
    #error Graphics Paths require wxUSE_GRAPHICS_CONTEXT
#endif
}

inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }

void DrawingContext::DrawText(const wxString &msg, int x, int y, double rotation) {
#if wxUSE_GRAPHICS_CONTEXT
    #ifdef __WXMSW__
        dc->DrawRotatedText(msg, x, y, rotation);
    #else
        gc->DrawText(msg, x, y, DegToRad(rotation));
    #endif
#else
    dc->DrawRotatedText(msg, x, y, rotation);
#endif
}
void DrawingContext::DrawText(const wxString &msg, int x, int y) {
#if wxUSE_GRAPHICS_CONTEXT
    #ifdef __WXMSW__
        dc->DrawText(msg, x, y);
    #else
        gc->DrawText(msg, x, y);
    #endif
#else
    dc->DrawText(msg, x, y);
#endif
}

void DrawingContext::GetTextExtent(const wxString &msg, double *width, double *height) {
#if wxUSE_GRAPHICS_CONTEXT
    #ifdef __WXMSW__
        wxSize size = dc->GetTextExtent(msg);
        *width = size.GetWidth();
        *height = size.GetHeight();
    #else
        gc->GetTextExtent(msg, width, height);
    #endif
#else
    wxSize size = dc->GetTextExtent(msg);
    *width = size.GetWidth();
    *height = size.GetHeight();
#endif
}

RenderBuffer::RenderBuffer()
{
    frameTimeInMs = 50;
    drawingContext = NULL;
    InhibitClear = false;
}

RenderBuffer::~RenderBuffer()
{
    //dtor
    if (drawingContext != NULL) {
        delete drawingContext;
    }
    for (std::map<int, EffectRenderCache*>::iterator i = infoCache.begin(); i != infoCache.end(); i++) {
        delete i->second;
    }
}

void RenderBuffer::InitBuffer(int newBufferHt, int newBufferWi)
{
    BufferHt=newBufferHt;
    BufferWi=newBufferWi;
    drawingContext = new DrawingContext(newBufferWi, newBufferHt);
    int NumPixels=BufferHt * BufferWi;
    pixels.resize(NumPixels);
    tempbuf.resize(NumPixels);
}

void RenderBuffer::Clear(const xlColour& bgColor)
{
    if (InhibitClear) { InhibitClear = false; return; } //allow canvas to be persistent for piano fx (self-reseting for safety) -DJ
    for(size_t i=0; i<pixels.size(); i++)
    {
        pixels[i]=bgColor;
    }
}

void RenderBuffer::SetPalette(xlColourVector& newcolors)
{
    palette.Set(newcolors);
}

size_t RenderBuffer::GetColorCount()
{
    return palette.Size();
}

// return a random number between 0 and 1 inclusive
double RenderBuffer::rand01()
{
    return (double)rand()/(double)RAND_MAX;
}

// generates a random number between num1 and num2 inclusive
double RenderBuffer::RandomRange(double num1, double num2)
{
    double hi,lo;
    if (num1 < num2)
    {
        lo = num1;
        hi = num2;
    }
    else
    {
        lo = num2;
        hi = num1;
    }
    return rand01()*(hi-lo)+ lo;
}

void RenderBuffer::Color2HSV(const xlColour& color, wxImage::HSVValue& hsv)
{
    wxImage::RGBValue rgb(color.Red(),color.Green(),color.Blue());
    hsv=wxImage::RGBtoHSV(rgb);
}

// sets newcolor to a random color between hsv1 and hsv2
void RenderBuffer::SetRangeColor(const wxImage::HSVValue& hsv1, const wxImage::HSVValue& hsv2, wxImage::HSVValue& newhsv)
{
    newhsv.hue=RandomRange(hsv1.hue,hsv2.hue);
    newhsv.saturation=RandomRange(hsv1.saturation,hsv2.saturation);
    newhsv.value=1.0;
}

// return a value between c1 and c2
wxByte RenderBuffer::ChannelBlend(wxByte c1, wxByte c2, double ratio)
{
    return c1 + floor(ratio*(c2-c1)+0.5);
}

void RenderBuffer::Get2ColorBlend(int coloridx1, int coloridx2, double ratio, xlColour &color)
{
    xlColour c1,c2;
    palette.GetColor(coloridx1,c1);
    palette.GetColor(coloridx2,c2);
    color.Set(ChannelBlend(c1.Red(),c2.Red(),ratio), ChannelBlend(c1.Green(),c2.Green(),ratio), ChannelBlend(c1.Blue(),c2.Blue(),ratio));
}

void RenderBuffer::Get2ColorAlphaBlend(const xlColour& c1, const xlColour& c2, double ratio, xlColour &color)
{
    color.Set(ChannelBlend(c1.Red(),c2.Red(),ratio), ChannelBlend(c1.Green(),c2.Green(),ratio), ChannelBlend(c1.Blue(),c2.Blue(),ratio));
}

HSVValue RenderBuffer::Get2ColorAdditive(HSVValue& hsv1, HSVValue& hsv2)
{
    wxImage::RGBValue rgb;
    wxImage::RGBValue rgb1 = wxImage::HSVtoRGB(hsv1);
    wxImage::RGBValue rgb2 = wxImage::HSVtoRGB(hsv2);
    rgb.red = rgb1.red + rgb2.red;
    rgb.green = rgb1.green + rgb2.green;
    rgb.blue = rgb1.blue + rgb2.blue;
    return wxImage::RGBtoHSV(rgb);
}
// 0 <= n < 1
void RenderBuffer::GetMultiColorBlend(double n, bool circular, xlColour &color)
{
    size_t colorcnt=GetColorCount();
    if (colorcnt <= 1)
    {
        palette.GetColor(0,color);
        return;
    }
    if (n >= 1.0) n=0.99999;
    if (n < 0.0) n=0.0;
    double realidx=circular ? n*colorcnt : n*(colorcnt-1);
    int coloridx1=floor(realidx);
    int coloridx2=(coloridx1+1) % colorcnt;
    double ratio=realidx-double(coloridx1);
    Get2ColorBlend(coloridx1,coloridx2,ratio,color);
}


// 0,0 is lower left
void RenderBuffer::SetPixel(int x, int y, const xlColour &color, bool wrap)
{
    if (wrap) {
        while (x < 0) {
            x += BufferWi;
        }
        while (y < 0) {
            y += BufferHt;
        }
        while (x > BufferWi) {
            x -= BufferWi;
        }
        while (y > BufferHt) {
            y -= BufferHt;
        }
    }
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        pixels[y*BufferWi+x] = color;
    }
}

void RenderBuffer::ProcessPixel(int x_pos, int y_pos, const xlColour &color, bool wrap_x, int width)
{
    int x_value = x_pos;
    if( wrap_x )  // if set wrap image at boundary
    {
        x_value %= width;
        x_value = (x_value >= 0) ? (x_value) : (width + x_value);
    }
    SetPixel(x_value,y_pos,color);
}

// 0,0 is lower left
void RenderBuffer::SetPixel(int x, int y, const wxImage::HSVValue& hsv, bool wrap)
{
    if (wrap) {
        while (x < 0) {
            x += BufferWi;
        }
        while (y < 0) {
            y += BufferHt;
        }
        while (x > BufferWi) {
            x -= BufferWi;
        }
        while (y > BufferHt) {
            y -= BufferHt;
        }
    }
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        pixels[y*BufferWi+x] = hsv;
    }
}

//copy src to dest: -DJ
void RenderBuffer::CopyPixel(int srcx, int srcy, int destx, int desty)
{
    if ((srcx >= 0) && (srcx < BufferWi) && (srcy >= 0) && (srcy < BufferHt))
        if ((destx >= 0) && (destx < BufferWi) && (desty >= 0) && (desty < BufferHt))
            pixels[desty * BufferWi + destx] = pixels[srcy * BufferWi + srcx];
}

void RenderBuffer::DrawHLine(int y, int xstart, int xend, const xlColor &color, bool wrap) {
    if (xstart > xend) {
        int i = xstart;
        xstart = xend;
        xend = i;
    }
    for (int x = xstart; x <= xend; x++) {
        SetPixel(x, y, color, wrap);
    }
}
void RenderBuffer::DrawVLine(int x, int ystart, int yend, const xlColor &color, bool wrap) {
    if (ystart > yend) {
        int i = ystart;
        ystart = yend;
        yend = i;
    }
    for (int y = ystart; y <= yend; y++) {
        SetPixel(x, y, color, wrap);
    }
}
void RenderBuffer::DrawBox(int x1, int y1, int x2, int y2, const xlColor& color, bool wrap) {
    if (y1 > y2) {
        int i = y1;
        y1 = y2;
        y2 = i;
    }
    if (x1 > x2) {
        int i = x1;
        x1 = x2;
        x2 = i;
    }
    for (int x = x1; x <= x2; x++) {
        for (int y = y1; y <= y2; y++) {
            SetPixel(x, y, color, wrap);
        }
    }
}

// Bresenham's line algorithm
void RenderBuffer::DrawLine( const int x0_, const int y0_, const int x1_, const int y1_, const xlColor& color )
{
    int x0 = x0_;
    int x1 = x1_;
    int y0 = y0_;
    int y1 = y1_;

    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    SetPixel(x0,y0, color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void RenderBuffer::DrawThickLine( const int x0_, const int y0_, const int x1_, const int y1_, const xlColor& color, bool direction )
{
    int x0 = x0_;
    int x1 = x1_;
    int y0 = y0_;
    int y1 = y1_;
    int lastx = x0;
    int lasty = y0;

    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    SetPixel(x0,y0, color);
    if( (x0 != lastx) && (y0 != lasty) && (x0_ != x1_) && (y0_ != y1_) )
    {
        int fix = 0;
        if( x0 > lastx ) fix += 1;
        if( y0 > lasty ) fix += 2;
        if( direction  ) fix += 4;
        switch (fix)
        {
        case 2:
        case 4:
            if( x0 < BufferWi -2 ) SetPixel(x0+1,y0, color);
            break;
        case 3:
        case 5:
            if( x0 > 0 ) SetPixel(x0-1,y0, color);
            break;
        case 0:
        case 1:
            if( y0 < BufferHt -2 )SetPixel(x0,y0+1, color);
            break;
        case 6:
        case 7:
            if( y0 > 0 )SetPixel(x0,y0-1, color);
            break;
        }
    }
    lastx = x0;
    lasty = y0;
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void RenderBuffer::DrawFadingCircle(int x0, int y0, int radius, const xlColor& rgb, bool wrap)
{
    HSVValue hsv = wxImage::RGBtoHSV(rgb);
    xlColor color(rgb);
    int r = radius;
    if (allowAlpha) {
        while(r >= 0)
        {
            color.alpha = (double)rgb.alpha * (1.0 - (double)(r) / (double)radius);
            DrawCircle(x0, y0, r, color, wrap);
            r--;
        }
    } else {
        double full_brightness = hsv.value;
        while(r >= 0)
        {
            hsv.value = full_brightness * (1.0 - (double)(r) / (double)radius);
            if( hsv.value > 0.0 )
            {
                color = wxImage::HSVtoRGB(hsv);
                DrawCircle(x0, y0, r, color, wrap);
            }
            r--;
        }
    }
}

void RenderBuffer::DrawCircle(int x0, int y0, int radius, const xlColor& rgb, bool filled, bool wrap)
{
    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    while(x >= y) {
        if (!filled) {
            SetPixel(x + x0, y + y0, rgb, wrap);
            SetPixel(y + x0, x + y0, rgb, wrap);
            SetPixel(-x + x0, y + y0, rgb, wrap);
            SetPixel(-y + x0, x + y0, rgb, wrap);
            SetPixel(-x + x0, -y + y0, rgb, wrap);
            SetPixel(-y + x0, -x + y0, rgb, wrap);
            SetPixel(x + x0, -y + y0, rgb, wrap);
            SetPixel(y + x0, -x + y0, rgb, wrap);
        } else {
            DrawVLine(x0 - x, y0 - y, y0 + y, rgb, wrap);
            DrawVLine(x0 + x, y0 - y, y0 + y, rgb, wrap);
            DrawVLine(x0 - y, y0 - x, y0 + x, rgb, wrap);
            DrawVLine(x0 + y, y0 - x, y0 + x, rgb, wrap);
        }
        y++;
        if (radiusError<0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}


// 0,0 is lower left
void RenderBuffer::GetPixel(int x, int y, xlColour &color)
{
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        color=pixels[y*BufferWi+x];
    }
}


// 0,0 is lower left
void RenderBuffer::SetTempPixel(int x, int y, const xlColour &color)
{
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        tempbuf[y*BufferWi+x]=color;
    }
}

void RenderBuffer::SetTempPixel(int x, int y, const xlColour & color, int alpha)
{
    xlColor c(color.Red(), color.Green(), color.Blue(), alpha);

    SetTempPixel(x, y, c);
}

// 0,0 is lower left
void RenderBuffer::GetTempPixel(int x, int y, xlColour &color)
{
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        color=tempbuf[y*BufferWi+x];
    }
}
const xlColor &RenderBuffer::GetTempPixel(int x, int y) {
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        return tempbuf[y*BufferWi+x];
    }
    return xlBLACK;
}


const xlColor& RenderBuffer::GetTempPixelRGB(int x, int y)
{
    if (x >= 0 && x < BufferWi && y >= 0 && y < BufferHt)
    {
        return tempbuf[y*BufferWi+x];
    }
    return xlBLACK;
}

void RenderBuffer::SetState(int period, bool ResetState, const wxString& model_name)
{
    if (ResetState)
    {
        needToInit = true;
    }
    curPeriod = period;
    cur_model = model_name;
    curPeriod = period;
}
void RenderBuffer::ClearTempBuf()
{
    for (size_t i=0; i < tempbuf.size(); i++)
    {
        tempbuf[i].Set(0, 0, 0, 0);
    }
}
double RenderBuffer::GetEffectTimeIntervalPosition(float cycles) {
    if (curEffEndPer == curEffStartPer) {
        return 0.0;
    }
    double retval = (double)(curPeriod-curEffStartPer)/(double)(curEffEndPer-curEffStartPer);
    retval *= cycles;
    while (retval > 1.0) {
        retval -= 1.0;
    }
    return retval;
}
double RenderBuffer::GetEffectTimeIntervalPosition()
{
    if (curEffEndPer == curEffStartPer) {
        return 0.0;
    }
    double retval = (double)(curPeriod-curEffStartPer)/(double)(curEffEndPer-curEffStartPer);
//    debug(10, "GetEffTiIntPos(fr last? %d): (cur %d - curst %d)/(curend %d - curst) = %f, (cur - curst)/(next %d - curst) = %f, (cur - prev %d)/(curend - prev) = %f", from_last, curPeriod, curEffStartPer, curEffEndPer, GetEffectPeriodPosition(), nextEffTimePeriod, (double)(curPeriod-curEffStartPer)/(nextEffTimePeriod-curEffStartPer), prevNonBlankStartPeriod, (double)(curPeriod - prevNonBlankStartPeriod) / (curEffEndPer - prevNonBlankStartPeriod));
    return retval;
}

void RenderBuffer::SetFadeTimes(float fadeInDuration, float fadeOutDuration )
{
    fadeinsteps = (int)(fadeInDuration*1000)/frameTimeInMs;
    fadeoutsteps = (int)(fadeOutDuration*1000)/frameTimeInMs;
}
void RenderBuffer::GetFadeSteps( int& fadeInSteps, int& fadeOutSteps)
{
    fadeInSteps = fadeinsteps;
    fadeOutSteps = fadeoutsteps;
}

void RenderBuffer::SetEffectDuration(int startMsec, int endMsec)
{
    curEffStartPer = startMsec / frameTimeInMs;
    curEffEndPer = (endMsec - 1) / frameTimeInMs;
}

void RenderBuffer::GetEffectPeriods( int& start, int& endp)
{
    start = curEffStartPer;
    endp = curEffEndPer;
}

void RenderBuffer::SetDisplayListHRect(Effect *eff, int idx, double x1, double y1, double x2, double y2,
                                     const xlColor &c1, const xlColor &c2) {
    SetDisplayListRect(eff, idx, x1, y1, x2, y2, c1, c1, c2, c2);
}
void RenderBuffer::SetDisplayListVRect(Effect *eff, int idx, double x1, double y1, double x2, double y2,
                                     const xlColor &c1, const xlColor &c2) {
    SetDisplayListRect(eff, idx, x1, y1, x2, y2, c1, c2, c1, c2);
}
void RenderBuffer::SetDisplayListRect(Effect *eff, int idx, double x1, double y1, double x2, double y2,
                                    const xlColor &cx1y1, const xlColor &cx1y2,
                                    const xlColor &cx2y1, const xlColor &cx2y2) {
    eff->GetBackgroundDisplayList()[idx].color = cx1y1;
    eff->GetBackgroundDisplayList()[idx+1].color = cx1y2;
    eff->GetBackgroundDisplayList()[idx+2].color = cx2y2;
    eff->GetBackgroundDisplayList()[idx+3].color = cx2y1;
    eff->GetBackgroundDisplayList()[idx].x = x1;
    eff->GetBackgroundDisplayList()[idx+1].x = x1;
    eff->GetBackgroundDisplayList()[idx+2].x = x2;
    eff->GetBackgroundDisplayList()[idx+3].x = x2;

    eff->GetBackgroundDisplayList()[idx].y = y1;
    eff->GetBackgroundDisplayList()[idx+1].y = y2;
    eff->GetBackgroundDisplayList()[idx+2].y = y2;
    eff->GetBackgroundDisplayList()[idx+3].y = y1;

    eff->GetBackgroundDisplayList()[idx].usage = GL_QUADS;
    eff->GetBackgroundDisplayList()[idx+1].usage = GL_QUADS;
    eff->GetBackgroundDisplayList()[idx+2].usage = GL_QUADS;
    eff->GetBackgroundDisplayList()[idx+3].usage = GL_QUADS;
}
void RenderBuffer::CopyPixelsToDisplayListX(Effect *eff, int row, int sx, int ex, int inc) {
    wxMutexLocker lock(eff->GetBackgroundDisplayList().lock);
    int count = curEffEndPer - curEffStartPer + 1;


    int total = curEffEndPer - curEffStartPer + 1;
    double x = double(curPeriod - curEffStartPer) / double(total);
    double x2 = (curPeriod - curEffStartPer + 1.0) / double(total);
    xlColor c;

    int cur = 0;
    for (int p = sx; p <= ex; p += inc) {
        double y = double(p - sx) / double(ex - sx + 1.0);
        double y2 = double(p - sx + 1.0) / double(ex - sx + 1.0);
        GetPixel(p, row, c);

        int idx = cur * count + (curPeriod - curEffStartPer);
        cur++;
        SetDisplayListHRect(eff, idx*4, x, y, x2, y2, c, c);
    }
}

double RenderBuffer::calcAccel(double ratio, double accel)
{
    if( accel == 0 ) return ratio;

    double pct_accel = (std::abs(accel) - 1.0) / 9.0;
    double new_accel1 = pct_accel * 5 + (1.0 - pct_accel) * 1.5;
    double new_accel2 = 1.5 + (ratio * new_accel1);
    double final_accel = pct_accel * new_accel2 + (1.0 - pct_accel) * new_accel1;

    if( accel > 0 )
    {
        return std::pow(ratio, final_accel);
    }
    else
    {
        return (1.0 - std::pow(1.0 - ratio, new_accel1));
    }
}

double RenderBuffer::GetStepAngle(int width, int height)
{
    double step = 0.5;
    int biggest = std::max(width, height);
    if( biggest > 50 ) {
        step = 0.4;
    }
    if( biggest > 150 ) {
        step = 0.3;
    }
    if( biggest > 250 ) {
        step = 0.2;
    }
    if( biggest > 350 ) {
        step = 0.1;
    }
    return step;
}
