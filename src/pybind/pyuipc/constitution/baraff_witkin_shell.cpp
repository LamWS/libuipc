#include <pyuipc/constitution/baraff_witkin_shell.h>
#include <uipc/constitution/baraff_witkin_shell.h>
#include <pyuipc/common/json.h>

namespace pyuipc::constitution
{
using namespace uipc::constitution;
PyBaraffWitkinShell::PyBaraffWitkinShell(py::module& m)
{
    auto class_BaraffWitkinShell =
        py::class_<BaraffWitkinShell, FiniteElementConstitution>(m, "BaraffWitkinShell");

    class_BaraffWitkinShell.def(py::init<const Json&>(),
                              py::arg("config") = BaraffWitkinShell::default_config());

    class_BaraffWitkinShell.def_static("default_config", &BaraffWitkinShell::default_config);

    class_BaraffWitkinShell.def("apply_to",
                              &BaraffWitkinShell::apply_to,
                              py::arg("sc"),
                              py::arg("moduli") =
                                  ElasticModuli::youngs_poisson(10.0_MPa, 0.49),
                              py::arg("mass_density") = 1.0e3,
                              py::arg("thickness")    = 0.01_m);
}
}  // namespace pyuipc::constitution
