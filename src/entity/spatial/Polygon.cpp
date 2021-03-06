#include <sempr/entity/spatial/Polygon.hpp>
#include <Polygon_odb.h>

namespace sempr { namespace entity {

SEMPR_ENTITY_SOURCE(Polygon)

Polygon::Polygon() : Polygon(new core::IDGen<Polygon>())
{
}

Polygon::Polygon(const core::IDGenBase* idgen)
    : CurvePolygon(idgen)
{
    this->setDiscriminator<Polygon>();
    geometry_ = static_cast<OGRPolygon*>(OGRGeometryFactory::createGeometry(wkbPolygon));
}

Polygon::~Polygon()
{
    OGRGeometryFactory::destroyGeometry(geometry_);
}

OGRPolygon* Polygon::geometry() {
    return geometry_;
}

Polygon::Ptr Polygon::clone() const {
    // raw clone is virtual! :)
    return Polygon::Ptr(raw_clone());
}

Polygon* Polygon::raw_clone() const
{
    Polygon* newInstance = new Polygon();
    // set the same reference frame
    newInstance->setCS(this->getCS());

    // copy the geometry
    *(newInstance->geometry_) = *geometry_; // use OGRGeometry copy ctor
    return newInstance;
}

}}
