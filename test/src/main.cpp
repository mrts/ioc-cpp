#include "Persons.h"

#include <ioccpp/ioc.h>

#include <testcpp/testcpp.h>

#include <boost/make_shared.hpp>

class TestIoC : public Test::Suite
{
public:
    TESTCPP_TYPEDEF_TESTMETHOD(TestIoC)

    void test()
    {
        testSingleton();
        testLazySingleton();
        testScopedRegistration();
        testInvalidUsage();
    }

    void testSingleton()
    {
        IoCContainer<IPerson>::Register(boost::make_shared<Developer>());

        IPerson& person = IoCContainer<IPerson>::Resolve();

        assertEqual(person.role(), "Developer");
    }

    void testLazySingleton()
    {
        IoCContainer<IPerson>::RegisterFactory(managerFactory);

        IoCContainer<IPerson>::Reset();
        assertFalse(IoCContainer<IPerson>::DoesInstanceExist());

        IPerson& person = IoCContainer<IPerson>::Resolve();

        assertTrue(IoCContainer<IPerson>::DoesInstanceExist());
        assertEqual(person.role(), "Manager");
    }

    void testFactory()
    {
        IoCContainer<IPerson>::RegisterFactory(testerFactory);

        Tester::ResetCreationCount();
        assertEqual(Tester::CreationCount(), 0u);

        IoCContainer<IPerson>::ResolveNew();
        boost::shared_ptr<IPerson> person = IoCContainer<IPerson>::ResolveNew();

        assertEqual(person->role(), "Tester");
        assertEqual(Tester::CreationCount(), 2u);
    }

    void testScopedRegistration()
    {
        IoCContainer<IPerson>::Register(boost::make_shared<Developer>());
        IPerson& person = IoCContainer<IPerson>::Resolve();
        assertEqual(person.role(), "Developer");

        {
            IoCRegisterScoped<IPerson> scopedRegistrationGuard(boost::make_shared<Tester>());
            IPerson& scopedPerson = IoCContainer<IPerson>::Resolve();
            assertEqual("Scoped registration guard overrides registered type inside a scope",
                scopedPerson.role(), "Tester");
        }

        assertEqual("Original type is restored after registration guard goes out of scope",
            person.role(), "Developer");
    }

    void testInvalidUsage()
    {
        assertThrows(TestIoC, TestMethod, IoCError,
            *this, &TestIoC::resolveFromUninitializedContainer);

        assertThrows(TestIoC, TestMethod, IoCError,
            *this, &TestIoC::resolveFromUninitializedFactory);

        assertThrows(TestIoC, TestMethod, IoCError,
            *this, &TestIoC::registerNullObject);

        assertThrows(TestIoC, TestMethod, IoCError,
            *this, &TestIoC::registerNullFactory);
    }

    void resolveFromUninitializedContainer()
    {
        IoCContainer<IPerson>::Reset();
        IoCContainer<IPerson>::ResetFactory();

        IoCContainer<IPerson>::Resolve();
    }

    void resolveFromUninitializedFactory()
    {
        IoCContainer<IPerson>::ResetFactory();

        IoCContainer<IPerson>::ResolveNew();
    }

    void registerNullObject()
    {
        IoCContainer<IPerson>::Register(boost::shared_ptr<IPerson>());
    }

    void registerNullFactory()
    {
        IoCContainer<IPerson>::RegisterFactory(NULL);
    }
};

int main()
{
    Test::Controller &controller = Test::Controller::instance();
    controller.addTestSuite("ioc-cpp tests", Test::Suite::instance<TestIoC>);
    return controller.run();
}
