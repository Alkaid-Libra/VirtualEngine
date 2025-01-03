#pragma once

#include "runtime/core/base/public_singleton.h"

#include <filesystem>
#include <atomic>

namespace VE
{
    class VirtualRenderer;
    class FrameBuffer;
    class SurfaceIO;

    extern bool g_is_editor_mode;

    struct EngineInitParams
    {
        std::filesystem::path m_root_folder;
        std::filesystem::path m_config_file_path;
    };

    class ThreeFrameBuffers
    {
        union TriBuffer
        {
            struct _Struct
            {
                FrameBuffer* _A;
                FrameBuffer* _B;
                FrameBuffer* _C;
            } _struct;
            FrameBuffer* (_array)[3];
        } three_buffers;

        std::atomic<size_t> m_logical_frame_index{0};
        size_t m_last_producing_index{0};
        size_t m_producing_index{0};
        size_t m_consuming_index{0};

    public:
        void initialize();
        void clear();

        FrameBuffer* producingBufferShift();
        FrameBuffer* getProducingBuffer();

        const FrameBuffer* consumingBufferShift();
        const FrameBuffer* getConsumingBuffer();
        
    };

    class VirtualEngine : public PublicSingleton<VirtualEngine>
    {
        friend class PublicSingleton<VirtualEngine>;

    protected:
        VirtualEngine();

        bool m_is_quit{false};
        std::chrono::steady_clock::time_point m_last_tick_time_point{std::chrono::steady_clock::now()};

        ThreeFrameBuffers m_tri_frame_buffer;
        std::shared_ptr<VirtualRenderer> m_renderer;

        void logicalTick(float delta_time);
        bool rendererTick();

        bool rendererTick(float delta_time);


    public:
        VirtualEngine(const VirtualEngine&) = delete;
        VirtualEngine& operator=(const VirtualEngine&) = delete;

        void startEngine(const EngineInitParams& params);
        void shutdownEngine();

        void initialize();
        void initialize(const std::string& config_file_path);
        void clear();

        bool isQuit() const { return m_is_quit; }
        void run();

        std::shared_ptr<SurfaceIO> getSurfaceIO();
        std::shared_ptr<VirtualRenderer> getRender() const { return m_renderer; }
    };
} // namespace VE







#pragma once

#include <cassert>
#include <chrono>
#include <memory>
#include <string>

namespace Piccolo
{
    class PiccoloEngine
    {
    public:
        void initialize(const std::string& config_file_path);
        void shutdown();

        float calculateDeltaTime();
        bool  tickOneFrame(float delta_time);

    protected:
        bool rendererTick(float delta_time);

        std::chrono::steady_clock::time_point tick_time_point_last = std::chrono::steady_clock::now(); // 上个时间点
    };
} // namespace Piccolo

