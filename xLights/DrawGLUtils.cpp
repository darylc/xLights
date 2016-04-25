

#include "wx/wx.h"

#ifdef __WXMAC__
 #include "OpenGL/glu.h"
 #include "OpenGL/gl.h"
 #include "GLUT/glut.h"
#else
	#ifdef _MSC_VER
		#include "GL/glut.h"
	#else
		#include <GL/glu.h>
		#include <GL/gl.h>
		#include <GL/glut.h>
	#endif
#endif

#include <wx/bitmap.h>
#include "DrawGLUtils.h"

const double PI  =3.141592653589793238463;


static DrawGLUtils::xlGLCacheInfo *currentCache;

class DrawGLUtils::xlGLCacheInfo {
public:
    xlGLCacheInfo() {};
    virtual ~xlGLCacheInfo() {};
    
    virtual void addVertex(double x, double y, const xlColor &c) = 0;
    virtual int vertexCount() = 0;
    virtual void flush(int type) = 0;
    virtual void ensureSize(int i) = 0;
};

class OpenGL21Cache : public DrawGLUtils::xlGLCacheInfo {
public:
    OpenGL21Cache() {
        max = 1024;
        colors = new unsigned char[max * 4];
        vertices = new double[max * 2];
        curCount = 0;
    };
    virtual ~OpenGL21Cache() {
        delete [] colors;
        delete [] vertices;
    };
    
    virtual void addVertex(double x, double y, const xlColor &c) {
        ensureSize(1);
        vertices[curCount * 2] = x;
        vertices[curCount * 2 + 1] = y;
        colors[curCount*4] = c.Red();
        colors[curCount*4 + 1] = c.Green();
        colors[curCount*4 + 2] = c.Blue();
        colors[curCount*4 + 3] = c.Alpha();
        curCount++;
    }
    virtual void flush(int type) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
        glVertexPointer(2, GL_DOUBLE, 0, vertices);
        glDrawArrays(type, 0, curCount);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        curCount = 0;
    }
    virtual void ensureSize(int s) {
        int size = curCount + s;
        if (size > max) {
            unsigned char *tmp = new unsigned char[size * 4];
            for (int x = 0; x < (max*4); x++) {
                tmp[x] = colors[x];
            }
            delete [] colors;
            colors = tmp;
            double *tmpf = new double[size * 2];
            for (int x = 0; x < (max*2); x++) {
                tmpf[x] = vertices[x];
            }
            delete [] vertices;
            vertices = tmpf;
            max = size;
        }
    }
    virtual int vertexCount() {
        return curCount;
    }
private:
    int max;
    unsigned char *colors;
    double *vertices;
    int curCount;
};













DrawGLUtils::xlGLCacheInfo *DrawGLUtils::CreateCache() {
    const GLubyte* str = glGetString(GL_VERSION);
    if (str[0] == '3') {
        
    }
    return new OpenGL21Cache();
}
void DrawGLUtils::SetCurrentCache(xlGLCacheInfo *c) {
    currentCache = c;
}
void DrawGLUtils::DestroyCache(xlGLCacheInfo *cache) {
    if (currentCache == cache) {
        currentCache = nullptr;
    }
    delete cache;
}





void DrawGLUtils::PreAlloc(int verts) {
    currentCache->ensureSize(verts);
}
int DrawGLUtils::VertexCount() {
    return currentCache->vertexCount();
}
void DrawGLUtils::AddVertex(double x, double y, const xlColor &c, int transparency) {
    xlColor color(c);
    if (transparency) {
        double t = 100.0 - transparency;
        t *= 2.55;
        transparency = t;
        color.alpha = transparency > 255 ? 255 : (transparency < 0 ? 0 : transparency);
    }
    currentCache->addVertex(x, y, color);
}

void DrawGLUtils::AddRectAsTriangles(double x1, double y1,
                                     double x2, double y2,
                                     const xlColor &c, int transparency) {
    xlColor color(c);
    if (transparency) {
        double t = 100.0 - transparency;
        t *= 2.55;
        transparency = t;
        color.alpha = transparency > 255 ? 255 : (transparency < 0 ? 0 : transparency);
    }
    PreAlloc(6);
    currentCache->addVertex(x1, y1, color);
    currentCache->addVertex(x1, y2, color);
    currentCache->addVertex(x2, y2, color);
    
    currentCache->addVertex(x2, y2, color);
    currentCache->addVertex(x2, y1, color);
    currentCache->addVertex(x1, y1, color);
}
void DrawGLUtils::AddRect(double x1, double y1,
                          double x2, double y2,
                          const xlColor &c, int transparency) {
    xlColor color(c);
    if (transparency) {
        double t = 100.0 - transparency;
        t *= 2.55;
        transparency = t;
        color.alpha = transparency > 255 ? 255 : (transparency < 0 ? 0 : transparency);
    }
    currentCache->ensureSize(4);
    currentCache->addVertex(x1, y1, color);
    currentCache->addVertex(x1, y2, color);
    currentCache->addVertex(x2, y2, color);
    currentCache->addVertex(x2, y1, color);
}

void DrawGLUtils::End(int type) {
    currentCache->flush(type);
}

void DrawGLUtils::DrawPoint(const xlColor &color, double x, double y)
{
    currentCache->addVertex(x, y, color);
    currentCache->flush(GL_POINTS);
}

void DrawGLUtils::DrawCircle(const xlColor &color, double cx, double cy, double r, int ctransparency, int etransparency)
{
    xlColor c(color);
    
    if (ctransparency) {
        double t = 100.0 - ctransparency;
        t *= 2.56;
        ctransparency = t;
        c.alpha = ctransparency;
    }
    
    int num_segments = r / 2;
    if (num_segments < 16) {
        num_segments = 16;
    }
    currentCache->ensureSize(num_segments * 2 + 4);
    float theta = 2 * 3.1415926 / float(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor
    
    float radial_factor = cosf(theta);//calculate the radial factor
    
    float x = r;//we start at angle = 0
    
    float y = 0;
    
    currentCache->addVertex(cx, cy, c);
    
    if (etransparency) {
        double t = 100.0 - etransparency;
        t *= 2.56;
        etransparency = t;
        c.alpha = etransparency;
    } else {
        c.alpha = 255;
    }
    
    for(int ii = 0; ii < num_segments; ii++)
    {
        currentCache->addVertex(x + cx, y + cy, c);
        
        //calculate the tangential vector
        //remember, the radial vector is (x, y)
        //to get the tangential vector we flip those coordinates and negate one of them
        
        float tx = -y;
        float ty = x;
        
        //add the tangential vector
        
        x += tx * tangetial_factor;
        y += ty * tangetial_factor;
        
        //correct using the radial factor
        
        x *= radial_factor;
        y *= radial_factor;
    }
    currentCache->addVertex(x + cx, y + cy, c);
    currentCache->flush(GL_TRIANGLE_FAN);
}

void DrawGLUtils::DrawCircleUnfilled(const xlColor &color, double cx, double cy, double r, float width )
{
    static const double inc = PI / 12;
    static const double max = 2 * PI;
    glLineWidth(width);
    for(double d = 0; d < max; d += inc) {
        currentCache->addVertex(cos(d) * r + cx, sin(d) * r + cy, color);
    }
    currentCache->flush(GL_LINE_LOOP);
}

void DrawGLUtils::DrawLine(const xlColor &color, wxByte alpha,int x1, int y1, int x2, int y2, float width)
{
    glLineWidth(width);
    currentCache->ensureSize(4);
    currentCache->addVertex(x1, y1, color);
    currentCache->addVertex(x2, y2, color);
    currentCache->flush(GL_LINES);
}

void DrawGLUtils::DrawRectangle(const xlColor &color, bool dashed, int x1, int y1, int x2, int y2)
{
    if (!dashed)
    {
        currentCache->ensureSize(16);
        currentCache->addVertex(x1, y1, color);
        currentCache->addVertex(x2, y1, color);
        
        currentCache->addVertex(x2, y1, color);
        currentCache->addVertex(x2, y2 - 0.4f, color);
        
        currentCache->addVertex(x2, y2 - 0.4f, color);
        currentCache->addVertex(x1, y2 - 0.4f, color);
        
        currentCache->addVertex(x1, y1, color);
        currentCache->addVertex(x1, y2, color);
        currentCache->flush(GL_LINES);
    }
    else
    {
        // Line 1
        int xs = x1<x2?x1:x2;
        int xf = x1>x2?x1:x2;
        for(int x=xs;x<=xf;x++)
        {
            if(x%8<4)
            {
                currentCache->addVertex(x, y1, color);
            }
        }
        // Line 2
        int ys = y1<y2?y1:y2;
        int yf = y1>y2?y1:y2;
        for(int y=ys;y<=yf;y++)
        {
            if(y%8<4)
            {
                currentCache->addVertex(x2, y, color);
            }
        }
        // Line 3
        xs = x1<x2?x1:x2;
        xf = x1>x2?x1:x2;
        for(int x=xs;x<=xf;x++)
        {
            if(x%8<4)
            {
                currentCache->addVertex(x, y2, color);
            }
        }
        // Line 4
        ys = y1<y2?y1:y2;
        yf = y1>y2?y1:y2;
        for(int y=ys;y<=yf;y++)
        {
            if(y%8<4)
            {
                currentCache->addVertex(x1, y, color);
            }
        }
        currentCache->flush(GL_POINTS);
    }
}

void DrawGLUtils::DrawFillRectangle(const xlColor &color, wxByte alpha, int x, int y,int width, int height)
{
    xlColor c(color);
    c.alpha = alpha;
    AddRectAsTriangles(x, y, x + width, y + height, c);
    currentCache->flush(GL_TRIANGLES);
}

void DrawGLUtils::DrawRectangleArray(double y1, double y2, double x, std::vector<double> &xs, std::vector<xlColor> & colors, bool flush) {
    // each rect is 6 vertices (using GL_TRIANGLES) of x/y
    currentCache->ensureSize(colors.size() * 6);
    for (int n = 0; n < xs.size(); n++) {
        int x2 = xs[n];
        currentCache->addVertex(x, y1, colors[n]);
        currentCache->addVertex(x, y2, colors[n]);
        currentCache->addVertex(x2, y2, colors[n]);
        
        currentCache->addVertex(x, y1, colors[n]);
        currentCache->addVertex(x2, y2, colors[n]);
        currentCache->addVertex(x2, y1, colors[n]);
        x = x2;
    }
    if (flush) {
        currentCache->flush(GL_TRIANGLES);
    }
}


void DrawGLUtils::DrawHBlendedRectangle(const xlColor &left, const xlColor &right, int x1, int y1,int x2, int y2, bool flush) {
    currentCache->addVertex(x1, y1, left);
    currentCache->addVertex(x1, y2, left);
    currentCache->addVertex(x2, y2, right);
    
    currentCache->addVertex(x2, y2, right);
    currentCache->addVertex(x2, y1, right);
    currentCache->addVertex(x1, y1, left);
    if (flush) {
        currentCache->flush(GL_TRIANGLES);
    }
}
void DrawGLUtils::DrawHBlendedRectangle(const xlColorVector &colors, int x1, int y1,int x2, int y2, int offset) {
    xlColor start;
    xlColor end;
    int cnt = colors.size();
    if (cnt == 0) {
        return;
    }
    start = colors[0+offset];
    if (cnt == 1) {
        DrawGLUtils::DrawHBlendedRectangle(start, start, x1, y1, x2, y2);
        return;
    }
    int xl = x1;
    start = colors[0+offset];
    for (int x = 1+offset; x < cnt; x++) {
        end =  colors[x];
        int xr = x1 + (x2 - x1) * x / (cnt  - 1);
        if (x == (cnt - 1)) {
            xr = x2;
        }
        DrawGLUtils::DrawHBlendedRectangle(start, end, xl, y1, xr, y2, false);
        start = end;
        xl = xr;
    }
    currentCache->flush(GL_TRIANGLES);
}


void DrawGLUtils::DrawDisplayList(double xOffset, double yOffset,
                                  double width, double height,
                                  const DrawGLUtils::xlDisplayList & dl) {
    std::lock_guard<std::recursive_mutex> lock(dl.lock);
    if (dl.empty()) {
        return;
    }
    int lastUsage = dl[0].usage;
    for (int idx = 0; idx < dl.size(); idx++) {
        const DisplayListItem &item = dl[idx];
        if (item.valid) {
            if (item.usage != lastUsage) {
                currentCache->flush(lastUsage);
            }
            lastUsage = item.usage;
            currentCache->addVertex(xOffset + item.x * width, yOffset + item.y * height, item.color);
        }
    }
    currentCache->flush(lastUsage);
}










/*** -----------  Yet to be updated for OpenGL 3.1 ----------------- ****/


static void addMipMap(GLuint* texture, const wxImage &l_Image, int &level) {
    if (l_Image.IsOk() == true)
    {
        glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, (GLsizei)l_Image.GetWidth(), (GLsizei)l_Image.GetHeight(),
                     0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)l_Image.GetData());
        int err = glGetError();
        if (err == GL_NO_ERROR) {
            level++;
        }
    }
}


void DrawGLUtils::CreateOrUpdateTexture(const wxBitmap &bmp48,
                                        const wxBitmap &bmp32,
                                        const wxBitmap &bmp16,
                                        GLuint* texture)
{
    int level = 0;
    glGenTextures(1,texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    addMipMap(texture, bmp48.ConvertToImage().Rescale(64, 64, wxIMAGE_QUALITY_HIGH), level);
    addMipMap(texture, bmp32.ConvertToImage(), level);
    addMipMap(texture, bmp16.ConvertToImage(), level);
    addMipMap(texture, bmp16.ConvertToImage().Rescale(8, 8, wxIMAGE_QUALITY_HIGH), level);
    addMipMap(texture, bmp16.ConvertToImage().Rescale(4, 4, wxIMAGE_QUALITY_HIGH), level);
    addMipMap(texture, bmp16.ConvertToImage().Rescale(2, 2, wxIMAGE_QUALITY_HIGH), level);
    addMipMap(texture, bmp16.ConvertToImage().Rescale(1, 1, wxIMAGE_QUALITY_HIGH), level);
}

void DrawGLUtils::DrawTexture(GLuint* texture,double x, double y, double x2, double y2)
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D,*texture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    
    glVertex2f(x-0.4, y);
    
    glTexCoord2f(1,0);
    glVertex2f(x2-0.4,y);
    
    glTexCoord2f(1,1);
    glVertex2f(x2-0.4,y2);
    
    glTexCoord2f(0,1);
    glVertex2f(x-0.4,y2);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void DrawGLUtils::UpdateTexturePixel(GLuint* texture,double x, double y, xlColor& color, bool hasAlpha)
{
    int bytesPerPixel = hasAlpha ?  4 : 3;
    GLubyte *imageData=new GLubyte[bytesPerPixel];
    imageData[0] = color.red;
    imageData[1] = color.green;
    imageData[2] = color.blue;
    if( hasAlpha ) {
        imageData[3] = color.alpha;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,*texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, hasAlpha ?  GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
    delete [] imageData;
    glDisable(GL_TEXTURE_2D);
}

void DrawGLUtils::DrawText(double x, double y, double size, const wxString &text, double factor) {
    int tsize = size * factor;
    void *font = nullptr;
    switch (tsize) {
    case 10:
        font = GLUT_BITMAP_HELVETICA_10;
        break;
    case 12:
        font = GLUT_BITMAP_HELVETICA_12;
        break;
    case 18:
        font = GLUT_BITMAP_HELVETICA_18;
        break;
    }
    if (font) {
        DrawText(x, y, font, text);
    } else {
        DrawStrokedText(x, y, size, text);
    }
}
int DrawGLUtils::GetTextWidth(double size, const wxString &text, double factor) {
    int tsize = size * factor;
    void *font = nullptr;
    switch (tsize) {
        case 10:
            font = GLUT_BITMAP_HELVETICA_10;
            break;
        case 12:
            font = GLUT_BITMAP_HELVETICA_12;
            break;
        case 18:
            font = GLUT_BITMAP_HELVETICA_18;
            break;
    }
    if (font) {
        return GetTextWidth(font, text);
    }
    return GetStrokedTextWidth(size, text);
}

void DrawGLUtils::DrawStrokedText(double x, double y, float size, const wxString &text, double factor) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(factor * 0.9);
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(x, y, 0);
    glScalef(size / 150.0, - size / 150.0, 1.0);
    for (int x = 0; x  < text.length(); x++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[x]);
    }
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}

int DrawGLUtils::GetStrokedTextWidth(float size, const wxString &text) {
    int ret = glutStrokeLength(GLUT_STROKE_ROMAN, text.c_str()) * size / 150;
    return ret;
}


void DrawGLUtils::DrawText(double x, double y, void *glutBitmapFont, const wxString &text) {
    glPushMatrix();
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int x = 0; x  < text.length(); x++) {
        glutBitmapCharacter(glutBitmapFont, text[x]);
    }
    glPopMatrix();
}

int DrawGLUtils::GetTextWidth(void *glutBitmapFont, const wxString &text)
{
    int length = 0;
    for (int x = 0; x  < text.length(); x++) {
        length += glutBitmapWidth(glutBitmapFont, text[x]);
    }
    return length;
}


