#include "platform/window_system.hpp"
#include "platform/io.hpp"
#include "core/log.hpp"
#include "graphics/api/graphics_api.hpp"
#include "main/application.hpp"
#include "main/engine.hpp"


#define InitAssert(source,error) Log(source,error);if(error != Result::Success){return Result::Failure;}
//#define I(call,name) {Result __err = call; Log(name,__err); if(__err!=Result::Success){return Result::Failure;}}
#define SupportAssert(b, name) if(b){ LogInfo(name ": Supported"); }else{ LogError(name ": Is Not Supported"); return Result::Failure;}

// should be defined at client side
extern StraitX::Application *StraitXMain();
extern StraitX::Result StraitXExit(StraitX::Application *);

namespace StraitX{


int Engine::Run(){
    LogTrace("Engine::Initialize: Begin");

    Result initCode = Initialize();

    Log("Engine::Initialize",initCode);

    LogSeparator();

    if(initCode == Result::Success){
        LogInfo("Engine: Enter Main loop");
        MainLoop();
        LogInfo("Engine: Left Main loop");
    }else{
        LogWarn("Engine: Main loop was skipped due to Initialization failure");
    }

    LogSeparator();

    LogTrace("Engine: Finalize: Begin");

    Result finalizeCode = Finalize();

    Log("Engine::Finalize",finalizeCode);

    if(finalizeCode != Result::Success || initCode != Result::Success)
        return -1;

    return 0;
}

void Engine::Stop(){
    m_Running = false;
}

Result Engine::Initialize(){
    LogTrace("========= First stage init =========");

    LogInfo("WindowSystem::Initialize: Begin");
    m_ErrorWindowSystem = WindowSystem::Initialize();
    InitAssert("WindowSystem::Initialize", m_ErrorWindowSystem);

    SupportAssert(WindowSystem::CheckSupport(WindowSystem::Ext::DoubleBuffer), "WindowSystem::DoubleBuffer");
    SupportAssert(WindowSystem::CheckSupport(WindowSystem::Ext::OpenGLCore), "WindowSystem::OpenGL Core");

    auto res = GraphicsAPI::Create(GraphicsAPI::OpenGL);
    InitAssert("GraphicsAPI::Create",res);

    LogTrace("GraphicsAPI::Initialize: Begin");
    m_ErrorGraphicsAPI = GraphicsAPI::Instance().Initialize();
    InitAssert("GraphicsAPI::Initialize",m_ErrorGraphicsAPI);

    LogTrace("========= Second stage init =========");

    LogTrace("DisplayServer::Initialize: Begin");
    m_ErrorDisplayServer = m_DisplayServer.Initialize();
    InitAssert("DisplayServer::Initialize", m_ErrorDisplayServer);

    LogTrace("========= Third stage init =========");

    //Engine should be completely initialized at this moment
    LogTrace("StraitXMain: Begin");
    m_Application = StraitXMain();
    m_ErrorMX = (m_Application == nullptr ? Result::NullObject : Result::Success);
    InitAssert("StraitXMain",m_ErrorMX);

    m_DisplayServer.m_Window.SetTitle(m_Application->Name());
    
    m_Application->SetEngine(this);

    LogTrace("Application::OnInitialize: Begin");
    m_ErrorApplication = m_Application->OnInitialize();
    CoreAssert((int)m_ErrorApplication >= 0 && m_ErrorApplication < Result::ResultCodesCount, "Application::OnInitialize() returned unsupported result code");
    InitAssert("Application::OnInitialize",m_ErrorApplication);

    return Result::Success;
}

Result Engine::Finalize(){

    if(m_ErrorApplication==Result::Success){
        LogTrace("Application::OnFinalize: Begin");
        m_ErrorApplication = m_Application->OnFinalize();
        CoreAssert((int)m_ErrorApplication >= 0 && m_ErrorApplication < Result::ResultCodesCount, "Application::OnFinalize() returned unsupported result code");
        Log("Application::OnFinalize",m_ErrorApplication);
    }

    if(m_ErrorMX == Result::Success){
        LogTrace("StraitXExit: Begin");
        m_ErrorMX = StraitXExit(m_Application);
        Log("StraitXExit",m_ErrorMX);
    }

    if(m_ErrorDisplayServer == Result::Success){
        LogTrace("DisplayServer::Finalize: Begin");
        m_ErrorDisplayServer = m_DisplayServer.Finalize();
        Log("DisplayServer::Finalize", m_ErrorDisplayServer);
    }

    if(m_ErrorGraphicsAPI == Result::Success){
        LogTrace("GraphicsAPI::Finalize: Begin");
        m_ErrorGraphicsAPI = GraphicsAPI::Instance().Finalize();
        Log("GraphicsAPI::Finalize",m_ErrorGraphicsAPI);
    }

    if(m_ErrorWindowSystem == Result::Success){
        LogTrace("WindowSystem::Finalize: Begin");
        m_ErrorWindowSystem = WindowSystem::Finalize();
        Log("WindowSystem::Finalize",m_ErrorWindowSystem);
    }


    return Result::Success;
}

void Engine::MainLoop(){
    while(m_Running){
        Event e;
        while(m_DisplayServer.m_Window.PollEvent(e)){
            if(e.Type == EventType::WindowClose)
                Stop();
            else
                (void)m_Application->OnEvent(e);
        }
        float dt = m_Clock.getElapsedTime().asSeconds();
        m_Clock.restart();
        m_Application->OnUpdate(dt);
    }
}

}; // namespace StraitX::
