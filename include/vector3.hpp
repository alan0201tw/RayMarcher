#pragma once

/**
 *   This should be the only place that uses the header file "linalg.h",
 *   This file serves as a wrapper around external dependencies, in this case
 *   the "linalg.h". It's good to have a wrapper in practice, making it easy
 *   to switch vector/matrix libraries.
 */

#include "linalg.h"

using Vector3 = linalg::aliases::float3;
using Matrix3x3 = linalg::aliases::float3x3;