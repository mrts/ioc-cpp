#ifndef PERSONS_H__
#define PERSONS_H__

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class IPerson
{
public:
    virtual ~IPerson() {}
    virtual const char* role() const = 0;
};

class Developer : public IPerson
{
public:
    virtual const char* role() const
    { return "Developer"; }
};

class Manager : public IPerson
{
public:
    virtual const char* role() const
    { return "Manager"; }
};

class Tester : public IPerson
{
public:
    Tester()
    { ++creationCount; }

    virtual const char* role() const
    { return "Tester"; }

    static unsigned int CreationCount()
    { return creationCount; }

    static void ResetCreationCount()
    { creationCount = 0; }

private:
    static unsigned int creationCount;
};

inline boost::shared_ptr<IPerson> managerFactory()
{
    return boost::make_shared<Manager>();
}

inline boost::shared_ptr<IPerson> testerFactory()
{
    return boost::make_shared<Tester>();
}

#endif /* PERSONS_H */
