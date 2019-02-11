#pragma once

const size_t BUFFER_LEN = 8192;
const TBUFIDX INVALID_IDX = -1;

enum EIOTyp
{
	EIO_Invalid = -1,
	EIO_Accept,
	EIO_Read,
	EIO_Write,

	EIO_CNT
};
