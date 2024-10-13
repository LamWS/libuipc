import numpy
import pyuipc.geometry

class AffineBodyConstitution(IConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, kappa: float, mass_density: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class Constraint(IConstitution):
    def __init__(self, *args, **kwargs) -> None: ...

class ElasticModuli:
    def __init__(self, *args, **kwargs) -> None: ...
    @staticmethod
    def lame(arg0: float, arg1: float) -> ElasticModuli: ...
    def mu(self) -> float: ...
    @staticmethod
    def youngs_poisson(arg0: float, arg1: float) -> ElasticModuli: ...
    @staticmethod
    def youngs_shear(arg0: float, arg1: float) -> ElasticModuli: ...

class Empty(FiniteElementConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, mass_density: float = ..., thickness: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class FiniteElementConstitution(IConstitution):
    def __init__(self, *args, **kwargs) -> None: ...

class FiniteElementExtraConstitution(IConstitution):
    def __init__(self, *args, **kwargs) -> None: ...

class HookeanSpring(FiniteElementConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, moduli: float = ..., mass_density: float = ..., thickness: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class IConstitution:
    def __init__(self, *args, **kwargs) -> None: ...
    def name(self) -> str: ...
    def type(self) -> str: ...
    def uid(self) -> int: ...

class KirchhoffRodBending(FiniteElementExtraConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, E: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class LinearMotor(Constraint):
    def __init__(self, config: json = ...) -> None: ...
    @staticmethod
    def animate(sc: pyuipc.geometry.SimplicialComplex, dt: float) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, strength: float = ..., motor_axis: numpy.ndarray[numpy.float64] = ..., motor_vel: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class NeoHookeanShell(FiniteElementConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, moduli: ElasticModuli = ..., mass_density: float = ..., thickness: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class Particle(FiniteElementConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, mass_density: float = ..., thickness: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class RotatingMotor(Constraint):
    def __init__(self, config: json = ...) -> None: ...
    @staticmethod
    def animate(sc: pyuipc.geometry.SimplicialComplex, dt: float) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, strength: float = ..., motor_axis: numpy.ndarray[numpy.float64] = ..., motor_rot_vel: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class SoftPositionConstraint(Constraint):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, strength_rate: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class SoftTransformConstraint(Constraint):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, strength_rate: numpy.ndarray[numpy.float64] = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...

class StableNeoHookean(FiniteElementConstitution):
    def __init__(self, config: json = ...) -> None: ...
    def apply_to(self, sc: pyuipc.geometry.SimplicialComplex, moduli: ElasticModuli = ..., mass_density: float = ...) -> None: ...
    @staticmethod
    def default_config() -> json: ...
