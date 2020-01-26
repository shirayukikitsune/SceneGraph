#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE kitsune::scenegraph::MultiPtr
#include <boost/test/unit_test.hpp>

#include "Base/MultiPtr.h"

using kitsune::scenegraph::MultiPtr;

class DestructorTestClass {
public:
    bool *ref;

    ~DestructorTestClass() {
        *ref = true;
    }
};

BOOST_AUTO_TEST_SUITE(MultiPtrTest)

BOOST_AUTO_TEST_CASE(EmptyCheck)
{
    MultiPtr<int> ptr;

    BOOST_TEST(!(bool)ptr, "MultiPtr should be empty");
}

BOOST_AUTO_TEST_CASE(NotEmptyCheck)
{
    MultiPtr<int> ptr(new int);

    BOOST_TEST((bool)ptr, "MultiPtr should not be empty");
}

BOOST_AUTO_TEST_CASE(ValueCheck)
{
    int *pointer = new int;
    MultiPtr<int> ptr(pointer);

    BOOST_TEST(ptr.get() == pointer, "MultiPtr should store the pointer");
}

BOOST_AUTO_TEST_CASE(DestructorTest)
{
    bool destructorCalled = false;
    MultiPtr<DestructorTestClass> ptr(new DestructorTestClass);
    ptr->ref = &destructorCalled;

    ptr.reset();
    BOOST_TEST(destructorCalled, "Pointer must be deleted");
}

BOOST_AUTO_TEST_CASE(CustomDeleterTest)
{
    bool deleterCalled = false;
    int *pointer = new int;
    auto deleter = [&deleterCalled](int *p) {
        deleterCalled = true;
        delete p;
    };
    MultiPtr<int, kitsune::scenegraph::delete_condition::all, decltype(&deleter)> ptr(pointer, &deleter);
    ptr.reset();

    BOOST_TEST(deleterCalled, "Pointer must be deleted");
}

BOOST_AUTO_TEST_SUITE_END()
