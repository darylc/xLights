
#ifndef __XL_DRAWGLUTILS
#define __XL_DRAWGLUTILS

#include <mutex>
#include "wx/glcanvas.h"
#include "Color.h"

class xlGLCanvas;

namespace DrawGLUtils
{
    class xlGLCacheInfo {
    public:
        xlGLCacheInfo() {};
        virtual ~xlGLCacheInfo() {};
        
        virtual void addVertex(double x, double y, const xlColor &c) = 0;
        virtual int vertexCount() = 0;
        virtual void flush(int type, int enableCapability = 0) = 0;
        virtual void ensureSize(int i) = 0;
        
        virtual void Ortho(int topleft_x, int topleft_y, int bottomright_x, int bottomright_y) = 0;
        virtual void PushMatrix() = 0;
        virtual void PopMatrix() = 0;
        virtual void Translate(float x, float y, float z) = 0;
        virtual void Rotate(float angle, float x, float y, float z) = 0;
        virtual void Scale(float w, float h, float z) = 0;
        virtual void DrawTexture(GLuint* texture,
                                 float x, float y, float x2, float y2,
                                 float tx = 0.0, float ty = 0.0, float tx2 = 1.0, float ty2 = 1.0) = 0;

    };
    
    xlGLCacheInfo *CreateCache();
    void SetCurrentCache(xlGLCacheInfo *cache);
    void DestroyCache(xlGLCacheInfo *cache);
    
    void SetViewport(xlGLCanvas &win, int x1, int y1, int x2, int y2);
    void PushMatrix();
    void PopMatrix();
    void Translate(float x, float y, float z);
    void Rotate(float angle, float x, float y, float z);
    void Scale(float w, float h, float z);
    
    
    class DisplayListItem {
    public:
        DisplayListItem() : valid(true), usage(GL_POINT), x(0.0), y(0.0) {};
        bool valid;
        int usage;  //POINT, TRIANGLE, QUAD, ETC...
        xlColor color;
        double x, y;
    };
    class xlDisplayList : public std::vector<DisplayListItem> {
    public:
        xlDisplayList() : iconSize(2) {};
        int iconSize;
        mutable std::recursive_mutex lock;
        
        void LockedClear() {
            std::unique_lock<std::recursive_mutex> locker(lock);
            clear();
        }
    };
   
    void DrawText(double x, double y, double size, const wxString &text, double factor = 1.0);
    int GetTextWidth(double size, const wxString &text, double factor = 1.0);
    
    
    void DrawPoint(const xlColor &color, double x, double y);

    void DrawCircle(const xlColor &color, double x, double y, double r, int ctransparency = 0, int etransparency = 0);
    void DrawCircleUnfilled(const xlColor &color, double cx, double cy, double r, float width);

    /* Methods to hold vertex informaton (x, y, color) in an array until End is called where they are all
       draw out to the context in very few calls) */
    void AddVertex(double x, double y, const xlColor &c, int transparency = 0);
    int VertexCount();
    /* Add four vertices to the cache list, all with the given color */
    void PreAlloc(int verts);
    void AddRect(double x1, double y1,
                 double x2, double y2,
                 const xlColor &c, int transparency = 0);
    void AddRectAsTriangles(double x1, double y1,
                            double x2, double y2,
                            const xlColor &c, int transparency = 0);
    void End(int type, int enableCapability = 0);

    void DrawLine(const xlColor &color, wxByte alpha,int x1, int y1,int x2, int y2,float width);
    void DrawRectangle(const xlColor &color, bool dashed, int x1, int y1,int x2, int y2);
    void DrawFillRectangle(const xlColor &color, wxByte alpha, int x, int y,int width, int height);

    void DrawHBlendedRectangle(const xlColor &lcolor, const xlColor &rcolor, int x, int y, int x2, int y2, bool flush = true);
    void DrawHBlendedRectangle(const xlColorVector &colors, int x, int y, int x2, int y2, int offset = 0);
    void CreateOrUpdateTexture(const wxBitmap &bmp48,    //will scale to 64x64 for base
                               const wxBitmap &bmp32,
                               const wxBitmap &bmp16,
                               GLuint* texture);
    void DrawTexture(GLuint* texture,
                     float x, float y, float x2, float y2,
                     float tx = 0.0, float ty = 0.0, float tx2 = 1.0, float ty2 = 1.0);
    
    void UpdateTexturePixel(GLuint* texture,double x, double y, xlColor& color, bool hasAlpha);

    void DrawRectangleArray(double y1, double y2, double x, std::vector<double> &xs, std::vector<xlColor> & colors, bool flush = true);

    void DrawDisplayList(double xOffset, double yOffset,
                         double width, double height,
                         const xlDisplayList & dl);
}

#endif
