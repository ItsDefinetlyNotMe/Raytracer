#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS
template <typename T>
bool floating_equal(T const x, T const y) {
	if (x - y >= 0)
		return (x - y) <= 0.01f;//std::numeric_limits<T>::epsilon();
	return (y - x) <= 0.01f;//std::numeric_limits<T>::epsilon();
}
#endif // !HELPER_FUNCTIONS
