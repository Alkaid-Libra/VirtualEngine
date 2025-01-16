#pragma once


namespace VE
{
    
    struct VMaterialHandle
    {
        TextureHandle m_image_handle0;
        TextureHandle m_image_handle1;
        TextureHandle m_image_handle2;
        TextureHandle m_image_handle3;
        TextureHandle m_image_handle4;
        
        bool operator==(const VMaterialHandle& rhs) const
        {
            return m_image_handle0 == rhs.m_image_handle0 && m_image_handle1 == rhs.m_image_handle1 &&
                   m_image_handle2 == rhs.m_image_handle2 && m_image_handle3 == rhs.m_image_handle3 &&
                   m_image_handle4 == rhs.m_image_handle4;
        }
    };
    


    struct Material
    {
        bool m_blend = false;
        bool m_doubleSided = false;

        TextureHandle m_baseColorTexture = VIRTUAL_INVALID_HANDLE;
        Vector4 m_baseColorFactor = {1.0f, 1.0f, 1.0f, 1.0f};

        TextureHandle m_metallicRoughnessTexture = VIRTUAL_INVALID_HANDLE; // blue = metallic, green = rough
        float m_metallicFactor = 1.0f;
        float m_roughnessFactor = 1.0f;

        TextureHandle m_normalTexture = VIRTUAL_INVALID_HANDLE;
        float m_normalScale = 1.0f;

        TextureHandle m_occlusionTexture = VIRTUAL_INVALID_HANDLE;
        float m_occlusionStrengh = 1.0f;

        TextureHandle m_emissiveTexture = VIRTUAL_INVALID_HANDLE;
        Vector3 m_emissiveFactor = {0.0f, 0.0f, 0.0f};

        size_t m_guid = VIRTUAL_INVALID_GUID;

        bool operator==(const Material& rhs) const
        {
            return m_baseColorTexture == rhs.m_baseColorTexture &&
                   m_metallicRoughnessTexture == rhs.m_metallicRoughnessTexture &&
                   m_normalTexture == rhs.m_normalTexture &&
                   m_occlusionTexture == rhs.m_occlusionTexture &&
                   m_emissiveTexture == rhs.m_emissiveTexture;
        }
        size_t getHashValue() const
        {
            return (((m_baseColorTexture.getHashValue() ^ (m_metallicRoughnessTexture.getHashValue() << 1)) ^
                    (m_normalTexture.getHashValue() << 1)) ^
                    (m_occlusionTexture.getHashValue() << 1)) ^
                    (m_emissiveTexture.getHashValue() << 1);
        }
    };
    
} // namespace VE
