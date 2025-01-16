#pragma once

#include "runtime/core/math/matrix4.h"

namespace VE
{
    REFLECTION_TYPE(GameObjectMeshDesc)
    STRUCT(GameObjectMeshDesc, Fields)
    {
        REFLECTION_BODY(GameObjectMeshDesc)

        std::string mesh_file;
    };
    REFLECTION_TYPE(GameObjectMaterialDesc)
    STRUCT(GameObjectMaterialDesc, Fields)
    {
        REFLECTION_BODY(GameObjectMaterialDesc)

        std::string baseColorTextureFile;
        std::string metallicRoughnessTextureFile;
        std::string normalTextureFile;
        std::string occlusionTextureFile;
        std::string emissiveTextureFile;
        bool with_texture{false};
    };
    REFLECTION_TYPE(GameObjectTransformDesc)
    STRUCT(GameObjectTransformDesc, WhiteListFields)
    {
        REFLECTION_BODY(GameObjectTransformDesc)

        Matrix4x4 transform_matrix{Matrix4x4::IDENTITY};
    };

    REFLECTION_TYPE(SkeletonAnimationResultTransform)
    STRUCT(SkeletonAnimationResultTransform, WhiteListFields)
    {
        REFLECTION_BODY(SkeletonAnimationResultTransform)

        Matrix4x4 matrix; // because Matrix4x4 is not reflection type, 
                          // so must be whitelistfields, not fields(try all reflection)
    };

    REFLECTION_TYPE(SkeletonBindingDesc)
    STRUCT(SkeletonBindingDesc, Fields)
    {
        REFLECTION_BODY(SkeletonBindingDesc)

        std::string skeleton_binding_file;
    };
    REFLECTION_TYPE(SkeletonAnimationResult)
    STRUCT(SkeletonAnimationResult, Fields)
    {
        REFLECTION_BODY(SkeletonAnimationResult)

        std::vector<SkeletonAnimationResultTransform> transforms;
    };

    // component desc
    REFLECTION_TYPE(GameObjectComponentDesc)
    STRUCT(GameObjectComponentDesc, Fields)
    {
        REFLECTION_BODY(GameObjectComponentDesc)
        
        GameObjectMeshDesc mesh_desc;
        GameObjectMaterialDesc material_desc;
        GameObjectTransformDesc transform_desc;
        bool with_animation{false};
        SkeletonBindingDesc skeleton_binding_desc;
        SkeletonAnimationResult skeleton_animation_result;
    };


    class GameObjectDesc
    {
    public:
        GameObjectDesc() : m_go_id(0) {}
        GameObjectDesc(size_t go_id, const std::vector<GameObjectComponentDesc>& components) :
            m_go_id(go_id), m_components(components)
        {}


        size_t getId() const { return m_go_id; }
        const std::vector<GameObjectComponentDesc>& getComponents() const { return m_components; }

    private:
        size_t m_go_id;
        std::vector<GameObjectComponentDesc> m_components;
    };

    static const size_t s_invalid_go_id = std::numeric_limits<size_t>::max();
    static const size_t s_invalid_component_id = std::numeric_limits<size_t>::max();
    struct ComponentId
    {
        size_t go_id{s_invalid_go_id};
        size_t component_id{s_invalid_component_id};

        bool operator==(const ComponentId& rhs) const { return go_id == rhs.go_id && component_id == rhs.component_id; }
        size_t getHashValue() const { return go_id ^ (component_id << 1); }
    };
    


} // namespace VE

template<>
struct std::hash<VE::ComponentId>
{
    size_t operator()(const VE::ComponentId& rhs) const noexcept { return rhs.getHashValue(); }
};