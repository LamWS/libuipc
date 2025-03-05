#include <finite_element/finite_element_kinetic.h>
#include <finite_element/finite_element_diff_dof_reporter.h>

namespace uipc::backend::cuda
{
REGISTER_SIM_SYSTEM(FiniteElementKinetic);

void FiniteElementKinetic::do_build(BuildInfo& info)
{
    m_impl.finite_element_method = &require<FiniteElementMethod>();
    m_impl.finite_element_method->add_constitution(this);
}

void FiniteElementKinetic::do_report_extent(ReportExtentInfo& info)
{
    auto vert_count = m_impl.fem().xs.size();
    info.energy_count(vert_count);
    info.stencil_dim(1);
}

void FiniteElementKinetic::do_compute_energy(ComputeEnergyInfo& info)
{
    m_impl.compute_energy(info);
}

void FiniteElementKinetic::do_compute_gradient_hessian(ComputeGradientHessianInfo& info)
{
    m_impl.compute_gradient_hessian(info);
}

void FiniteElementKinetic::Impl::compute_energy(ComputeEnergyInfo& info)
{
    using namespace muda;
    // Compute kinetic energy
    ParallelFor()
        .file_line(__FILE__, __LINE__)
        .apply(fem().xs.size(),
               [is_fixed   = fem().is_fixed.cviewer().name("is_fixed"),
                xs         = fem().xs.cviewer().name("xs"),
                is_dynamic = fem().is_dynamic.cviewer().name("is_dynamic"),
                gravities  = fem().gravities.cviewer().name("gravities"),
                x_tildes   = fem().x_tildes.viewer().name("x_tildes"),
                masses     = fem().masses.cviewer().name("masses"),
                dt         = info.dt(),
                Ks = info.energies().viewer().name("kinetic_energy")] __device__(int i) mutable
               {
                   auto& K = Ks(i);
                   if(is_fixed(i))
                   {
                       K = 0.0;
                   }
                   else
                   {
                       if(is_dynamic(i))
                       {
                           const Vector3& x       = xs(i);
                           const Vector3& x_tilde = x_tildes(i);
                           Float          M       = masses(i);
                           Vector3        dx      = x - x_tilde;
                           K                      = 0.5 * M * dx.dot(dx);
                       }
                       else
                       {
                           const Vector3& g = gravities(i);
                           const Vector3& x = xs(i);
                           K                = -x.dot(g) * dt * dt;
                       }
                   }
               });
}
void FiniteElementKinetic::Impl::compute_gradient_hessian(ComputeGradientHessianInfo& info)
{
    using namespace muda;

    // Kinetic
    ParallelFor()
        .file_line(__FILE__, __LINE__)
        .apply(fem().xs.size(),
               [xs         = fem().xs.viewer().name("xs"),
                x_tildes   = fem().x_tildes.cviewer().name("x_tildes"),
                vs         = fem().vs.viewer().name("vs"),
                masses     = fem().masses.cviewer().name("masses"),
                is_fixed   = fem().is_fixed.cviewer().name("is_fixed"),
                is_dynamic = fem().is_dynamic.cviewer().name("is_dynamic"),
                G3s        = info.gradients().viewer().name("G3s"),
                gravities  = fem().gravities.cviewer().name("gravities"),
                dt         = info.dt(),
                H3x3s = info.hessians().viewer().name("H3x3s")] __device__(int i) mutable
               {
                   auto& m       = masses(i);
                   auto& x       = xs(i);
                   auto& x_tilde = x_tildes(i);

                   Vector3   G;
                   Matrix3x3 H;

                   if(is_fixed(i))  // fixed
                   {
                       G = Vector3::Zero();
                   }
                   else
                   {
                       if(is_dynamic(i))
                       {
                           G = m * (x - x_tilde);
                       }
                       else
                       {
                           G = -gravities(i) * dt * dt;
                       }
                   }
                   if(is_dynamic(i))
                   {
                       H = masses(i) * Matrix3x3::Identity();
                   }
                   else
                   {
                       H = Matrix3x3::Zero();
                   }

                   G3s(i).write(i, G);
                   H3x3s(i).write(i, i, H);
               });
}
}  // namespace uipc::backend::cuda
