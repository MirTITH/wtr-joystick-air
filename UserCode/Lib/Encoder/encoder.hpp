#pragma once
#include <cstdint>
class Encoder
{
private:
    bool prevA_;
    bool prevB_;
    volatile int32_t count_        = 0;
    volatile uint32_t error_count_ = 0;

public:
    void Init(bool A, bool B)
    {
        prevA_ = A;
        prevB_ = B;
    }

    int32_t Count() const
    {
        return count_;
    }

    int32_t ErrorCount() const
    {
        return error_count_;
    }

    void ExtiCallback(bool A, bool B)
    {
        if ((prevA_ ^ A) != (prevB_ ^ B)) {
            // A B 发生了变化

            if ((prevA_ ^ prevB_) ^ (prevA_ ^ A)) {
                count_--;
            } else {
                count_++;
            }
        } else {
            // A B 没发生变化，正常情况不应发生
            error_count_++;
        }
        prevA_ = A;
        prevB_ = B;
    }
};
