#ifndef IOCCPP_IOC_H__
#define IOCCPP_IOC_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

template <typename T>
class IoCRegisterScoped;

/** Simple inversion of control dependency resolver. */
template <typename T>
class IoCContainer
{
public:
    typedef boost::shared_ptr<T> type_ptr;
    typedef boost::function<type_ptr (void)> factory_ptr;

    static void Register(const type_ptr& object);
    static void RegisterFactory(const factory_ptr& factory);

    static bool DoesInstanceExist();

    static void Reset();

    static T& Resolve();
    static type_ptr ResolveNew();

private:
    type_ptr object;
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
    explicit IoCRegisterScoped(const boost::shared_ptr<T>& t);
    ~IoCRegisterScoped();

private:
    boost::shared_ptr<T> old_object;
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
