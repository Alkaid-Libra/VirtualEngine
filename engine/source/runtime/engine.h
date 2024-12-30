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


    public:
        VirtualEngine(const VirtualEngine&) = delete;
        VirtualEngine& operator=(const VirtualEngine&) = delete;

        void startEngine(const EngineInitParams& params);
        void shutdownEngine();

        void initialize();
        void clear();

        bool isQuit() const { return m_is_quit; }
        void run();

        std::shared_ptr<SurfaceIO> getSurfaceIO();
        std::shared_ptr<VirtualRenderer> getRender() const { return m_renderer; }
    };
} // namespace VE

