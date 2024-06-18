#include <catch.hpp>
#include <app/asset_dir.h>
#include <uipc/geometry/utils/io.h>
#include <uipc/common/format.h>

using namespace uipc;
using namespace uipc::geometry;
TEST_CASE("read_msh", "[io]")
{
    SimplicialComplexIO io;

    auto mesh = io.read_msh(fmt::format("{}cube.msh", AssetDir::tetmesh_path()));
    REQUIRE(mesh.vertices().size() == 8);
    REQUIRE(mesh.tetrahedra().size() == 5);
    REQUIRE(mesh.dim() == 3);

    REQUIRE_THROWS_AS(io.read_msh(fmt::format("{}NOMESH.msh", AssetDir::tetmesh_path())),
                      GeometryIOError);
}

TEST_CASE("read_obj", "[io]")
{
    SimplicialComplexIO io;
    auto mesh = io.read_obj(fmt::format("{}cube.obj", AssetDir::trimesh_path()));
    REQUIRE(mesh.vertices().size() == 8);
    REQUIRE(mesh.triangles().size() == 12);
    REQUIRE(mesh.tetrahedra().size() == 0);
    REQUIRE(mesh.dim() == 2);

    REQUIRE_THROWS_AS(io.read_obj(fmt::format("{}NOMESH.obj", AssetDir::trimesh_path())),
                      GeometryIOError);
}
