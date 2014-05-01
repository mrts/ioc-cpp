ioc-cpp
=======

Inversion of control/dependency injection container for C++03.

Usage:

    #include <ioccpp/ioc.h>
    ...
    // in composition root/assembler
    IoCContainer<IPerson>::Register(boost::make_shared<Developer>());
    ...
    // later in code
    IPerson& person = IoCContainer<IPerson>::Resolve();

or, alternatively, use a factory function for lazy initialization/getting copies:

    #include <ioccpp/ioc.h>
    ...
    boost::shared_ptr<IPerson> developerFactory() { return boost::make_shared<Developer>(); }
    ...
    IoCContainer<IPerson>::RegisterFactory(developerFactory);
    ...
    IPerson& person = IoCContainer<IPerson>::Resolve();
    boost::shared_ptr<IPerson> newPerson = IoCContainer<IPerson>::ResolveNew();

Note that you must create the symobol for each interface as follows:

    // in IocContainterIPerson.cpp
    #include "Persons.h"
    #include <ioccpp/IoCContainerImpl.hh>
    
    template class IoCContainer<IPerson>;
