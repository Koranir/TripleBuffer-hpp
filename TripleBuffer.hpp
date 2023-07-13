/**
* Copyright (c) Daniel Yoon 2023
**/

#ifndef TRIPLE_BUFFER_H_
#define TRIPLE_BUFFER_H_

#include <array>
#include <cinttypes>
#include <mutex>

/// @brief A thread-safe multi-buffering solution for transferring constant, up-to-date data from one thread to another without risk of data being overwritten.
/// @tparam Buffer The Buffer type will have [sz] copies made.
/// @tparam sz The size of the multi-buffer.
template<typename Buffer>
class TripleBuffer {
public:
    /// @brief Default initialisation of contents.
    constexpr TripleBuffer() {
        buffers = std::array<Buffer, 3>();
    }

    /// @brief Constructs TripleBuffer with buffers already constructed. Moves the buffers.
    /// @param buffers An array of three buffers.
    constexpr TripleBuffer(const std::array<Buffer, 3>& buffers) : buffers(std::move(buffers)) {}

    // Prevent moving any TripleBuffers
    TripleBuffer(const TripleBuffer&) = delete;
    TripleBuffer& operator=(const TripleBuffer&) = delete;

    /// @brief Get the next writable buffer. Make sure that no references to the previous written buffer is still around, or it will be corrupted.
    /// @return A reference to the write buffer.
    inline Buffer& nextWriteBuffer() {
        const std::unique_lock<std::mutex> lock(stateChangeMutex);

        latestBuffer = writeBuffer;
        std::size_t nextWriteBufferIndex = buffers.size() - (latestBuffer + readBuffer);
        writeBuffer = nextWriteBufferIndex;

        return buffers[writeBuffer];
    }

    /// @brief Get the latest readable buffer. If no buffer has finished being written to since last call, it will return the same buffer.
    /// @return A reference to the read buffer.
    inline Buffer& nextReadBuffer() {
        const std::unique_lock<std::mutex> lock(stateChangeMutex);

        readBuffer = latestBuffer;

        return buffers[readBuffer];
    }

    inline std::size_t _writeIndex() const {
        return writeBuffer;
    }
    inline std::size_t _readIndex() const {
        return readBuffer;
    }
    inline std::size_t _latestIndex() const {
        return latestBuffer;
    }
    inline std::array<Buffer, 3>& _buffers() {
        return buffers
    }

private:
    std::mutex stateChangeMutex;

    std::size_t latestBuffer = 0;
    std::size_t readBuffer = 0;
    std::size_t writeBuffer = 1;

    std::array<Buffer, 3> buffers;
};

#endif
