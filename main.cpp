// boost-ringbuf.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <boost/circular_buffer.hpp>
#include <numeric>
#include <assert.h>
#include "ring_buf.h"

void test1()
{
	// Create a circular buffer of capacity 3.
	boost::circular_buffer<int> cb(3);
	assert(cb.capacity() == 3);
	// Check is empty.
	assert(cb.size() == 0);
	assert(cb.empty());

	// Insert some elements into the circular buffer.
	cb.push_back(1);
	cb.push_back(2);

	// Assertions to check push_backs have expected effect.
	assert(cb[0] == 1);
	assert(cb[1] == 2);
	assert(!cb.full());
	assert(cb.size() == 2);
	assert(cb.capacity() == 3);

	// Insert some other elements.
	cb.push_back(3);
	cb.push_back(4);

	// Evaluate the sum of all elements.
	int sum = std::accumulate(cb.begin(), cb.end(), 0);

	// Assertions to check state.
	assert(sum == 9);
	assert(cb[0] == 2);
	assert(cb[1] == 3);
	assert(cb[2] == 4);
	assert(*cb.begin() == 2);
	assert(cb.front() == 2);
	assert(cb.back() == 4);
	assert(cb.full());
	assert(cb.size() == 3);
	assert(cb.capacity() == 3);
}

void test2()
{
	typedef boost::circular_buffer<int> circular_buffer;
	circular_buffer cb{ 3 };

	cb.push_back(0);
	cb.push_back(1);
	cb.push_back(2);
	cb.push_back(3);

	std::cout << std::boolalpha << cb.is_linearized() << '\n';

	circular_buffer::array_range ar1, ar2;

	ar1 = cb.array_one();
	ar2 = cb.array_two();
	std::cout << ar1.second << ";" << ar2.second << '\n';

	for (int i : cb)
		std::cout << i << '\n';

	cb.linearize();

	ar1 = cb.array_one();
	ar2 = cb.array_two();
	std::cout << ar1.second << ";" << ar2.second << '\n';

}

void test3()
{
	ring_buf_t<10, int> buf;
	int arr1[10] = { 1 ,2 ,3 ,4 ,5, 6, 7, 8, 9 };

	buf.write(arr1, 7);

	int read_arr[10] = { 0 };
	buf.read((void*)read_arr, 5);
	for (int i = 0; i < 10; i++) {
		std::cout << ": " << read_arr[i] << std::endl;
	}

	buf.write(arr1, 7);

	buf.read(read_arr, 5);
	for (int i = 0; i < 10; i++) {
		std::cout << ": " << read_arr[i] << std::endl;
	}
	std::cout << "size: " << buf.read_available() << std::endl;
}

int main(int /*argc*/, char* /*argv*/[])
{
	test3();
	//test2();
	return 0;
}