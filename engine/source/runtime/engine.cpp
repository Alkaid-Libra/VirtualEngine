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

    VirtualEngine::VirtualEngine() { m_renderer = std::make_shared<VirtualRenderer>(); }

    void VirtualEngine::startEngine(const EngineInitParams& param)
    {
        // Reflection::TypeMetaRegister::Register();

        ConfigManager::getInstance().initialize(param);
        AssetManager::getInstance().initialize();
        VUIManager::getInstance().initialize();

        WorldManager::getInstance().initialize();
        SceneManager::getInstance().initialize();

        m_renderer->initialize();

        LOG_INFO("engine start");
    }

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

    FrameBuffer* ThreeFrameBuffers::producingBufferShift()
    {
        return nullptr;
    }
    FrameBuffer* ThreeFrameBuffers::getProducingBuffer() { return nullptr; }
} // namespace VE
