#include "wx/wx.h"
#include "wx/sizer.h"
#ifdef __WXMAC__
 #include "OpenGL/glu.h"
 #include "OpenGL/gl.h"
#else
#ifdef _MSC_VER
#include "GL/glut.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#endif

#include "ModelPreview.h"
#include "models/Model.h"

#include "DrawGLUtils.h"


BEGIN_EVENT_TABLE(ModelPreview, xlGLCanvas)
EVT_MOTION(ModelPreview::mouseMoved)
EVT_LEFT_DOWN(ModelPreview::mouseLeftDown)
EVT_LEFT_UP(ModelPreview::mouseLeftUp)
EVT_LEAVE_WINDOW(ModelPreview::mouseLeftWindow)
EVT_RIGHT_DOWN(ModelPreview::rightClick)
//EVT_KEY_DOWN(ModelPreview::keyPressed)
//EVT_KEY_UP(ModelPreview::keyReleased)
//EVT_MOUSEWHEEL(ModelPreview::mouseWheelMoved)
EVT_PAINT(ModelPreview::render)
END_EVENT_TABLE()

void ModelPreview::mouseMoved(wxMouseEvent& event) {
    event.ResumePropagation(1);
    event.Skip (); // continue the event
}

void ModelPreview::mouseLeftDown(wxMouseEvent& event) {
    event.ResumePropagation(1);
    event.Skip (); // continue the event
}

void ModelPreview::rightClick(wxMouseEvent& event) {
    event.ResumePropagation(1);
    event.Skip (); // continue the event
}

void ModelPreview::mouseLeftUp(wxMouseEvent& event) {
    event.ResumePropagation(1);
    event.Skip (); // continue the event
}

void ModelPreview::mouseLeftWindow(wxMouseEvent& event) {
    event.ResumePropagation(1);
    event.Skip (); // continue the event
}

void ModelPreview::render( wxPaintEvent& event )
{
    if(mIsDrawing) return;
    //if(!mIsInitialized) { InitializeGLCanvas(); }
    //SetCurrentGLContext();
    //wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(!StartDrawing(mPointSize)) return;
    Render();
    EndDrawing();
}

void ModelPreview::Render() {
    if (PreviewModels != NULL) {
        for (int i=0; i<PreviewModels->size(); i++) {
			const xlColor *color = &xlLIGHT_GREY;
			if (((*PreviewModels)[i])->Selected) {
				color = &xlYELLOW;
			} else if (((*PreviewModels)[i])->GroupSelected) {
				color = &xlYELLOW;
			} else if (((*PreviewModels)[i])->Overlapping) {
				color = &xlRED;
			}
            if (!allowSelected) {
                color = &xlLIGHT_GREY;
            }
            (*PreviewModels)[i]->DisplayModelOnWindow(this, color, allowSelected);
        }
    }
}
void ModelPreview::Render(const unsigned char *data) {
    if (StartDrawing(mPointSize)) {
        if (PreviewModels != NULL) {
            for (int m=0; m<PreviewModels->size(); m++) {
                int NodeCnt=(*PreviewModels)[m]->GetNodeCount();
                for(int n=0; n<NodeCnt; n++) {
                    int start = (*PreviewModels)[m]->NodeStartChannel(n);
                    (*PreviewModels)[m]->SetNodeChannelValues(n, &data[start]);
                }
                (*PreviewModels)[m]->DisplayModelOnWindow(this);
            }
        }
        EndDrawing();
    }
}



void ModelPreview::mouseWheelMoved(wxMouseEvent& event) {}
//void ModelPreview::rightClick(wxMouseEvent& event) {}
void ModelPreview::keyPressed(wxKeyEvent& event) {}
void ModelPreview::keyReleased(wxKeyEvent& event) {}

ModelPreview::ModelPreview(wxPanel* parent, std::vector<Model*> &models, bool a, int styles)
    : xlGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, styles, "", true), PreviewModels(&models), allowSelected(a)
{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    virtualWidth = 0;
    virtualHeight = 0;
}
ModelPreview::ModelPreview(wxPanel* parent)
: xlGLCanvas(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, "", true), PreviewModels(NULL), allowSelected(false)
{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    virtualWidth = 0;
    virtualHeight = 0;
}
ModelPreview::~ModelPreview()
{
}

void ModelPreview::SetCanvasSize(int width,int height)
{
    SetVirtualCanvasSize(width, height);
}
void ModelPreview::SetVirtualCanvasSize(int width, int height) {
    virtualWidth = width;
    virtualHeight = height;
}
void ModelPreview::InitializePreview(wxString img,int brightness)
{
    image = NULL;
    mBackgroundImage = img;
    mBackgroundImageExists = wxFileExists(mBackgroundImage)?true:false;
    mBackgroundBrightness = brightness;
}

void ModelPreview::InitializeGLCanvas()
{
    if(!IsShownOnScreen()) return;
    SetCurrentGLContext();

    if (allowSelected) {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Black Background
    } else {
        glClearColor(0.0, 0.0, 0.0, 1.0f); // Black Background
    }
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    mIsInitialized = true;
}

void ModelPreview::SetOrigin()
{
}
void ModelPreview::SetScaleBackgroundImage(bool b) {
    scaleImage = b;
    Refresh();
}

void ModelPreview::SetbackgroundImage(wxString img)
{
    image = NULL;
    mBackgroundImage = img;
    mBackgroundImageExists = wxFileExists(mBackgroundImage)?true:false;
}

void ModelPreview::SetBackgroundBrightness(int brightness)
{
   mBackgroundBrightness = brightness;
   if(mBackgroundBrightness < 0 || mBackgroundBrightness > 100)
   {
        mBackgroundBrightness = 100;
   }
}

void ModelPreview::SetPointSize(wxDouble pointSize)
{
    mPointSize = pointSize;
    glPointSize( mPointSize );
}
double ModelPreview::calcPixelSize(double i) {
    double d = translateToBacking(i * currentPixelScaleFactor);
    if (d < 1.0) {
        d = 1.0;
    }
    return d;
}


bool ModelPreview::StartDrawing(wxDouble pointSize)
{
    if(!IsShownOnScreen()) return false;
    if(!mIsInitialized) { InitializeGLCanvas(); }
    mIsInitialized = true;
    mPointSize = pointSize;
    mIsDrawing = true;
    SetCurrentGLContext();
    wxClientDC dc(this);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if( mWindowResized )
    {
        prepare2DViewport(0,0,mWindowWidth, mWindowHeight);
    }
    glPointSize(translateToBacking(mPointSize));
    DrawGLUtils::PushMatrix();
    // Rotate Axis and tranlate
    DrawGLUtils::Rotate(180,0,0,1);
    DrawGLUtils::Rotate(180,0,1,0);

    currentPixelScaleFactor = 1.0;
    if (!allowSelected && virtualWidth > 0 && virtualHeight > 0
        && (virtualWidth != mWindowWidth || virtualHeight != mWindowHeight)) {
        DrawGLUtils::Translate(0,-mWindowHeight,0);
        double scaleh= double(mWindowHeight) / double(virtualHeight);
        double scalew = double(mWindowWidth) / double(virtualWidth);
        DrawGLUtils::Scale(scalew, scaleh, 1.0);

        if (scalew < scaleh) {
            scaleh = scalew;
        }
        currentPixelScaleFactor = scaleh;
        glPointSize(calcPixelSize(mPointSize));
        DrawGLUtils::DrawFillRectangle(xlBLACK, 255, 0, 0, virtualWidth, virtualHeight);
    } else if (virtualWidth == 0 && virtualHeight == 0) {
        DrawGLUtils::Translate(0, -mWindowHeight, 0);
    } else {
        DrawGLUtils::Translate(0, -virtualHeight, 0);
        DrawGLUtils::DrawFillRectangle(xlBLACK, 255, 0, 0, virtualWidth, virtualHeight);
    }
    if(mBackgroundImageExists)
    {
        if (image == NULL)
        {
           image = new Image(mBackgroundImage);
           sprite = new xLightsDrawable(image);
        }
        float intensity = mBackgroundBrightness*.01;
        DrawGLUtils::PushMatrix();
        double scaleh= double(virtualHeight) / double(image->height);
        double scalew = double(virtualWidth) / double(image->width);
        if (scaleImage) {
            DrawGLUtils::Scale(scalew, scaleh, 1.0);
        } else {
            if (scalew < scaleh) {
                scaleh = scalew;
            }
            DrawGLUtils::Scale(scaleh, scaleh, 1.0);
        }
        
        glColor3f(intensity, intensity, intensity);
        glEnable(GL_TEXTURE_2D);   // textures
        sprite->render();
        glDisable(GL_TEXTURE_2D);   // textures
        DrawGLUtils::PopMatrix();
    }
    return true;
}

void ModelPreview::EndDrawing()
{
    DrawGLUtils::PopMatrix();
    SwapBuffers();
    mIsDrawing = false;
}

