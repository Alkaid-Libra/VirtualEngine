#pragma once

#include "runtime/core/math/math_headers.h"

#include "render_mesh.h"
#include "material.h"

#include <memory>
#include <atomic>
#include <mutex>

namespace VE
{

    enum class VIRTUAL_PIXEL_FORMAT : size_t
    {
        VIRTUAL_PIXEL_FORMAT_R8G8B8_UNORM = 1,
        VIRTUAL_PIXEL_FORMAT_R8G8B8_SRGB,
        VIRTUAL_PIXEL_FORMAT_R8G8B8A8_UNORM,
        VIRTUAL_PIXEL_FORMAT_R8G8B8A8_SRGB,
        VIRTUAL_PIXEL_FORMAT_R32G32_FLOAT,
        VIRTUAL_PIXEL_FORMAT_R32G32B32_FLOAT,
        VIRTUAL_PIXEL_FORMAT_R32G32B32A32_FLOAT
    };
    enum class VIRTUAL_IMAGE_TYPE : size_t
    {
        VIRTUAL_IMAGE_TYPE_2D
    };

    class SceneMemory
    {
    public:
        size_t m_size;
        char* m_data;
    };    
    class SceneImage
    {
    public:
        size_t m_width;
        size_t m_height;
        size_t m_depth;

        size_t m_mip_levels;
        size_t m_array_layers;

        VIRTUAL_PIXEL_FORMAT m_format;
        VIRTUAL_IMAGE_TYPE   m_type;
        void*                m_pixels;
    };

    class SceneBuffers
    {
    public:
        static void destroy(VertexBufferHandle handle)
        {
            auto scene_memory = (SceneMemory*)handle.handle;
            if (scene_memory)
            {
                if (scene_memory->m_data)
                {
                    delete[] scene_memory->m_data;
                    scene_memory->m_data = nullptr;
                }
                delete scene_memory;
            }
        }
        static void destroy(IndexBufferHandle handle)
        {
            auto scene_memory = (SceneMemory*)handle.handle;
            if (scene_memory)
            {
                if (scene_memory->m_data)
                {
                    delete[] scene_memory->m_data;
                    scene_memory->m_data = nullptr;
                }
                delete scene_memory;
            }
        }
        static void destroy(TextureHandle handle)
        {
            auto scene_memory = (SceneMemory*)handle.handle;
            if (scene_memory)
            {
                if (scene_memory->m_data)
                {
                    delete[] scene_memory->m_data;
                    scene_memory->m_data = nullptr;
                }
                delete scene_memory;
            }
        }
        static void destroy(SkeletonBindingBufferHandle handle)
        {
            auto scene_memory = (SceneMemory*)handle.handle;
            if (scene_memory)
            {
                if (scene_memory->m_data)
                {
                    delete[] scene_memory->m_data;
                    scene_memory->m_data = nullptr;
                }
                delete scene_memory;
            }
        }

        static const SceneMemory* alloc(size_t size) { return new SceneMemory{size, new char[size]}; }

        static VertexBufferHandle createVertexBuffer(const SceneMemory* memory, size_t) { return { (size_t)memory }; }
        static IndexBufferHandle createIndexBuffer(const SceneMemory* memory) { return { (size_t)memory }; }
        static SkeletonBindingBufferHandle createSkeletonBindingBuffer(const SceneMemory* memory) { return { (size_t)memory}; }
    
        static SceneImage* allocTexture() { return new SceneImage(); }
        static TextureHandle createTexture(const SceneImage* texture) { return { (size_t)texture }; }

    };

    enum class VRenderPath : int
    {
        Forward,
        Deferred,
        Clustered
    };

    class Scene
    {
    public:
        bool m_loaded = false;

        Vector3 m_minBounds{0, 0, 0};
        Vector3 m_maxBounds{0, 0, 0};
        Vector3 m_center{0, 0, 0};
        float m_diagonal{0};

        // std::shared_ptr<PCamera> m_camera;
        


        void lock() { m_scene_mutex.lock(); }
        void unlock() { m_scene_mutex.unlock(); }

        void addMesh(const RenderMesh& mesh) { m_meshes.push_back(mesh); }
        void addMaterial(const Material& material) { m_materials.push_back(material); }

        void clear()
        {
            clearMeshes();
            clearMaterial();
        }
        void clearMeshes() { m_meshes.clear(); }
        void clearMaterial() { m_materials.clear(); }

        void setAxisMesh(const std::vector<RenderMesh>& axis_meshes) { m_axis = axis_meshes;}

    private:
        std::mutex m_scene_mutex;

        std::vector<RenderMesh> m_meshes;
        std::vector<Material> m_materials;
        std::vector<RenderMesh> m_axis;
    };

    class UIState
    {
    public:
        bool m_writeLog{true};
        VRenderPath m_renderPath{VRenderPath::Forward};

        bool m_fullscreen{false};
        bool m_showUI{true};
        bool m_showConfigWindow{true};
        bool m_showLog{false};

        bool m_showStatsOverlay{false};
        struct
        {
            bool fps;
            bool frameTime;
            bool profiler;
            bool gpuMemory;
        } m_overlays{true, true, true, true};

        // std::shared_ptr<VCamera> m_editor_camera;
        
    };

    struct SceneReleaseHandles
    {

    };

    class FrameBuffer
    {
    public:
        FrameBuffer() : m_uistate(std::make_unique<UIState>()) {}
        
        size_t logicalFrameIndex{0};

        std::unique_ptr<UIState> m_uistate;
        std::shared_ptr<Scene> m_scene;
        VRenderPath m_renderpath{VRenderPath::Clustered};
    };
} // namespace VE

template<>
struct std::hash<VE::SceneResourceHandle>
{
    size_t operator()(const VE::SceneResourceHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::VertexBufferHandle>
{
    size_t operator()(const VE::VertexBufferHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::IndexBufferHandle>
{
    size_t operator()(const VE::IndexBufferHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::TextureHandle>
{
    size_t operator()(const VE::TextureHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::DynamicVertexBufferHandle>
{
    size_t operator()(const VE::DynamicVertexBufferHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::SkeletonBindingBufferHandle>
{
    size_t operator()(const VE::SkeletonBindingBufferHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::MeshHandle>
{
    size_t operator()(const VE::MeshHandle& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::SkeletonMeshBinding>
{
    size_t operator()(const VE::SkeletonMeshBinding& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::RenderMesh>
{
    size_t operator()(const VE::RenderMesh& rhs) const noexcept { return rhs.getHashValue(); }
};
template<>
struct std::hash<VE::Material>
{
    size_t operator()(const VE::Material& rhs) const noexcept { return rhs.getHashValue(); }
};
