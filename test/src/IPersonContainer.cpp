#include "Persons.h"
#include <ioccpp/IoCContainerImpl.hh>

template class IoCContainer<IPerson>;
template class IoCRegisterScoped<IPerson>;
