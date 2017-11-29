#include <sempr/entity/Person.hpp>
#include <sempr/entity/Triple.hpp>
#include <Person_odb.h>
#include <sempr/core/RDF.hpp>

namespace sempr { namespace entity {

Person::Person(const core::IDGenBase& idgen)
    : Entity(idgen)
{
    setDiscriminator<Person>();
    prop_ = RDFPropertyMap::Ptr(new RDFPropertyMap(*this));
    registerChildEntity(prop_);

    RDFPropertyMap& m = *prop_;
    m["name"] = "Max Mustermann";
    m["age"] = 42;
    m["height"] = 1.85;
}

Person::Person() : Person(core::IDGen<Person>())
{
}


} /* entity */

} /* sempr */
