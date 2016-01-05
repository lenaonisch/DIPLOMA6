#pragma once
#include "StdAfx.h"

using namespace concurrency;
// Read character at index idx from array arr.
template <typename T>
unsigned int read_uchar(T& arr, int idx) restrict(amp)
{
    return (arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3))) >> ((idx & 0x3) << 3);
}

// Increment character at index idx in array arr.
template<typename T>
void increment_uchar(T& arr, int idx) restrict(amp)
{
    atomic_fetch_add(&arr[idx >> 2], 1 << ((idx & 0x3) << 3));
}

// Add value val to character at index idx in array arr.
template<typename T>
void addto_uchar(T& arr, int idx, unsigned int val) restrict(amp)
{
    atomic_fetch_add(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}

// Write value val to character at index idx in array arr.
template<typename T>
void write_uchar(T& arr, int idx, unsigned int val) restrict(amp)
{
    atomic_fetch_xor(&arr[idx >> 2], arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3)));
    atomic_fetch_xor(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}

// Helper function to accept 1D indices of index<1> type instead of integers.
template <typename T>
unsigned int read_uchar(T& arr, index<1> idx) restrict(amp) { return read_uchar(arr, idx[0]); }
template<typename T>
void increment_uchar(T& arr, index<1> idx) restrict(amp) { increment_uchar(arr, idx[0]); }
template<typename T>
void addto_uchar(T& arr, index<1> idx, unsigned int val) restrict(amp) { addto_uchar(arr, idx[0], val); }
template<typename T>
void write_uchar(T& arr, index<1> idx, unsigned int val) restrict(amp) { write_uchar(arr, idx[0], val); }

// 2D indexes
template <typename T>
unsigned int read_uchar(T& arr, int idx0, int idx1, int cols) restrict(amp)
{
	int idx = idx0*cols+idx1;
    return (arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3))) >> ((idx & 0x3) << 3);
}
template<typename T>
void increment_uchar(T& arr, int idx0, int idx1, int cols) restrict(amp)
{
	int idx = idx0*cols+idx1;
    atomic_fetch_add(&arr[idx >> 2], 1 << ((idx & 0x3) << 3));
}
template<typename T>
void addto_uchar(T& arr, int idx0, int idx1, int cols, unsigned int val) restrict(amp)
{
	int idx = idx0*cols+idx1;
    atomic_fetch_add(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}
template<typename T>
void write_uchar(T& arr, int idx0, int idx1, int cols, unsigned int val) restrict(amp)
{
	int idx = idx0*cols+idx1;
    atomic_fetch_xor(&arr[idx >> 2], arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3)));
    atomic_fetch_xor(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}

// Helper function to accept 2D indices of index<1> type instead of integers.
template <typename T>
unsigned int read_uchar(T& arr, index<2> idx, int step) restrict(amp) { return read_uchar(arr, idx[0], idx[1], step); }
template<typename T>
void increment_uchar(T& arr, index<2> idx, int step) restrict(amp) { increment_uchar(arr, idx[0], idx[1], step); }
template<typename T>
void addto_uchar(T& arr, index<2> idx, int step, unsigned int val) restrict(amp) { addto_uchar(arr, idx[0], idx[1], step, val); }
template<typename T>
void write_uchar(T& arr, index<2> idx, int step, unsigned int val) restrict(amp) { write_uchar(arr, idx[0], idx[1], step, val); }

// 3D indexes
template <typename T>
unsigned int read_uchar(T& arr, index<3>idx, int step, int channels) restrict(amp, cpu)
{
	//int step = cols*channels;
	int index = idx[0]*step + idx[1]*channels + idx[2];
    return (arr[index >> 2] & (0xFF << ((index & 0x3) << 3))) >> ((index & 0x3) << 3);
}

template<typename T>
void addto_uchar(T& arr, index<3>idx, int cols, int channels, unsigned int val) restrict(amp)
{
	int step = cols*channels;
	int idx = idx[0]*step+idx[1]*channels+idx[2];
    atomic_fetch_add(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}
template<typename T>
void write_uchar(T& arr, index<3>idx, int cols, int channels, unsigned int val) restrict(amp)
{
	int step = cols*channels;
	int idx = idx[0]*step+idx[1]*channels+idx[2];
    atomic_fetch_xor(&arr[idx >> 2], arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3)));
    atomic_fetch_xor(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}

// 3D indexes
template <typename T>
unsigned int read_uchar(T& arr, index<2>idx, int channel, int cols, int channels) restrict(amp)
{
	int step = cols*channels;
	int idx = idx[0]*step+idx[1]*channels+channel;
    return (arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3))) >> ((idx & 0x3) << 3);
}

template<typename T>
void addto_uchar(T& arr, index<2>idx, int channel, int cols, int channels, unsigned int val) restrict(amp)
{
	int step = cols*channels;
	int idx = idx[0]*step+idx[1]*channels+channel;
    atomic_fetch_add(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}
template<typename T>
void write_uchar(T& arr, index<2>idx, int channel, int cols, int channels, unsigned int val) restrict(amp)
{
	int step = cols*channels;
	int idx = idx[0]*step+idx[1]*channels+channel;
    atomic_fetch_xor(&arr[idx >> 2], arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3)));
    atomic_fetch_xor(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}
