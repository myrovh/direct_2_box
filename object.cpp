#include "object.h"

object::object()
{
	reference_count = 0;
	filename = NULL;
}

object::~object()
{
	release();
}