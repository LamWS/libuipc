#pragma once
#include <global_geometry/vertex_reporter.h>
#include <finite_element/finite_element_method.h>

namespace uipc::backend::cuda
{
class FiniteElementVertexReporter final : public VertexReporter
{
  public:
    using VertexReporter::VertexReporter;

    class Impl
    {
      public:
        void report_count(GlobalVertexManager::VertexCountInfo& info);
        void report_attributes(GlobalVertexManager::VertexAttributeInfo& info);
        void report_displacements(GlobalVertexManager::VertexDisplacementInfo& info);

        SizeT                      reporter_vertex_offset = 0;
        SizeT                      reporter_vertex_count  = 0;
        FiniteElementMethod*       finite_element_method;
        FiniteElementMethod::Impl& fem()
        {
            return finite_element_method->m_impl;
        }
    };

    SizeT vertex_offset() const noexcept
    {
        return m_impl.reporter_vertex_offset;
    }
    SizeT vertex_count() const noexcept { return m_impl.reporter_vertex_count; }

  protected:
    virtual void do_build(BuildInfo& info) override;
    virtual void do_report_count(GlobalVertexManager::VertexCountInfo& info) override;
    virtual void do_report_attributes(GlobalVertexManager::VertexAttributeInfo& info) override;
    virtual void do_report_displacements(GlobalVertexManager::VertexDisplacementInfo& info) override;

  private:
    Impl m_impl;
};
}  // namespace uipc::backend::cuda
