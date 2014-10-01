#include "Persons.h"

#include <ioccpp/ioc.h>

#include <testcpp/testcpp.h>

class TestIoC : public Test::Suite
{
public:
    TESTCPP_TYPEDEFS(TestIoC)

    void test()
    {
        testSingleton();
        testLazySingleton();
        testScopedRegistration();
        testInvalidUsage();
    }

    void testSingleton()
    {
        IoCContainer<IPerson>::Register(stdutil::make_shared<Developer>());

        IPerson& person = IoCContainer<IPerson>::Resolve();
        stdutil::shared_ptr<IPerson> person_ptr =
            IoCContainer<IPerson>::ResolvePtr();

        assertEqual(person.role(), "Developer");
        assertEqual(person_ptr->role(), "Developer");
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
        stdutil::shared_ptr<IPerson> person =
            IoCContainer<IPerson>::ResolveNew();

        assertEqual(person->role(), "Tester");
        assertEqual(Tester::CreationCount(), 2u);
    }

    void testScopedRegistration()
    {
        IoCContainer<IPerson>::Register(stdutil::make_shared<Developer>());
        IPerson& person = IoCContainer<IPerson>::Resolve();
        assertEqual(person.role(), "Developer");

        {
            IoCRegisterScoped<IPerson> scopedRegistrationGuard(
                stdutil::make_shared<Tester>());
            IPerson& scopedPerson = IoCContainer<IPerson>::Resolve();
            assertEqual("Scoped registration guard overrides registered type inside a scope",
                scopedPerson.role(), "Tester");
        }

        assertEqual("Original type is restored after registration guard goes out of scope",
            person.role(), "Developer");
    }

    void testInvalidUsage()
    {
        assertThrows(resolveFromUninitializedContainer, IoCError);
        assertThrows(resolveFromUninitializedFactory, IoCError);
        assertThrows(registerNullObject, IoCError);
        assertThrows(registerNullFactory, IoCError);
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
        IoCContainer<IPerson>::Register(stdutil::shared_ptr<IPerson>());
    }

    void registerNullFactory()
    {
#ifdef IOCCPP_HAVE_CPP11
        IoCContainer<IPerson>::RegisterFactory(nullptr);
#else
        IoCContainer<IPerson>::RegisterFactory(NULL);
#endif
    }
};

int main()
{
    Test::Controller &controller = Test::Controller::instance();
    controller.addTestSuite("ioc-cpp tests", Test::Suite::instance<TestIoC>);
    return controller.run();
}
