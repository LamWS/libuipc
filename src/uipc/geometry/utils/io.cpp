#include <uipc/geometry/io.h>
#include <uipc/geometry/factory.h>
#include <igl/readMSH.h>
#include <uipc/common/format.h>
#include <uipc/common/enumerate.h>
#include <filesystem>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOBJ.h>
#include <uipc/builtin/attribute_name.h>

namespace uipc::geometry
{
template <typename T>
using RowMajorMatrix =
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
using Eigen::VectorXi;

namespace fs = std::filesystem;

SimplicialComplex SimplicialComplexIO::read(std::string_view file_name)
{
    fs::path path{file_name};
    if(path.extension() == ".msh")
    {
        return read_msh(file_name);
    }
    else if(path.extension() == ".obj")
    {
        return read_obj(file_name);
    }
    else
    {
        throw GeometryIOError{fmt::format("Unsupported file format: {}", file_name)};
    }
}

SimplicialComplex SimplicialComplexIO::read_msh(std::string_view file_name)
{
    if(!std::filesystem::exists(file_name))
    {
        throw GeometryIOError{fmt::format("File does not exist: {}", file_name)};
    }
    RowMajorMatrix<Float>  X;
    RowMajorMatrix<IndexT> F;
    RowMajorMatrix<IndexT> T;
    VectorXi               TriTag;
    VectorXi               TetTag;
    if(!igl::readMSH(std::string{file_name}, X, F, T, TriTag, TetTag))
    {
        throw GeometryIOError{fmt::format("Failed to load .msh file: {}", file_name)};
    }
    vector<Vector3> Vs;
    Vs.resize(X.rows());
    for(auto&& [i, v] : enumerate(Vs))
        v = X.row(i);
    vector<Vector4i> Ts;
    Ts.resize(T.rows());
    for(auto&& [i, t] : enumerate(Ts))
        t = T.row(i);
    return tetmesh(Vs, Ts);
}

SimplicialComplex SimplicialComplexIO::read_obj(std::string_view file_name)
{
    if(!std::filesystem::exists(file_name))
    {
        throw GeometryIOError{fmt::format("File does not exist: {}", file_name)};
    }
    // TODO: We may want to take more information from the .obj file
    RowMajorMatrix<Float>  X;
    RowMajorMatrix<IndexT> F;
    if(!igl::read_triangle_mesh(std::string{file_name}, X, F))
    {
        throw GeometryIOError{fmt::format("Failed to load .obj file: {}", file_name)};
    }
    vector<Vector3> Vs;
    Vs.resize(X.rows());
    for(auto&& [i, v] : enumerate(Vs))
        v = X.row(i);
    vector<Vector3i> Fs;
    Fs.resize(F.rows());
    for(auto&& [i, f] : enumerate(Fs))
        f = F.row(i);
    return trimesh(Vs, Fs);
}

void SimplicialComplexIO::write_obj(std::string_view file_name, const SimplicialComplex& sc)
{
    if(sc.dim() > 2)
    {
        throw GeometryIOError{fmt::format("Cannot write simplicial complex of dimension {} to .obj file",
                                          sc.dim())};
    }

    auto fp = std::fopen(file_name.data(), "w");

    if(!fp)
    {
        throw GeometryIOError{fmt::format("Failed to open file {} for writing.", file_name)};
    }

    fmt::println(fp,
                 R"(# File generated by Libuipc
# Vertices {}
# Faces {})",
                 sc.vertices().size(),
                 sc.triangles().size());

    auto orient = sc.triangles().find<IndexT>(builtin::orient);

    auto Fs = sc.triangles().topo().view();
    auto Vs = sc.positions().view();

    for(auto&& v : Vs)
        fmt::println(fp, "v {} {} {}", v[0], v[1], v[2]);

    if(orient)
    {
        auto orient_view = sc.triangles().find<IndexT>(builtin::orient)->view();

        for(auto&& [i, F] : enumerate(Fs))
        {
            if(orient_view[i] >= 1)  // outward orientation, write as is
                fmt::println(fp, "f {} {} {}", F[0] + 1, F[1] + 1, F[2] + 1);
            else  // inward orientation, flip the order
                fmt::println(fp, "f {} {} {}", F[0] + 1, F[2] + 1, F[1] + 1);
        }
    }
    else
    {
        for(auto&& F : Fs)
            fmt::println(fp, "f {} {} {}", F[0] + 1, F[1] + 1, F[2] + 1);
    }

    std::fclose(fp);
}
}  // namespace uipc::geometry
