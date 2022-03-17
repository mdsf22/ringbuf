#pragma once

#include <boost/circular_buffer.hpp>

template <size_t N, typename T = char>
class ring_buf_t
{
public:
	ring_buf_t();
	~ring_buf_t();

	size_t write(const T* data, size_t n);
	size_t read(void* data, size_t n);
	size_t write_available();
	size_t read_available();

private:
	boost::circular_buffer<T> buf_;
};

template <size_t N, typename T>
ring_buf_t<N, T>::ring_buf_t(): buf_(N)
{
}

template <size_t N, typename T>
ring_buf_t<N, T>::~ring_buf_t()
{
}

template <size_t N, typename T>
size_t ring_buf_t<N, T>::write(const T* data, size_t n)
{
	size_t m = std::min<size_t>(n, buf_.capacity() - buf_.size());
	for (int i = 0; i < m; i++) {
		buf_.push_back(data[i]);
	}
	return m;
}

template <size_t N, typename T>
size_t ring_buf_t<N, T>::read(void* data, size_t n)
{
	if (buf_.size() == 0) {
		return 0;
	}

	size_t m = std::min<size_t>(n, buf_.size());
	auto begin = buf_.array_one();
	if (buf_.is_linearized()) {
		memcpy(data, begin.first, m * sizeof(T));
		buf_.erase_begin(m);
	}
	else {
		auto off = std::min<size_t>(m, begin.second);
		memcpy(data, begin.first, off * sizeof(T));
		if (off < m) {
			auto end = buf_.array_two();
			memcpy((char*)data + off * sizeof(T), end.first, (m - off) * sizeof(T));
		}
		buf_.erase_begin(m);

	}
	return m;
}

template <size_t N, typename T>
size_t ring_buf_t<N, T>::write_available()
{
	return buf_.capacity() - buf_.size();
}

template <size_t N, typename T>
size_t ring_buf_t<N, T>::read_available()
{
	return buf_.size();
}
