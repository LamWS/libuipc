#include <finite_element/codim_2d_constitution.h>
#include <finite_element/constitutions/baraff_witkin_shell_2d_function.h>
#include <kernel_cout.h>
#include <muda/ext/eigen/log_proxy.h>
#include <Eigen/Dense>
#include <muda/ext/eigen/inverse.h>
#include <utils/codim_thickness.h>

namespace uipc::backend::cuda
{
class BaraffWitkinShell2D final : public Codim2DConstitution
{
  public:
    // Constitution UID by libuipc specification
    static constexpr U64 ConstitutionUID = 18;

    using Codim2DConstitution::Codim2DConstitution;

    vector<Float> h_kappas;
    vector<Float> h_lambdas;

    muda::DeviceBuffer<Float> kappas;
    muda::DeviceBuffer<Float> lambdas;

    virtual U64 get_constitution_uid() const override
    {
        return ConstitutionUID;
    }

    virtual void do_build(BuildInfo& info) override {}

    virtual void do_retrieve(FiniteElementMethod::Codim2DFilteredInfo& info) override
    {
        using ForEachInfo = FiniteElementMethod::ForEachInfo;

        auto geo_slots = world().scene().geometries();
    }

    virtual void do_compute_energy(ComputeEnergyInfo& info) override
    {
        using namespace muda;
        namespace BWS = sym::baraff_witkin_shell_2d;

        ParallelFor()
            .file_line(__FILE__, __LINE__)
            .apply(info.indices().size(),
                   [mus        = kappas.cviewer().name("mus"),
                    lambdas    = lambdas.cviewer().name("lambdas"),
                    rest_areas = info.rest_areas().viewer().name("rest_area"),
                    thicknesses = info.thicknesses().viewer().name("thicknesses"),
                    element_energies = info.element_energies().viewer().name("energies"),
                    indices = info.indices().viewer().name("indices"),
                    xs      = info.xs().viewer().name("xs"),
                    x_bars  = info.x_bars().viewer().name("x_bars"),
                    dt      = info.dt()] __device__(int I)
                   {
                       //
                       MUDA_ERROR_WITH_LOCATION("Not implemented yet");
                   });
    }

    virtual void do_compute_gradient_hessian(ComputeGradientHessianInfo& info) override
    {
        using namespace muda;
        namespace BWS = sym::baraff_witkin_shell_2d;

        ParallelFor()
            .file_line(__FILE__, __LINE__)
            .apply(info.indices().size(),
                   [mus     = kappas.cviewer().name("mus"),
                    lambdas = lambdas.cviewer().name("lambdas"),
                    indices = info.indices().viewer().name("indices"),
                    xs      = info.xs().viewer().name("xs"),
                    x_bars  = info.x_bars().viewer().name("x_bars"),
                    thicknesses = info.thicknesses().viewer().name("thicknesses"),
                    G9s        = info.gradient().viewer().name("gradient"),
                    H9x9s      = info.hessian().viewer().name("hessian"),
                    rest_areas = info.rest_areas().viewer().name("volumes"),
                    dt         = info.dt()] __device__(int I)
                   {
                       //
                       MUDA_ERROR_WITH_LOCATION("Not implemented yet");
                   });
    }
};

REGISTER_SIM_SYSTEM(BaraffWitkinShell2D);
}  // namespace uipc::backend::cuda
