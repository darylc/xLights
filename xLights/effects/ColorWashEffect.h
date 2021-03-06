#ifndef COLORWASHEFFECT_H
#define COLORWASHEFFECT_H

#include "RenderableEffect.h"


class ColorWashEffect : public RenderableEffect
{
    public:
        ColorWashEffect(int id);
        virtual ~ColorWashEffect();
    
        virtual void SetDefaultParameters(Model *cls) override;
        virtual void Render(Effect *effect, const SettingsMap &settings, RenderBuffer &buffer) override;
        virtual int DrawEffectBackground(const Effect *e, int x1, int y1, int x2, int y2, DrawGLUtils::xlVertexColorAccumulator &bg) override;
    
    
        virtual std::string GetEffectString() override;

    protected:
        virtual wxPanel *CreatePanel(wxWindow *parent) override;
    private:
};

#endif // COLORWASHEFFECT_H
