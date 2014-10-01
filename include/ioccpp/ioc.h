#ifndef IOCCPP_IOC_H__
#define IOCCPP_IOC_H__

#include <stdexcept>

#include "detect_cpp11.h"

#ifdef IOCCPP_HAVE_CPP11
#include <memory>
#include <functional>
namespace stdutil = std;
#else
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
namespace stdutil = boost;
#endif

template <typename T>
class IoCRegisterScoped;

/** Simple inversion of control dependency resolver. */
template <typename T>
class IoCContainer
{
public:
    typedef stdutil::shared_ptr<T> object_ptr;
    typedef stdutil::function<object_ptr(void)> factory_ptr;

    static void Register(const object_ptr& object);
    static void RegisterFactory(const factory_ptr& factory);

    static bool DoesInstanceExist();

    static void Reset();
    static void ResetFactory();

    static T& Resolve();
    static object_ptr ResolvePtr();
    static object_ptr ResolveNew();

private:
    object_ptr object;
    factory_ptr factory;

    IoCContainer();

    typedef IoCContainer<T> this_type;
    static this_type& instance();

    friend class IoCRegisterScoped<T>;
};

/** Scope guard that registers the object given in constructur
 * for the duration of its lifetime in the IoC container and
 * puts the old object back in the destructor.
 */
template <typename T>
class IoCRegisterScoped
{
public:
    typedef stdutil::shared_ptr<T> object_ptr;

    explicit IoCRegisterScoped(const object_ptr& object);
    ~IoCRegisterScoped();

private:
    object_ptr old_object;
};

class IoCError : public std::runtime_error
{
public:
    explicit IoCError(const std::string& message,
            const std::string& objectType)
        : std::runtime_error(message + " for " + objectType)
    { }

    virtual ~IoCError() throw()
    { }
};

#endif /* IOCCPP_IOC_H */
