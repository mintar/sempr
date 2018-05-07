#ifndef SEMPR_CORE_ENTITYEVENT_H_
#define SEMPR_CORE_ENTITYEVENT_H_

#include <sempr/core/Event.hpp>
#include <sempr/core/Utility.hpp>

namespace sempr {

namespace entity {
    class Entity;
}

namespace storage {
    class DBObject;
}

namespace core {


/**
    This is the base for all EntityEvents. It holds a type (created, changed, removed, loaded) and
    the actual pointer to an Entity.
*/
class EntityEventBase : public Event {
public:
    using Ptr = std::shared_ptr<EntityEventBase>;
    enum EventType {
        CREATED,
        CHANGED,
        REMOVED,
        LOADED
    };

    using EntityPtr = std::shared_ptr<entity::Entity>;
    EntityEventBase(EntityPtr eptr, EventType t) : type_(t), entity_(eptr) {}
    virtual ~EntityEventBase(){}

    /** returns the internal event type (created, changed, removed) */
    EventType what() { return type_; }

    /** returns the entity that this event refers to. */
    EntityPtr getEntity() { return entity_; };

protected:
    EventType type_;
    EntityPtr entity_;
};



// -------------------------------------------------------------------------
// Interface
// -------------------------------------------------------------------------


/**
    The EntityEvent-class automatically constructs an inheritance chain that follows the known types
    of the entity it is constructed for. E.g., consider the entity inheritance chain:

    Entity <-- Geometry <-- Polygon
                    ^------ Point

    We use the type traits generated by ODB to follow the inheritance upwards: A
    EntityEvent<Polygon> automatically inherits EntityEvent<Geometry> and EntityEvent<Polygon>,
    which in turn register their types, allowing processing modules to process Polygons and
    Points event when they only registered for Geometry.
*/
/// While the Entity still has a known base class, inherit EntityEvent<Base>.
template <  class EntityType,
            class Base = typename base_of<EntityType>::type >
class EntityEvent : public EntityEvent<Base> {
public:
    using Ptr = std::shared_ptr<EntityEvent<EntityType>>;
    using EntityPtr = std::shared_ptr<EntityType>;

public:
    EntityEvent(EntityPtr eptr, EntityEventBase::EventType type)
        : EntityEvent<Base>(eptr, type)
    {
        this->template registerType<EntityEvent>();
    }

    /** returns the entity that this event referes to */
    std::shared_ptr<EntityType> getEntity() {
        // sadly, no covariant return types with shared_ptrs.
        return std::static_pointer_cast<EntityType>(this->entity_);
    };

    static Ptr create(EntityPtr e, EntityEventBase::EventType type) {
        return Ptr(new EntityEvent<EntityType>(e, type));
    }
};


/**
    Recursion anchor: When base_of<Entity> is 'void', the last step is to inherit Event.
    (Actually, it is easier to wait for EntityEvent<void, void>, so that EntityEvent<Entity, void>
    is still handled by the general definition above.)
*/
template <>
class EntityEvent<void, void> : public EntityEventBase {
protected:
    using EntityPtr = std::shared_ptr<entity::Entity>;
    EntityEvent(EntityPtr eptr, EventType type)
        : EntityEventBase(eptr, type)
    {
    };
};

/**
    Specialization for DBObject / alternative recursion anchor.
    This one will be used most of the time, since currently all entities derive from DBObject --
    which might change in the future.
*/
template <>
class EntityEvent<storage::DBObject, void> : public EntityEventBase {
protected:
    using EntityPtr = std::shared_ptr<entity::Entity>;
    EntityEvent(EntityPtr eptr, EventType type)
        : EntityEventBase(eptr, type)
    {
    }
};


} /* core */

} /* sempr */


#endif /* end of include guard: SEMPR_CORE_ENTITYEVENT_H_ */
