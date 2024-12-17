#pragma once

#define VIRTUAL_MIN(x, y) ((x < y) ? x : y)
#define VIRTUAL_MAX(x, y) ((x > y) ? x : y)
#define VIRTUAL_PIN(a, min_value, max_value) VIRTUAL_MIN(max_value, VIRTUAL_MAX(a, min_value))

#define VIRTUAL_VALID_INDEX(idx, range) ((idx >= 0) && (idx < range))
#define VIRTUAL_PIN_INDEX(idx, range) VIRTUAL_PIN(idx, 0, range - 1)

#define VIRTUAL_SIGN(x) ((x > 0.0f ? 1.0f : 0.0f) + (x < 0.0f ? -1.0f : 0.0f))