#ifndef STRAITX_APPLICATION_HPP
#define STRAITX_APPLICATION_HPP

#include "main/engine.hpp"
#include "core/math/vector2.hpp"

namespace StraitX{

struct ApplicationConfig{
    Vector2u WindowSize;
};

extern ApplicationConfig GameConfig;

class Application{
private:
    Engine *m_Engine;
    const char *m_Name;
    friend class Engine;
public:
    Application(const char *name);
    virtual ~Application() = default;

    virtual Result OnInitialize(){return Result::Success;};

    // return true if event was handled
    virtual bool OnEvent(const Event &event){return false;};

    virtual Result OnFinalize(){return Result::Success;};

    virtual void OnUpdate(float dt){
        (void)dt;
    };

    void Stop();

    const char *Name();

private:
    void SetEngine(Engine *engine);

};

}; // namespace StraitX::


#endif // STRAITX_APPLICATION_HPP