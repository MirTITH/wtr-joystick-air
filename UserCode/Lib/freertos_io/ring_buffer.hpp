#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>

template <size_t BufferSize>
class RingBuffer
{
public:
    uint8_t buffer_[BufferSize];
    size_t write_pos_ = 0; // 下一次写入的位置
    size_t read_pos_  = 0; // 下一次读取的位置
    size_t size_      = 0; // 元素个数

    /**
     * @brief 将 pos 转换到合法的数组 index.
     *
     * @note 出于代码效率考量，pos 不得大于 2 * BufferSize
     *
     */
    size_t ConvertToValidIndex(size_t pos)
    {
        if (pos >= BufferSize) {
            return pos - BufferSize;
        }

        return pos;
    }

    /**
     * @brief 重设读写位置
     * @note 当 size_ == 0 时重设读写位置可以使得连续部分尽可能长
     */
    void ResetPos()
    {
        write_pos_ = 0;
        read_pos_  = 0;
    }

public:
    /**
     * @brief 获取元素个数
     *
     * @return size_t 范围 [0, BufferSize]
     */
    size_t size() const
    {
        return size_;
    }

    void clear()
    {
        size_      = 0;
        write_pos_ = 0;
        read_pos_  = 0;
    }

    /**
     * @brief 在后面插入一个元素
     *
     * @param element 要插入的元素
     * @return size_t 成功插入的个数（0 或 1）
     */
    size_t push_back(uint8_t element)
    {
        if (size_ == BufferSize) {
            return 0;
        }

        buffer_[write_pos_] = element;
        write_pos_          = ConvertToValidIndex(write_pos_ + 1);
        size_++;
        return 1;
    }

    /**
     * @brief 从前面弹出一个元素
     * @note 不能对空的缓冲区弹出
     * @return uint8_t 被弹出的元素
     */
    uint8_t pop_front()
    {
        assert(size_ > 0); // 缓冲区非空
        auto result = buffer_[read_pos_];
        read_pos_   = ConvertToValidIndex(read_pos_ + 1);
        size_--;
        if (size_ == 0) {
            ResetPos();
        }

        return result;
    }

    /**
     * @brief 缓冲区剩余总空间
     *
     * @return size_t 剩余空间大小
     */
    size_t available_size() const
    {
        return BufferSize - size_;
    }

    /**
     * @brief 从前面删除
     *
     * @param count 删除元素的个数
     */
    void remove_front(size_t count)
    {
        assert(count <= size_);

        read_pos_ = ConvertToValidIndex(read_pos_ + count);
        size_ -= count;
        if (size_ == 0) {
            ResetPos();
        }
    }

    /**
     * @brief 将写入标志向后移动
     *
     * @param count 移动的个数（不能超过 available_size）
     */
    void move_write_pos_back(size_t count)
    {
        assert(count <= available_size());

        write_pos_ = ConvertToValidIndex(write_pos_ + count);
        size_ += count;
    }

    /**
     * @brief 获取可以连续读取的数据大小（如用DMA）
     *
     * @note 当可以不折返地读取所有数据时，返回的值等于 size()；否则等于不折返所能读取的最大数据长度
     *
     * @return size_t
     */
    size_t get_continous_read_size() const
    {
        if (write_pos_ > read_pos_) {
            return size_;
        }

        // 程序中，如果 size_ == 0，会调用 ResetPos()
        // 如果 write_pos_ == read_pos_ 且 size_ == 0, 应返回 0, 这与 size_ 相同
        // 如果 write_pos_ == read_pos_ 且 size_ != 0, 应返回 BufferSize - read_pos_
        // 如果 write_pos_ < read_pos_: 应返回 BufferSize - read_pos_

        if (size_ == 0) {
            return 0;
        }

        return BufferSize - read_pos_;
    }

    size_t get_continous_write_size() const
    {
        if (write_pos_ > read_pos_) {
            return BufferSize - write_pos_;
        }

        // 程序中，如果 size_ == 0，会调用 ResetPos()
        // 如果 write_pos_ == read_pos_ 且 size_ == 0, 应返回 BufferSize - write_pos_, 这与 available_size() 相同
        // 如果 write_pos_ == read_pos_ 且 size_ != 0, 则 size_ == BufferSize, 应返回 0, 这与 available_size() 相同
        // 如果 write_pos_ < read_pos_: 应返回 read_pos_ - write_pos_, 这与 available_size() 相同
        return available_size();
    }

    auto get_read_address() const
    {
        return &buffer_[read_pos_];
    }

    auto get_write_address()
    {
        return &buffer_[write_pos_];
    }

    auto get_buffer_ptr()
    {
        return buffer_;
    }

    /**
     * @brief 向缓冲区后面写入大量数据
     *
     * @param data 要写入的数据指针
     * @param data_size 要写入的数据大小
     * @return size_t 实际写入的数据
     */
    size_t push_data_back(const uint8_t *data, size_t data_size)
    {
        if (data_size > available_size()) {
            data_size = available_size();
        }

        auto continous_write_size = get_continous_write_size();
        if (data_size <= continous_write_size) {
            memcpy(get_write_address(), data, data_size);
        } else {
            memcpy(get_write_address(), data, continous_write_size);
            memcpy(get_buffer_ptr(), data + continous_write_size, data_size - continous_write_size);
        }

        move_write_pos_back(data_size);
        return data_size;
    }

    /**
     * @brief 从缓冲区前面读取大量数据
     *
     * @param dst 读到的数据会写到这里
     * @param max_read_size 最大读取数据大小
     * @return size_t 实际读取数据大小
     */
    size_t pop_data_front(uint8_t *dst, size_t max_read_size)
    {
        if (max_read_size > size_) {
            max_read_size = size_;
        }

        auto continous_read_size = get_continous_read_size();
        if (max_read_size <= continous_read_size) {
            memcpy(dst, get_read_address(), max_read_size);
        } else {
            memcpy(dst, get_read_address(), continous_read_size);
            memcpy(dst + continous_read_size, get_buffer_ptr(), max_read_size - continous_read_size);
        }

        remove_front(max_read_size);
        return max_read_size;
    }
};
