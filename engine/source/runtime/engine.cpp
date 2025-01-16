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


namespace VE
{
    bool g_is_editor_mode{true};

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

    void VirtualEngine::startEngine(const EngineInitParams& param)
    {
        Reflection::TypeMetaRegister::Register();

        ConfigManager::getInstance().initialize(param);
        AssetManager::getInstance().initialize();
        VUIManager::getInstance().initialize();

        WorldManager::getInstance().initialize();
        SceneManager::getInstance().initialize();

        m_tri_frame_buffer.initialize();
        m_renderer->ResgisterGetPtr(std::bind(&getFrameBuffer, &m_tri_frame_buffer, std::placeholders::_1));
        m_renderer->initialize();

        LOG_INFO("engine start");
    }

    void VirtualEngine::shutdownEngine()
    {
        LOG_INFO("engine shutdown");


        m_tri_frame_buffer.clear();
    }



    void VirtualEngine::initialize() {}
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

            logicalTick(delta_time);
            if (!rendererTick())
                return;
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
        m_last_producing_index = m_producing_index;
        do
        {
            m_producing_index = (m_producing_index + 1) % 3;
        } while (m_consuming_index == m_producing_index || m_last_producing_index == m_producing_index);

        three_buffers._array[m_producing_index]->logicalFrameIndex = ++m_logical_frame_index;

        return three_buffers._array[m_producing_index];
    }
    FrameBuffer* ThreeFrameBuffers::getProducingBuffer() { return nullptr; }



} // namespace VE
