#include <app/test_common.h>
#include <app/asset_dir.h>

#include <uipc/world/scene.h>
#include <uipc/world/world.h>
#include <uipc/engine/engine.h>
#include <uipc/engine/uipc_engine.h>
#include <uipc/geometry.h>
#include <uipc/constitutions/affine_body.h>
#include <fstream>

using namespace uipc;
using namespace uipc::world;
using namespace uipc::geometry;
using namespace uipc::engine;
using namespace uipc::constitution;
namespace fs = std::filesystem;

void test_engine(std::string_view name)
{
    auto this_output_path = AssetDir::output_path(__FILE__);

    UIPCEngine engine{name, this_output_path};
    World      world{engine};

    Scene scene;

    auto& constitution_tabular = scene.constitution_tabular();
    auto& abd = constitution_tabular.create<AffineBodyConstitution>();

    auto& contact_tabular = scene.contact_tabular();
    auto& default_contact = contact_tabular.default_element();


    auto object = scene.objects().create("cube");

    SimplicialComplexIO io;
    auto mesh = io.read(fmt::format("{}cube.msh", AssetDir::tetmesh_path()));
    // create 5 instances of the mesh, share the underlying mesh
    mesh.instances().resize(5);

    // apply the default contact information to the geometry
    default_contact.apply_to(mesh);

    // apply the constitution to the geometry
    // all the instances will have the same constitution
    abd.apply_to(mesh, 1e8);

    // copy_from the mesh to the object
    // to create the geometry and the rest geometry for simulation
    auto [geo, rest_geo] = object->geometries().create(mesh);

    // initialize the world using the scene
    world.init(scene);

    std::string output_path = fmt::format("{}/{}", this_output_path, name);

    fs::exists(output_path) || fs::create_directories(output_path);

    {
        std::ofstream f(fmt::format("{}/engine.json", output_path, name));
        f << engine.to_json().dump(4);
    }

    std::size_t total_frames = 1;

    // main loop
    while(total_frames--)
    {
        world.advance();
        world.sync();
        world.retrieve();
    }
}


TEST_CASE("engine", "[world]")
{
    test_engine("none");
    test_engine("cuda");
}
