#include <sempr/entity/spatial/Surface.hpp>
#include <Surface_odb.h>

namespace sempr { namespace entity {

SEMPR_ENTITY_SOURCE(Surface)

Surface::Surface() : Surface(new core::IDGen<Surface>())
{
}

Surface::Surface(const core::IDGenBase* idgen)
    : Geometry(idgen)
{
    this->setDiscriminator<Surface>();
}

Surface::~Surface()
{
}

OGRSurface* Surface::geometry() {
    // to be overridden by concrete surfaces.
    return NULL;
}

Surface::Ptr Surface::clone() const {
    // raw clone is virtual! :)
    return Surface::Ptr(raw_clone());
}

Surface* Surface::raw_clone() const
{
    return NULL;
}

}}
