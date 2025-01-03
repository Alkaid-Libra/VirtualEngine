#include "runtime/engine.h"

#include "runtime/core/base/macro.h"
#include "runtime/core/meta/reflection/reflection_register.h"

#include "runtime/resource/config_manager/config_manager.h"
#include "runtime/resource/asset_manager/asset_manager.h"

#include "runtime/function/render/include/render.h"
#include "runtime/function/ui/ui_system.h"
#include "runtime/function/scene/scene_manager.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/input/input_system.h"



#include "runtime/function/global/global_context.h"


namespace VE
{
    const FrameBuffer* getFrameBuffer(ThreeFrameBuffers* t, const VirtualRenderer*)
    {
        return t->consumingBufferShift();
    }

    const FrameBuffer* ThreeFrameBuffers::consumingBufferShift()
    {
        m_consuming_index = m_last_producing_index;
        return three_buffers._array[m_consuming_index];
    }


    VirtualEngine::VirtualEngine() { m_renderer = std::make_shared<VirtualRenderer>(); }

    // void VirtualEngine::startEngine(const EngineInitParams& param)
    // {
    //     // Reflection::TypeMetaRegister::Register();

    //     ConfigManager::getInstance().initialize(param);
    //     // AssetManager::getInstance().initialize();
    //     VUIManager::getInstance().initialize();

    //     WorldManager::getInstance().initialize();
    //     SceneManager::getInstance().initialize();

    //     m_tri_frame_buffer.initialize();
    //     m_renderer->ResgisterGetPtr(std::bind(&getFrameBuffer, &m_tri_frame_buffer, std::placeholders::_1));
    //     m_renderer->initialize();

    //     LOG_INFO("engine start");
    // }

    // void VirtualEngine::shutdownEngine()
    // {
    //     LOG_INFO("engine shutdown");


    //     m_tri_frame_buffer.clear();
    // }



    void VirtualEngine::initialize() {}
    void VirtualEngine::initialize(const std::string& config_file_path) 
    {
        Piccolo::g_runtime_global_context.startSystems(config_file_path); // 初始化子系统

        // LOG_INFO("engine start");
    }
    void VirtualEngine::clear() {}

    void VirtualEngine::run()
    {
        while (true)
        {
            float delta_time;
            {
                using namespace std::chrono;

                steady_clock::time_point tick_time_point = steady_clock::now();
                duration<float> time_span = duration_cast<duration<float>>(tick_time_point - m_last_tick_time_point);
                delta_time = time_span.count();

                m_last_tick_time_point = tick_time_point;
            }

            // logicalTick(delta_time);

            if (!rendererTick(delta_time))
                return;

            // if (!rendererTick())
            //     return;
        }
        
    }

    void VirtualEngine::logicalTick(float delta_time)
    {
        m_tri_frame_buffer.producingBufferShift();

        WorldManager::getInstance().tick(delta_time);
        SceneManager::getInstance().tick(m_tri_frame_buffer.getProducingBuffer());
        InputSystem::getInstance().tick();
        // PhysicsSystem::getInstance().tick(delta_time);
    }

    bool VirtualEngine::rendererTick() { return m_renderer->tick(); }

    bool VirtualEngine::rendererTick(float delta_time)
    {
        Piccolo::g_runtime_global_context.m_render_system->tick(delta_time);
        return true;
    }

    void ThreeFrameBuffers::initialize()
    {
        three_buffers._struct._A = new FrameBuffer();
        three_buffers._struct._B = new FrameBuffer();
        three_buffers._struct._C = new FrameBuffer();

        // tri frame buffers are designed to use same scene now
        auto current_scene = SceneManager::getInstance().getCurrentScene();
        three_buffers._struct._A->m_scene = current_scene;
        three_buffers._struct._B->m_scene = current_scene;
        three_buffers._struct._C->m_scene = current_scene;
    }
    void ThreeFrameBuffers::clear()
    {
        delete three_buffers._struct._A;
        delete three_buffers._struct._B;
        delete three_buffers._struct._C;
    }


    FrameBuffer* ThreeFrameBuffers::producingBufferShift()
    {
        return nullptr;
    }
    FrameBuffer* ThreeFrameBuffers::getProducingBuffer() { return nullptr; }



} // namespace VE




#include "runtime/engine.h"

#include "runtime/core/meta/reflection/reflection_register.h"

#include "core/base/macro.h"
#include "function/render/window_system.h"
#include "runtime/function/global/global_context.h"

#include <string>

namespace Piccolo
{
    void PiccoloEngine::initialize(const std::string& config_file_path)
    {
        // Reflection::TypeMetaRegister::metaRegister();

        g_runtime_global_context.startSystems(config_file_path); // 初始化子系统

        LOG_INFO("engine start");
    }

    void PiccoloEngine::shutdown() {}

    float PiccoloEngine::calculateDeltaTime()
    {
        using namespace std::chrono;                                    // 作用域using
        steady_clock::time_point tick_time_point = steady_clock::now(); // 用于计量时间点的时钟类型
        duration<float>          time_span =
            duration_cast<duration<float>>(tick_time_point - tick_time_point_last); // 用于计量时间间隔类型
        tick_time_point_last = tick_time_point;                                     // 更新时间
        return time_span.count();                                                   // 返回时间差
    }

    /**
     * @brief 引擎计算一帧
     * @param delta_time
     * @return true 可以更新一帧
     */
    bool PiccoloEngine::tickOneFrame(float delta_time)
    {
        rendererTick(delta_time);

        g_runtime_global_context.m_window_system->setTitle(
            std::string("VirtualEngine - " /*+std::to_string(getFPS())+" FPS"*/).c_str());
        // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
        g_runtime_global_context.m_window_system->pollEvents();
        return !g_runtime_global_context.m_window_system->shouldClose();
    }

    bool PiccoloEngine::rendererTick(float delta_time)
    {
        g_runtime_global_context.m_render_system->tick(delta_time);
        return true;
    }

} // namespace Piccolo