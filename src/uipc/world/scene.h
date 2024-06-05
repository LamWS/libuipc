#pragma once
#include <uipc/world/contact_tabular.h>
#include <uipc/world/constitution_tabular.h>
#include <uipc/world/object.h>
#include <uipc/world/object_collection.h>

namespace uipc::backend
{
class SceneVisitor;
}

namespace uipc::world
{
class UIPC_CORE_API Scene
{
    friend class backend::SceneVisitor;
    friend class World;
    friend class Object;

  public:
    class UIPC_CORE_API Objects
    {
        friend class Scene;

      public:
        P<Object> create(std::string_view name = "") &&;
        P<Object> find(IndexT id) && noexcept;
        void      destroy(IndexT id) &&;
        SizeT     size() const noexcept;

      private:
        Objects(Scene& scene) noexcept;
        Scene& m_scene;
    };

    class UIPC_CORE_API CObjects
    {
        friend class Scene;

      public:
        P<const Object> find(IndexT id) && noexcept;
        SizeT           size() const noexcept;

      private:
        CObjects(const Scene& scene) noexcept;
        const Scene& m_scene;
    };

    class UIPC_CORE_API Geometries
    {
        friend class Scene;

      public:
        template <std::derived_from<geometry::Geometry> GeometryT>
        ObjectGeometrySlots<GeometryT> find(IndexT id) && noexcept;


      private:
        Geometries(Scene& scene) noexcept;
        Scene& m_scene;
    };

    class UIPC_CORE_API CGeometries
    {
        friend class Scene;

      public:
        template <std::derived_from<geometry::Geometry> GeometryT>
        ObjectGeometrySlots<const GeometryT> find(IndexT id) && noexcept;


      private:
        CGeometries(const Scene& scene) noexcept;
        const Scene& m_scene;
    };

    Scene() = default;

    ContactTabular&       contact_tabular() noexcept;
    const ContactTabular& contact_tabular() const noexcept;

    ConstitutionTabular&       constitution_tabular() noexcept;
    const ConstitutionTabular& constitution_tabular() const noexcept;

    Objects  objects() noexcept;
    CObjects objects() const noexcept;

    Geometries  geometries() noexcept;
    CGeometries geometries() const noexcept;

  private:
    ContactTabular      m_contact_tabular;
    ConstitutionTabular m_constitution_tabular;
    ObjectCollection    m_objects;

    geometry::GeometryCollection m_geometries;
    geometry::GeometryCollection m_rest_geometries;

    bool m_started = false;
    void solve_pending() noexcept;
};
}  // namespace uipc::world
#include "details/scene.inl"
