#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS
template <typename T>
bool floating_equal(T const x, T const y) {
	if (x - y >= 0)
		return (x - y) <= 0.0005f;
	return (y - x) <= 0.0005f;
}
#endif // !HELPER_FUNCTIONS
