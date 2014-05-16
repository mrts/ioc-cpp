#ifndef IOCCONTAINERIMPL_HH__
#define IOCCONTAINERIMPL_HH__

#include "ioc.h"

#include <typeinfo>
#include <stdexcept>

template <typename T>
void IoCContainer<T>::Register(const typename IoCContainer::object_ptr& o)
{
    if (!o)
        throw IoCError("Cannot register null object", typeid(T).name());

    instance().object = o;
}

template <typename T>
void IoCContainer<T>::RegisterFactory(const typename IoCContainer::factory_ptr& f)
{
    if (!f)
        throw IoCError("Cannot register null factory", typeid(T).name());

    instance().factory = f;
}

template <typename T>
bool IoCContainer<T>::DoesInstanceExist()
{ return static_cast<bool>(instance().object); }

template <typename T>
void IoCContainer<T>::Reset()
{
    instance().object.reset();
}

template <typename T>
void IoCContainer<T>::ResetFactory()
{
    instance().factory = NULL;
}

template <typename T>
T& IoCContainer<T>::Resolve()
{
    this_type& self = instance();

    if (self.object)
        return *self.object;

    if (self.factory) {
        Register(self.factory());
        return *self.object;
    }

    throw IoCError("Neither object nor factory registered", typeid(T).name());
}

template <typename T>
typename IoCContainer<T>::object_ptr IoCContainer<T>::ResolveNew()
{
    this_type& self = instance();

    if (self.factory)
        return self.factory();

    throw IoCError("Factory not registered", typeid(T).name());
}

template <typename T>
IoCContainer<T>::IoCContainer() :
    object(),
    factory()
{ }

template <typename T>
IoCContainer<T>& IoCContainer<T>::instance()
{
    static this_type self;
    return self;
}


template <typename T>
IoCRegisterScoped<T>::IoCRegisterScoped(const boost::shared_ptr<T>& object) :
    old_object(IoCContainer<T>::instance().object)
{
    IoCContainer<T>::Register(object);
}

template <typename T>
IoCRegisterScoped<T>::~IoCRegisterScoped()
{
    IoCContainer<T>::Register(old_object);
}

#endif /* IOCCONTAINERIMPL_HH */
